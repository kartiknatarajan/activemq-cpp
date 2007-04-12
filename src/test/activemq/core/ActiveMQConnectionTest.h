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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/network/Socket.h>
#include <activemq/network/ServerSocket.h>
#include <activemq/concurrent/Concurrent.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/concurrent/Thread.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <cms/Connection.h>
#include <activemq/transport/DummyTransport.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConnectionData.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/connector/stomp/StompConnector.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/transport/DummyTransportFactory.h>
#include <activemq/transport/TransportFactoryMap.h>
#include <activemq/transport/TransportFactoryMapRegistrar.h>
#include <activemq/connector/stomp/StompConsumerInfo.h>
#include <activemq/connector/stomp/StompProducerInfo.h>
#include <activemq/connector/stomp/StompTransactionInfo.h>
#include <activemq/connector/stomp/StompSessionInfo.h>
#include <activemq/connector/stomp/StompTopic.h>
#include <activemq/connector/stomp/commands/TextMessageCommand.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace core{

    class ActiveMQConnectionTest : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( ActiveMQConnectionTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();

    public:

    	ActiveMQConnectionTest() {};
    	virtual ~ActiveMQConnectionTest() {}

        class MyCommandListener : public transport::CommandListener{
        public:

            transport::Command* cmd;

        public:

            MyCommandListener(){
                cmd = NULL;
            }
            virtual ~MyCommandListener(){}

            virtual void onCommand( transport::Command* command ){
                cmd = command;
            }
        };

        class MyMessageListener :
            public connector::ConsumerMessageListener
        {
        public:

            std::vector<connector::ConsumerInfo*> consumers;

        public:
            virtual ~MyMessageListener(){}

            virtual void onConsumerMessage(
                connector::ConsumerInfo* consumer,
                core::ActiveMQMessage* msg AMQCPP_UNUSED )
            {
                consumers.push_back( consumer );
            }
        };

        class MyExceptionListener : public cms::ExceptionListener{
        public:

            bool caughtOne;

        public:

            MyExceptionListener(){ caughtOne = false; }
            virtual ~MyExceptionListener(){}

            virtual void onException(const cms::CMSException& ex AMQCPP_UNUSED){
                caughtOne = true;
            }
        };

        class MyDispatcher : public Dispatcher
        {
        public:

            std::vector<ActiveMQMessage*> messages;

        public:
            virtual ~MyDispatcher(){}

            virtual void dispatch( DispatchData& data )
                throw ( exceptions::ActiveMQException )
            {
                messages.push_back( data.getMessage() );
            }
        };

        void test()
        {
            try
            {
                transport::TransportFactoryMapRegistrar registrar(
                    "dummy", new transport::DummyTransportFactory() );

                MyMessageListener listener;
                MyExceptionListener exListener;
                MyCommandListener cmdListener;
                MyDispatcher msgListener;
                std::string connectionId = "testConnectionId";
                util::SimpleProperties* properties =
                    new util::SimpleProperties();
                transport::Transport* transport = NULL;

                transport::TransportFactory* factory =
                    transport::TransportFactoryMap::getInstance().lookup(
                        "dummy" );
                if( factory == NULL ){
                    CPPUNIT_ASSERT( false );
                }

                // Create the transport.
                transport = factory->createTransport( *properties );
                if( transport == NULL ){
                    CPPUNIT_ASSERT( false );
                }

                transport::DummyTransport* dTransport =
                    dynamic_cast< transport::DummyTransport*>( transport );

                CPPUNIT_ASSERT( dTransport != NULL );

                dTransport->setCommandListener( &cmdListener );

                connector::stomp::StompConnector* connector =
                    new connector::stomp::StompConnector(
                        transport, *properties );

                connector->start();

                ActiveMQConnection connection(
                    new ActiveMQConnectionData(
                        connector, transport, properties) );

                // First - make sure exceptions are working.
                connection.setExceptionListener( &exListener );
                CPPUNIT_ASSERT( exListener.caughtOne == false );
                dTransport->fireException( exceptions::ActiveMQException( __FILE__, __LINE__, "test" ) );
                CPPUNIT_ASSERT( exListener.caughtOne == true );

                cms::Session* session1 = connection.createSession();
                cms::Session* session2 = connection.createSession();
                cms::Session* session3 = connection.createSession();

                CPPUNIT_ASSERT( session1 != NULL );
                CPPUNIT_ASSERT( session2 != NULL );
                CPPUNIT_ASSERT( session3 != NULL );

                connector::stomp::StompSessionInfo session;
                connector::stomp::StompConsumerInfo consumer;

                session.setSessionId( 1 );
                session.setConnectionId( "TEST:123" );
                session.setAckMode( cms::Session::AUTO_ACKNOWLEDGE );

                connector::stomp::StompTopic myTopic( "test" );
                consumer.setConsumerId( 1 );
                consumer.setSessionInfo( &session );
                consumer.setDestination( &myTopic );

                connection.addDispatcher( &consumer, &msgListener );

                connector::stomp::commands::TextMessageCommand* cmd =
                    new connector::stomp::commands::TextMessageCommand;
                connector::stomp::StompTopic topic1( "test" );
                cmd->setCMSDestination( &topic1 );

                connector::ConsumerMessageListener* consumerListener =
                    dynamic_cast< connector::ConsumerMessageListener* >(
                        &connection );

                connection.start();

                CPPUNIT_ASSERT( consumerListener != NULL );

                consumerListener->onConsumerMessage( &consumer, cmd );

                CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener.messages.size() );

                connection.removeDispatcher( &consumer );

                msgListener.messages.clear();
                consumerListener->onConsumerMessage( &consumer, cmd );

                CPPUNIT_ASSERT_EQUAL( 0, (int)msgListener.messages.size() );

                connection.addDispatcher( &consumer, &msgListener );

                connection.stop();
                consumerListener->onConsumerMessage( &consumer, cmd );
                connection.start();
                CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener.messages.size() );

                delete cmd;
                cmd = new connector::stomp::commands::TextMessageCommand;

                connector::stomp::StompTopic topic2( "test" );
                cmd->setCMSDestination( &topic2 );

                consumerListener->onConsumerMessage( &consumer, cmd );
                CPPUNIT_ASSERT_EQUAL( 2, (int)msgListener.messages.size() );

                connection.removeDispatcher( &consumer );
                msgListener.messages.clear();

                session1->close();
                session2->close();
                session3->close();
                connection.close();

                exListener.caughtOne = false;
                consumerListener->onConsumerMessage( &consumer, cmd );
                CPPUNIT_ASSERT( exListener.caughtOne == true );

                delete cmd;

                delete session1;
                delete session2;
                delete session3;

            } catch( exceptions::ActiveMQException& ex ) {
                ex.printStackTrace();
                throw ex;
            }
        }

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONTEST_H_*/