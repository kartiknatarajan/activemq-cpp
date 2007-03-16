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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORYTEST_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORYTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/network/Socket.h>
#include <activemq/network/ServerSocket.h>
#include <activemq/concurrent/Concurrent.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <cms/Connection.h>
#include <activemq/transport/TransportFactoryMapRegistrar.h>
#include <activemq/transport/DummyTransportFactory.h>
#include <activemq/connector/Connector.h>

namespace activemq{
namespace core{

    class ActiveMQConnectionFactoryTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( ActiveMQConnectionFactoryTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( test2 );
        CPPUNIT_TEST_SUITE_END();

    public:

        std::string username;
        std::string password;
        std::string clientId;

    	ActiveMQConnectionFactoryTest(){
            username = "timmy";
            password = "auth";
            clientId = "12345";
        }
    	virtual ~ActiveMQConnectionFactoryTest() {}
        
        void test()
        {
            try
            {
                transport::TransportFactoryMapRegistrar registrar(
                    "dummy", new transport::DummyTransportFactory() );
                    
                std::string URI = 
                    "dummy://127.0.0.1:23232&wireFormat=stomp";

                ActiveMQConnectionFactory connectionFactory( URI );

                cms::Connection* connection = 
                    connectionFactory.createConnection();

                CPPUNIT_ASSERT( connection != NULL );
                
                delete connection;
                
                return;
            }
            AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
            AMQ_CATCHALL_NOTHROW( )
            
            CPPUNIT_ASSERT( false );
        }
        
        void test2()
        {
            try
            {
                transport::TransportFactoryMapRegistrar registrar(
                    "dummy", new transport::DummyTransportFactory() );
                
                std::string URI = std::string() + 
                    "dummy://127.0.0.1:23232&wireFormat=stomp?"
                    "username=" + username + "?password=" + password + 
                    "?client-id=" + clientId;

                ActiveMQConnectionFactory connectionFactory( URI );

                cms::Connection* connection = 
                    connectionFactory.createConnection();
                CPPUNIT_ASSERT( connection != NULL );                

                ActiveMQConnection* amqConnection = 
                    dynamic_cast< ActiveMQConnection* >( connection );
                CPPUNIT_ASSERT( amqConnection != NULL );

                connector::Connector* connector = 
                    dynamic_cast< connector::Connector* >( 
                    amqConnection->getConnectionData()->getConnector() );
                CPPUNIT_ASSERT( connector != NULL );

                CPPUNIT_ASSERT( username == connector->getUsername() );
                CPPUNIT_ASSERT( password == connector->getPassword() );
                CPPUNIT_ASSERT( clientId == connector->getClientId() );

                // Free the allocated connection object.
                delete connection;
                
                return;
            }
            AMQ_CATCH_NOTHROW( exceptions::ActiveMQException )
            AMQ_CATCHALL_NOTHROW( )
            
            CPPUNIT_ASSERT( false );
        }

    };
    
}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORYTEST_H_*/