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

#include "CmsTemplate.h"

using namespace activemq::cmsutil;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate() {
    initDefaults();
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::CmsTemplate(cms::ConnectionFactory* connectionFactory) {
    initDefaults();
    setConnectionFactory(connectionFactory);
}

////////////////////////////////////////////////////////////////////////////////
CmsTemplate::~CmsTemplate() {
    
    destroySessionPools();
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::initDefaults() {
    defaultDestination = NULL;
    defaultDestinationName = "";
    messageIdEnabled = true;
    messageTimestampEnabled = true;
    noLocal = false;
    receiveTimeout = RECEIVE_TIMEOUT_INDEFINITE_WAIT;
    explicitQosEnabled = false;
    deliveryMode = cms::DeliveryMode::PERSISTENT;
    priority = DEFAULT_PRIORITY;
    timeToLive = DEFAULT_TIME_TO_LIVE;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::createSessionPools() {
    
    // Make sure they're destroyed first.
    destroySessionPools();
    
    /**
     * Create the session pools.
     */
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix) {
        sessionPools[ix] = new SessionPool(connection,
                (cms::Session::AcknowledgeMode)ix, 
                getResourceLifecycleManager());
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroySessionPools() {
    
    /**
     * Destroy the session pools.
     */
    for( int ix=0; ix<NUM_SESSION_POOLS; ++ix) {
        if( sessionPools[ix] != NULL ) {
            delete sessionPools[ix];
            sessionPools[ix] = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::init() throw (cms::CMSException, IllegalStateException) {

    // Invoke the base class.
    CmsDestinationAccessor::init();

    // Make sure we have a valid default destination.
    checkDefaultDestination();
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::checkDefaultDestination() throw (IllegalStateException) {
    if (this->defaultDestination == NULL) {
        throw IllegalStateException(
                __FILE__, __LINE__,
                "No defaultDestination or defaultDestinationName specified. Check configuration of CmsTemplate.");
    }
}

////////////////////////////////////////////////////////////////////////////////
cms::Connection* CmsTemplate::getConnection() 
throw (cms::CMSException) {

    try {
        
        // If we don't have a connection, create one.
        if( connection == NULL ) {
        
            // Invoke the base class to create the connection and add it
            // to the resource lifecycle manager.
            connection = createConnection();
            
            // Start the connection.
            connection->start();
            
            // Create the session pools, passing in this connection.
            createSessionPools();
        }
        
        return connection;
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
PooledSession* CmsTemplate::takeSession() 
throw (cms::CMSException) {

    try {
        
        // Take a session from the pool.
        return sessionPools[getSessionAcknowledgeMode()].takeSession();        
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::returnSession( PooledSession*& session ) 
throw (cms::CMSException) {

    try {

        if( session == NULL ) {
            return;
        }
        
        // Close the session, but do not delete since it's a pooled session
        session->close();
        session = NULL;
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
cms::Producer* CmsTemplate::createProducer(cms::Session* session,
        cms::Destination* dest) throw (cms::CMSException) {

    try {

        if( dest == NULL ) {
            dest = getDefaultDestination();
        }
        
        cms::MessageProducer* producer = session->createProducer(dest);
        if (!isMessageIdEnabled()) {
            producer->setDisableMessageID(true);
        }
        if (!isMessageTimestampEnabled()) {
            producer->setDisableMessageTimestamp(true);
        }

        return producer;
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyProducer( cms::MessageProducer*& producer) 
throw (cms::CMSException) {

    if( producer == NULL ) {
        return;
    }
    
    try {        
        
        // Close the producer, then destroy it.
        producer->close();                
    }
    AMQ_CATCH_NO_RETHROW( cms::CMSException )
    
    delete producer;
    producer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
cms::MessageConsumer* CmsTemplate::createConsumer(cms::Session* session,
        cms::Destination* dest, const std::string& messageSelector)
        throw (cms::CMSException) {

    try {
        cms::MessageConsumer* consumer = session->createConsumer(dest,
                messageSelector, 
                isNoLocal());
        
        return consumer;
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyConsumer( cms::MessageConsumer*& consumer) 
throw (cms::CMSException) {

    if( consumer == NULL ) {
        return;
    }
    
    try {        
        
        // Close the consumer, then destroy it.
        consumer->close();                
    }
    AMQ_CATCH_NO_RETHROW( cms::CMSException )
    
    delete consumer;
    consumer = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::destroyMessage( cms::Message*& message) {

    if( message == NULL ) {
        return;
    }
    
    // Destroy the message.      
    delete message;
    message = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(SessionCallback* action) throw (cms::CMSException) {    
    
    PooledSession* pooledSession = NULL;
    
    try {
        
        if( action == NULL ) {
            return;
        }
    
        // Take a session from the pool.
        pooledSession = takeSession();
        
        // Execute the action with the given session.
        action->doInCms(pooledSession);
        
        // Return the session to the pool.
        returnSession(pooledSession);
        
    } catch( cms::CMSException& e ) {
        
        e.setMark(__FILE__, __LINE__);
        
        // Return the session to the pool.
        returnSession(pooledSession);
        
        throw e;
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::execute(ProducerCallback* action) throw (cms::CMSException) {
    
    try {
        
        // Create the callback.
        ProducerSessionCallback cb(action);
        
        // Execute the action in a session.
        execute(&cb);
    }
    AMQ_CATCH_RETHROW( cms::CMSException )
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::ProducerSessionCallback::doInCms( cms::Session* session ) 
throw (cms::CMSException) {
        
    MessageProducer* producer = NULL;
    
    try {
    
        if( session == NULL ) {
            return;
        }
        
        // Create the producer.
        producer = createProducer(session, null);
        
        // Execute the action.
        action->doInCms(session, producer);
        
        // Destroy the producer.
        destroyProducer(producer);
        
    } catch( cms::CMSException& e) {
        
        // Destroy the producer.
        destroyProducer(producer);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CmsTemplate::doSend(cms::Session* session, cms::Destination* dest, 
        MessageCreator* messageCreator) throw (cms::CMSException) {
    
    cms::MessageProducer* producer = NULL;
    cms::Message* message = NULL;
        
    try {
    
        if( session == NULL || dest == NULL) {
            return;
        }
        
        // Create the producer.
        producer = createProducer(session, dest);
        
        // Create the message.
        message = messageCreator->createMessage(session);
        
        // Send the message.
        if( isExplicitQosEnabled() ) {
            producer->send(message, getDeliveryMode(), getPriority(), getTimeToLive());
        } else {
            producer->send(message);
        }
        
        // Destroy the resources.
        destroyProducer(producer);
        destroyMessage(message);
        
    } catch( cms::CMSException& e) {
        
        // Destroy the resources.
        destroyProducer(producer);
        destroyMessage(message);
    }
}

