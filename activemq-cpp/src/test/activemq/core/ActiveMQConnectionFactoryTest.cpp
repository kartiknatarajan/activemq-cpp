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

#include "ActiveMQConnectionFactoryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( activemq::core::ActiveMQConnectionFactoryTest );

#include <activemq/concurrent/Concurrent.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <cms/Connection.h>
#include <activemq/transport/TransportFactoryMapRegistrar.h>
#include <activemq/transport/DummyTransportFactory.h>
#include <activemq/connector/Connector.h>

using namespace activemq;
using namespace activemq::core;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test()
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

////////////////////////////////////////////////////////////////////////////////
void ActiveMQConnectionFactoryTest::test2()
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