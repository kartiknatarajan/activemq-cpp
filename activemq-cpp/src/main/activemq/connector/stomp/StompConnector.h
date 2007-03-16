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

#ifndef ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTOR_H_
#define ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTOR_H_

#include <activemq/connector/Connector.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/Properties.h>
#include <activemq/connector/stomp/StompCommandReader.h>
#include <activemq/connector/stomp/StompCommandWriter.h>
#include <activemq/connector/stomp/StompCommandListener.h>
#include <activemq/connector/stomp/StompSessionManager.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/core/ActiveMQConstants.h>
#include <activemq/exceptions/IllegalArgumentException.h>
#include <activemq/logger/LoggerDefines.h>

namespace activemq{
namespace connector{
namespace stomp{

    /**
     * The connector implementation for the STOMP protocol.
     */
    class StompConnector
    :
        public Connector,
        public transport::CommandListener,
        public transport::TransportExceptionListener,
        public StompCommandListener
    {

        LOGCMS_DECLARE( logger )

    private:

        // Flags the state we are in for connection to broker.
        enum connectionState
        {
            DISCONNECTED,
            CONNECTING,
            CONNECTED
        };

        // Maps Command Ids to listener that are interested
        typedef std::map< commands::CommandConstants::CommandId,
                          StompCommandListener* > CmdListenerMap;

    private:

        /**
         * The transport for sending/receiving commands on the wire.
         */
        transport::Transport* transport;

        /**
         * Flag to indicate the start state of the connector.
         */
        connectionState state;

        /**
         * Sync object.
         */
        concurrent::Mutex mutex;

        /**
         * Observer of messages directed at a particular
         * consumer.
         */
        ConsumerMessageListener* messageListener;

        /**
         * Observer of connector exceptions.
         */
        cms::ExceptionListener* exceptionListener;

        /**
         * This Connector's Command Reader
         */
        StompCommandReader reader;

        /**
         * This Connector's Command Writer
         */
        StompCommandWriter writer;

        /**
         * Map to hold StompCommandListeners
         */
        CmdListenerMap cmdListenerMap;

        /**
         * Session Manager object that will  be allocated when we connect
         */
        StompSessionManager* sessionManager;

        /**
         * Next avaliable Producer Id
         */
        long long nextProducerId;

        /**
         * Next avaliable Transaction Id
         */
        long long nextTransactionId;

        /**
         * Properties for the connector.
         */
        util::SimpleProperties properties;

    private:

        /**
         * Sends the connect message to the broker and
         * waits for the response.
         */
        void connect();

        /**
         * Sends a oneway disconnect message to the broker.
         */
        void disconnect();

        /**
         * Fires an exception event to the observing object.
         * @param ex the exception to fire
         */
        void fire( const exceptions::ActiveMQException& ex ){
            try{
                if( exceptionListener != NULL ){
                    exceptionListener->onException( ex );
                }
            }catch( ... ){/* do nothing*/}
        }

    public:

        /**
         * Constructor for the stomp connector.
         * @param transport the transport object for sending/receiving
         * commands on the wire.
         * @param properties properties for configuring the connector.
         */
        StompConnector( transport::Transport* transport,
                        const util::Properties& properties )
            throw ( exceptions::IllegalArgumentException );

        virtual ~StompConnector();

        /**
         * Starts the service.
         * @throws CMSException
         */
        virtual void start() throw( cms::CMSException );

        /**
         * Closes this object and deallocates the appropriate resources.
         * @throws CMSException
         */
        virtual void close() throw( cms::CMSException );

        /**
         * Gets the Client Id for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Client Id String
         */
        virtual std::string getClientId() const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_CLIENTID ), "" );
        }

        /**
         * Gets the Username for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Username String
         */
        virtual std::string getUsername() const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_USERNAME ), "" );
        }

        /**
         * Gets the Password for this connection, if this
         * connection has been closed, then this method returns ""
         * @return Password String
         */
        virtual std::string getPassword() const {
            return properties.getProperty(
                core::ActiveMQConstants::toString(
                    core::ActiveMQConstants::PARAM_PASSWORD ), "" );
        }

        /**
         * Gets a reference to the Transport that this connection
         * is using.
         * @return reference to a transport
         * @throws InvalidStateException if the Transport is not set
         */
        virtual transport::Transport& getTransport() const
            throw ( exceptions::InvalidStateException ) {

            if( transport == NULL ) {
                throw exceptions::InvalidStateException(
                    __FILE__, __LINE__,
                    "StompConnector::getTransport - "
                    "Invalid State, No Transport.");
            }

            return *transport;
        }

        /**
         * Creates a Session Info object for this connector
         * @param ackMode Acknowledgement Mode of the Session
         * @returns Session Info Object
         * @throws ConnectorException
         */
        virtual SessionInfo* createSession(
            cms::Session::AcknowledgeMode ackMode )
                throw( ConnectorException );

        /**
         * Create a Consumer for the given Session
         * @param destination Destination to Subscribe to.
         * @param session the session this consumer is attached to
         * @param selector the selector string for this consumer
         * @param noLocal should local message be delivered to the consumer
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createConsumer(
            const cms::Destination* destination,
            SessionInfo* session,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException );

        /**
         * Create a Durable Consumer for the given Session
         * @param topic Topic to Subscribe to.
         * @param session Session Information.
         * @param name name of the Durable Topic
         * @param selector Selector String
         * @param noLocal if set, inhibits the delivery of messages
         *        published by its own connection
         * @return Consumer Information
         * @throws ConnectorException
         */
        virtual ConsumerInfo* createDurableConsumer(
            const cms::Topic* topic,
            SessionInfo* session,
            const std::string& name,
            const std::string& selector = "",
            bool noLocal = false )
                throw ( ConnectorException );

        /**
         * Given a valid Consumer info Object that was previously created
         * by a call to <code>createConsumer</code>, the Consumer will be
         * registered with the Broker, and be placed in a state in which
         * it will now be able to receive messages.  All preperations
         * for message receipt should be done before calling this method.
         * @param consumer - ConsumerInfo of a consumer that isn't started
         * @throws ConnectorException
         */
        virtual void startConsumer( ConsumerInfo* consumer AMQCPP_UNUSED )
            throw ( ConnectorException ) {}

        /**
         * Create a Consumer for the given Session
         * @param destination Destination to Subscribe to.
         * @param session Session Information.
         * @return Producer Information
         * @throws ConnectorException
         */
        virtual ProducerInfo* createProducer(
            const cms::Destination* destination,
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Creates a Topic given a name and session info
         * @param name Topic Name
         * @param session Session Information
         * @return a newly created Topic Object
         * @throws ConnectorException
         */
        virtual cms::Topic* createTopic( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Queue given a name and session info
         * @param name Queue Name
         * @param session Session Information
         * @return a newly created Queue Object
         * @throws ConnectorException
         */
        virtual cms::Queue* createQueue( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Temporary Topic given a name and session info
         * @param session Session Information
         * @return a newly created Temporary Topic Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryTopic* createTemporaryTopic(
            SessionInfo* session )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Creates a Temporary Queue given a name and session info
         * @param session Session Information
         * @return a newly created Temporary Queue Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryQueue* createTemporaryQueue(
            SessionInfo* session )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Sends a Message
         * @param message The Message to send.
         * @param producerInfo Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( cms::Message* message, ProducerInfo* producerInfo )
            throw ( ConnectorException );

        /**
         * Sends a set of Messages
         * @param messages List of Messages to send.
         * @param producerInfo Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( std::list<cms::Message*>& messages,
                           ProducerInfo* producerInfo )
            throw ( ConnectorException );

        /**
         * Acknowledges a Message
         * @param session the Session that the message is linked to
         * @param consumer the Consumer that the message was linked to
         * @param message An ActiveMQMessage to Ack.
         * @param ackType the type of ack to perform
         * @throws ConnectorException
         */
        virtual void acknowledge( const SessionInfo* session,
                                  const ConsumerInfo* consumer,
                                  const cms::Message* message,
                                  AckType ackType = ConsumedAck)
            throw ( ConnectorException );

        /**
         * Starts a new Transaction.
         * @param session Session Information
         * @throws ConnectorException
         */
        virtual TransactionInfo* startTransaction(
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Commits a Transaction.
         * @param transaction The Transaction information
         * @param session Session Information
         * @throws ConnectorException
         */
        virtual void commit( TransactionInfo* transaction,
                             SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Rolls back a Transaction.
         * @param transaction The Transaction information
         * @param session Session Information
         * @throws ConnectorException
         */
        virtual void rollback( TransactionInfo* transaction,
                               SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a new Message.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::Message* createMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new BytesMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::BytesMessage* createBytesMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new TextMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::TextMessage* createTextMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new MapMessage.
         * @param session Session Information
         * @param transaction Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::MapMessage* createMapMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Unsubscribe from a givenDurable Subscription
         * @param name name of the Subscription
         * @throws ConnectorException
         */
        virtual void unsubscribe( const std::string& name )
            throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Destroys the given connector resource.
         * @param resource resource the resource to be destroyed.
         * @throws ConnectorException
         */
        virtual void destroyResource( ConnectorResource* resource )
            throw ( ConnectorException );

        /**
         * Sets the listener of consumer messages.
         * @param listener listener the observer.
         */
        virtual void setConsumerMessageListener(
            ConsumerMessageListener* listener )
        {
            this->messageListener = listener;

            if(sessionManager != NULL)
            {
                sessionManager->setConsumerMessageListener( listener );
            }
        }

        /**
         * Sets the Listner of exceptions for this connector
         * @param listener ExceptionListener the observer.
         */
        virtual void setExceptionListener(
            cms::ExceptionListener* listener )
        {
            this->exceptionListener = listener;
        }

    public: // transport::CommandListener

        /**
         * Event handler for the receipt of a non-response command from the
         * transport.
         * @param command command the received command object.
         */
        virtual void onCommand( transport::Command* command );

    public: // TransportExceptionListener

        /**
         * Event handler for an exception from a command transport.
         * @param source The source of the exception
         * @param ex The exception.
         */
        virtual void onTransportException(
            transport::Transport* source,
            const exceptions::ActiveMQException& ex );

    public: // StompCommandListener

        /**
         * Process the Stomp Command
         * @param command command to process
         * @throw ConnterException
         */
        virtual void onStompCommand( commands::StompCommand* command )
            throw ( StompConnectorException );

    public:

        /**
         * Registers a Command Listener using the CommandId specified
         * if there is already a listener for that command it will be
         * removed.
         * @param commandId CommandId to process
         * @param listener pointer to the listener to call
         */
        virtual void addCmdListener(
            commands::CommandConstants::CommandId commandId,
            StompCommandListener* listener );

        /**
         * UnRegisters a Command Listener using the CommandId specified
         * @param commandId CommandId of the listener to remove.
         */
        virtual void removeCmdListener(
            commands::CommandConstants::CommandId commandId );

    private:

        long long getNextProducerId();
        long long getNextTransactionId();

        // Check for Connected State and Throw an exception if not.
        void enforceConnected() throw ( ConnectorException );

    };

}}}

#endif /*ACTIVEMQ_CONNECTOR_STOMP_STOMPCONNECTOR_H_*/