/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ServerSocket.h"
#include "SocketError.h"

#ifdef HAVE_WINSOCK2_H
    #include <Winsock2.h>
    #include <Ws2tcpip.h>
    #include <sys/stat.h>
    #define stat _stat
#else
    #include <unistd.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <sys/file.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <assert.h>
#include <string>

using namespace decaf::net;

#ifdef HAVE_WINSOCK2_H

    // Static socket initializer needed for winsock

    ServerSocket::StaticServerSocketInitializer::StaticServerSocketInitializer () {
        socketInitError = NULL;
        const WORD version_needed = MAKEWORD(2,2); // lo-order byte: major version
        WSAData temp;
        if( WSAStartup(version_needed, &temp )){
           clear();
               socketInitError = new SocketException ( __FILE__, __LINE__,
                   "winsock.dll was not found");
        }
    }
    ServerSocket::StaticServerSocketInitializer::~StaticServerSocketInitializer () {
        clear();
        WSACleanup();
    }

    // Create static instance of the socket initializer.
    ServerSocket::StaticServerSocketInitializer
        ServerSocket::staticSocketInitializer;

#endif


////////////////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket()
{
    socketHandle = Socket::INVALID_SOCKET_HANDLE;

#if defined(HAVE_WINSOCK2_H)
    if( ServerSocket::staticSocketInitializer.getSocketInitError() != NULL ) {
        throw *ServerSocket::staticSocketInitializer.getSocketInitError();
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
ServerSocket::~ServerSocket()
{
    // No shutdown, just close - dont want blocking destructor.
    close();
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const char* host, int port ) throw ( SocketException )
{
    bind (host, port, SOMAXCONN);
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::bind( const char* host,
                         int port,
                         int backlog ) throw ( SocketException )
{
    if(isBound()) {
        throw SocketException ( __FILE__, __LINE__,
            "ServerSocket::bind - Socket already bound" );
    }

    // Create the socket.
    socketHandle = ::socket(AF_INET, SOCK_STREAM, 0 );
    if( socketHandle < 0) {
        socketHandle = Socket::INVALID_SOCKET_HANDLE;
            throw SocketException( __FILE__, __LINE__, SocketError::getErrorString().c_str());
    }

    // Verify the port value.
    if( port <= 0 || port > 65535 ) {
        throw SocketException( __FILE__, __LINE__,
            "ServerSocket::bind - Port out of range: %d", port );
    }

    sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons((short)port);
    bind_addr.sin_addr.s_addr = 0; // To be set later down...
    memset(&bind_addr.sin_zero, 0, sizeof(bind_addr.sin_zero));
    int status;

    // Resolve name
#if defined(HAVE_STRUCT_ADDRINFO)
    ::addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = PF_INET;
    struct addrinfo *res_ptr = NULL;
    status = ::getaddrinfo(host, NULL, &hints, &res_ptr);
    if( status != 0 || res_ptr == NULL) {
        throw SocketException( __FILE__, __LINE__, SocketError::getErrorString().c_str() );
    }
    assert(res_ptr->ai_addr->sa_family == AF_INET);
    // Porting: On both 32bit and 64 bit systems that we compile to soo far, sin_addr is a 32 bit value, not an unsigned long.
    assert(sizeof(((sockaddr_in*)res_ptr->ai_addr)->sin_addr.s_addr) == 4);
    bind_addr.sin_addr.s_addr = ((sockaddr_in*)res_ptr->ai_addr)->sin_addr.s_addr;
    freeaddrinfo(res_ptr);
#else
    struct ::hostent *he = ::gethostbyname(host);
    if( he == NULL ) {
        throw SocketException( __FILE__, __LINE__, "Failed to resolve hostname" );
    }
    bind_addr.sin_addr.s_addr = *((in_addr_t *)he->h_addr);
#endif


    // Set the socket to reuse the address.
    int value = 1;
    ::setsockopt(socketHandle, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(int) );

    status = ::bind(socketHandle,
             reinterpret_cast<sockaddr*>(&bind_addr), sizeof( bind_addr ));

    if( status < 0 ){
        close();
        throw SocketException ( __FILE__, __LINE__,
            "ServerSocket::bind - %s", SocketError::getErrorString().c_str() );
    }
    status = ::listen( socketHandle, (int)backlog );
    if( status < 0 ) {
        close();
        throw SocketException( __FILE__, __LINE__, SocketError::getErrorString().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ServerSocket::close() throw (cms::CMSException){

    if( isBound() ) {

        #if !defined(HAVE_WINSOCK2_H)
            ::close( socketHandle );
        #else
            ::closesocket( socketHandle );
        #endif

        socketHandle = Socket::INVALID_SOCKET_HANDLE;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool ServerSocket::isBound() const {
    return this->socketHandle != Socket::INVALID_SOCKET_HANDLE;
}

////////////////////////////////////////////////////////////////////////////////
Socket* ServerSocket::accept() throw (SocketException)
{
    struct sockaddr_in temp;

    #if !defined(HAVE_WINSOCK2_H)
        socklen_t temp_len = sizeof( sockaddr_in );
    #else
        int temp_len = sizeof( sockaddr_in );
    #endif

    SocketHandle ss_socket_handle = NULL;

    // Loop to ignore any signal interruptions that occur during the operation.
    do {

        ss_socket_handle = ::accept( socketHandle,
                                     reinterpret_cast<struct sockaddr*>(&temp),
                                     &temp_len );

    } while( ss_socket_handle < 0 &&
             SocketError::getErrorCode() == SocketError::INTERRUPTED );

    if( ss_socket_handle < 0 ) {
        throw SocketException( __FILE__, __LINE__,
            "ServerSocket::accept- %s", SocketError::getErrorString().c_str() );
    }

    return new TcpSocket( ss_socket_handle );
}
