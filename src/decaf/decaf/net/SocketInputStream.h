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

#ifndef _DECAF_NET_SOCKETINPUTSTREAM_H_
#define _DECAF_NET_SOCKETINPUTSTREAM_H_

#include <decaf/io/InputStream.h>
#include <decaf/net/Socket.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/lang/Exception.h>

namespace decaf{
namespace net{

    /**
     * Input stream for performing reads on a socket.  This
     * class will only work properly for blocking sockets.
     */
    class SocketInputStream : public io::InputStream
    {
    private:

        // The socket handle.
        Socket::SocketHandle socket;
        util::concurrent::Mutex mutex;

    public:

        /**
         * Constructor.
         * @param socket the socket handle.
         */
        SocketInputStream( Socket::SocketHandle socket );

        /**
         * Destructor.
         */
        virtual ~SocketInputStream();

        /**
         * Locks the object.
         * @throws Exception
         */
        virtual void lock() throw( lang::Exception ){
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws Exception
         */
        virtual void unlock() throw( lang::Exception ){
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void wait() throw( lang::Exception ){
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs time in millisecsonds to wait, or WAIT_INIFINITE
         * @throws Exception
         */
        virtual void wait( unsigned long millisecs )
            throw( lang::Exception ) {

            mutex.wait( millisecs );
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notify() throw( lang::Exception ){
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws Exception
         */
        virtual void notifyAll() throw( lang::Exception ){
            mutex.notifyAll();
        }

        /**
         * Returns the number of bytes available on the socket to
         * be read right now.
         * @return The number of bytes currently available to
         * be read on the socket.
         */
        virtual std::size_t available() const throw ( io::IOException );

        /**
         * Reads a single byte from the buffer.  If no data
         * is available, blocks until their is.
         * @return The next byte.
         * @throws IOException thrown if an error occurs.
         */
        virtual unsigned char read() throw ( io::IOException );

        /**
         * Reads an array of bytes from the buffer.  If no data
         * is available, blocks until there is.
         * @param buffer (out) the target buffer.
         * @param bufferSize the size of the output buffer.
         * @return The number of bytes read.
         * @throws IOException thrown if an error occurs.
         */
        virtual std::size_t read( unsigned char* buffer,
                                  std::size_t bufferSize )
            throw ( io::IOException );

        /**
         * Close - does nothing.  It is the responsibility of the owner
         * of the socket object to close it.
         * @throws CMSException
         */
        virtual void close() throw( lang::Exception ){}

        /**
         * Not supported.
         * @throws an UnsupportedOperationException.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( io::IOException,
                    lang::exceptions::UnsupportedOperationException );

    };

}}

#endif /*_DECAF_NET_SOCKETINPUTSTREAM_H_*/