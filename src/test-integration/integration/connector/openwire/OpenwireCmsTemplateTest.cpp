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

#include "OpenwireCmsTemplateTest.h"
#include <integration/IntegrationCommon.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/Properties.h>
#include <activemq/transport/TransportFactory.h>
#include <decaf/util/UUID.h>
#include <decaf/util/Properties.h>
#include <decaf/util/StringTokenizer.h>
#include <activemq/connector/ConnectorFactoryMap.h>
#include <decaf/net/SocketFactory.h>
#include <activemq/transport/TransportFactory.h>
#include <decaf/net/Socket.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <decaf/util/StringTokenizer.h>
#include <decaf/lang/Boolean.h>

#include <cms/Connection.h>
#include <cms/MessageConsumer.h>
#include <cms/MessageProducer.h>
#include <cms/MessageListener.h>
#include <cms/Startable.h>
#include <cms/Closeable.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

using namespace activemq::transport;
using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::connector;
using namespace activemq::exceptions;
using namespace decaf::net;
using namespace activemq::transport;
using namespace decaf::util::concurrent;
using namespace decaf::lang;
using namespace decaf::util;

using namespace integration;
using namespace integration::connector::openwire;
using namespace activemq::cmsutil;

////////////////////////////////////////////////////////////////////////////////
void OpenwireCmsTemplateTest::setUp() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireCmsTemplateTest::tearDown() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireCmsTemplateTest::testBasics()
{
    try {
        const unsigned int NUM_MESSAGES = IntegrationCommon::defaultMsgCount;
        
        Receiver receiver( IntegrationCommon::getInstance().getOpenwireURL(), 
                false, 
                "testBasics1", 
                NUM_MESSAGES);
        Thread rt(&receiver);
        rt.start();
        
        // Wait for receiver thread to start.
        decaf::lang::Thread::sleep(100);
        
        Sender sender( IntegrationCommon::getInstance().getOpenwireURL(), 
                false, 
                "testBasics1", 
                NUM_MESSAGES);
        Thread st(&sender);
        st.start();
        
        st.join();
        rt.join();
        
        unsigned int numReceived = receiver.getNumReceived();
        if( IntegrationCommon::debug ) {
            printf("received: %d\n", numReceived );
        }
        CPPUNIT_ASSERT(
            numReceived == NUM_MESSAGES );
    } catch ( ActiveMQException e ) {
        e.printStackTrace();
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireCmsTemplateTest::testReceiveException()
{
    try {
        // First, try receiving from a bad url
        activemq::core::ActiveMQConnectionFactory cf("tcp://localhost:61666"); // Invalid URL (at least by default)
        activemq::cmsutil::CmsTemplate cmsTemplate(&cf);
        cmsTemplate.setDefaultDestinationName("testReceive1");
        try {                
            cmsTemplate.receive();
            CPPUNIT_FAIL("failed to throw expected exception");
        }
        catch( ActiveMQException& ex) {
            // Expected.
        }
        
        // Now change to a good url and verify that we can reuse the same
        // CmsTemplate successfully.
        activemq::core::ActiveMQConnectionFactory cf2(IntegrationCommon::getInstance().getOpenwireURL());
        cmsTemplate.setConnectionFactory(&cf2);
        
        // Send 1 message.
        Sender sender( IntegrationCommon::getInstance().getOpenwireURL(), 
                false, 
                "testReceive1", 
                1);
        Thread st(&sender);
        st.start();
        
        // Receive the message.
        cms::Message* message = cmsTemplate.receive();
        CPPUNIT_ASSERT(message != NULL);
        delete message;
    }
    catch ( ActiveMQException e ) {
        e.printStackTrace();
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void OpenwireCmsTemplateTest::testSendException()
{
    try {
        // First, try sending to a bad url.
        activemq::core::ActiveMQConnectionFactory cf("tcp://localhost:61666"); // Invalid URL (at least by default)
        activemq::cmsutil::CmsTemplate cmsTemplate(&cf);
        cmsTemplate.setDefaultDestinationName("testSend1");
        try {                      
            TextMessageCreator msgCreator("hello world");
            cmsTemplate.send(&msgCreator);
            CPPUNIT_FAIL("failed to throw expected exception");
        }
        catch( ActiveMQException& ex) {
            // Expected.
        }
        
        // Now change to a good url and verify that we can reuse the same
        // CmsTemplate successfully.
        activemq::core::ActiveMQConnectionFactory cf2(IntegrationCommon::getInstance().getOpenwireURL());
        cmsTemplate.setConnectionFactory(&cf2);
        TextMessageCreator msgCreator("hello world");
        cmsTemplate.send(&msgCreator);
    } catch ( ActiveMQException e ) {
        e.printStackTrace();
        throw e;
    }
}