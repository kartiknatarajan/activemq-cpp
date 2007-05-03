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

#ifndef _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_
#define _ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_

#include <activemq/connector/Connector.h>

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

#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/exceptions/IllegalArgumentException.h>

#include <activemq/transport/Transport.h>
#include <activemq/connector/SessionInfo.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/connector/ProducerInfo.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/connector/ConsumerMessageListener.h>
#include <activemq/connector/ConnectorException.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/SimpleProperties.h>
#include <activemq/core/ActiveMQConstants.h>

#include <activemq/connector/openwire/OpenWireCommandReader.h>
#include <activemq/connector/openwire/OpenWireCommandWriter.h>
#include <activemq/connector/openwire/OpenWireFormat.h>
#include <activemq/connector/openwire/OpenWireFormatNegotiator.h>
#include <activemq/connector/openwire/OpenWireConsumerInfo.h>

#include <activemq/connector/openwire/commands/ActiveMQTempDestination.h>
#include <activemq/connector/openwire/commands/BrokerInfo.h>
#include <activemq/connector/openwire/commands/ConnectionInfo.h>
#include <activemq/connector/openwire/commands/ConsumerInfo.h>
#include <activemq/connector/openwire/commands/LocalTransactionId.h>
#include <activemq/connector/openwire/commands/WireFormatInfo.h>

namespace activemq{
namespace connector{
namespace openwire{

    class OpenWireConnector :
        public Connector,
        public transport::CommandListener,
        public transport::TransportExceptionListener
    {
    private:

        // Flags the state we are in for connection to broker.
        enum connectionState
        {
            DISCONNECTED,
            CONNECTING,
            CONNECTED
        };

        // Enumeration of Transaction State flags
        enum TransactionType
        {
            TRANSACTION_BEGIN = 0,
            TRANSACTION_PREPARE = 1,
            TRANSACTION_COMMITONEPHASE = 2,
            TRANSACTION_COMMITTWOPHASE = 3,
            TRANSACTION_ROLLBACK = 4,
            TRANSACTION_RECOVER = 5,
            TRANSACTION_FORGET = 6,
            TRANSACTION_END = 7
        };

    private:

        /**
         * The transport for sending/receiving commands on the wire.
         */
        transport::Transport* transport;

        /**
         * The OpenWireFormat class that controls Protocal versions and
         * marshalling details.
         */
        OpenWireFormat* wireFormat;

        /**
         * Connection Information for this connection to the Broker
         */
        commands::ConnectionInfo connectionInfo;

        /**
         * Command sent from the Broker with its BrokerInfo
         */
        commands::BrokerInfo* brokerInfo;

        /**
         * Command sent from the Broker with its WireFormatInfo
         */
        commands::WireFormatInfo* brokerWireFormatInfo;

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
        OpenWireCommandReader reader;

        /**
         * This Connector's Command Writer
         */
        OpenWireCommandWriter writer;

        /**
         * Next avaliable Producer Id
         */
        long long nextProducerId;

        /**
         * Next avaliable Producer Sequence Id
         */
        long long nextProducerSequenceId;

        /**
         * Next avaliable Consumer Id
         */
        long long nextConsumerId;

        /**
         * Next avaliable Transaction Id
         */
        long long nextTransactionId;

        /**
         * Next available Session Id.
         */
        long long nextSessionId;

        /**
         * Next Temporary Destination Id
         */
        long long nextTempDestinationId;

        /**
         * Properties for the connector.
         */
        util::SimpleProperties properties;

        /**
         * Mapping of consumer IDs to their respective
         * consumer info object.
         */
        util::Map< long long, OpenWireConsumerInfo* > consumerInfoMap;

    private:

        /**
         * Sends the connect message to the broker and
         * waits for the response.
         */
        void connect() throw (ConnectorException);

        /**
         * Sends a oneway disconnect message to the broker.
         */
        void disconnect() throw (ConnectorException);

        /**
         * Fires a consumer message to the observer.
         * @param msg - the message to file
         */
        void fire( ConsumerInfo* consumer, core::ActiveMQMessage* msg ){
            try{
                if( messageListener != NULL ){
                    messageListener->onConsumerMessage(
                        consumer,
                        msg );
                }
            }catch( ... ){/* do nothing*/}
        }

        /**
         * Fires an exception event to the observing object.
         * @param ex - the exception to fire.
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
         * Constructor for the OpenWire connector.
         * @param transport - the transport object for sending/receiving
         *                    commands on the wire.
         * @param properties - properties for configuring the connector.
         */
        OpenWireConnector( transport::Transport* transport,
                           const util::Properties& properties )
            throw ( exceptions::IllegalArgumentException );

        virtual ~OpenWireConnector();

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
         * @param reference to a transport
         * @throws InvalidStateException if the Transport is not set
         */
        virtual transport::Transport& getTransport() const
            throw ( exceptions::InvalidStateException ) {

            if( transport == NULL ) {
                throw exceptions::InvalidStateException(
                    __FILE__, __LINE__,
                    "OpenWireConnector::getTransport - "
                    "Invalid State, No Transport.");
            }

            return *transport;
        }

        /**
         * Creates a Session Info object for this connector
         * @param Acknowledgement Mode of the Session
         * @returns Session Info Object
         * @throws ConnectorException
         */
        virtual SessionInfo* createSession(
            cms::Session::AcknowledgeMode ackMode )
                throw( ConnectorException );

        /**
         * Create a Consumer for the given Session
         * @param Destination to Subscribe to.
         * @param Session Information.
         * @param Message Selector
         * @param No Local redelivery indicator
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
         * @param Topic to Subscribe to.
         * @param Session Information.
         * @param name of the Durable Topic
         * @param Selector
         * @param if set, inhibits the delivery of messages
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
        virtual void startConsumer( ConsumerInfo* consumer )
            throw ( ConnectorException );

        /**
         * Create a Consumer for the given Session
         * @param Destination to Subscribe to.
         * @param Session Information.
         * @return Producer Information
         * @throws ConnectorException
         */
        virtual ProducerInfo* createProducer(
            const cms::Destination* destination,
            SessionInfo* session)
                throw ( ConnectorException );

        /**
         * Creates a Topic given a name and session info
         * @param Topic Name
         * @param Session Information
         * @return a newly created Topic Object
         * @throws ConnectorException
         */
        virtual cms::Topic* createTopic( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Queue given a name and session info
         * @param Queue Name
         * @param Session Information
         * @return a newly created Queue Object
         * @throws ConnectorException
         */
        virtual cms::Queue* createQueue( const std::string& name,
                                         SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a Temporary Topic given a name and session info
         * @param Temporary Topic Name
         * @param Session Information
         * @return a newly created Temporary Topic Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryTopic* createTemporaryTopic(
            SessionInfo* session )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Creates a Temporary Queue given a name and session info
         * @param Temporary Queue Name
         * @param Session Information
         * @return a newly created Temporary Queue Object
         * @throws ConnectorException
         */
        virtual cms::TemporaryQueue* createTemporaryQueue(
            SessionInfo* session )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Sends a Message
         * @param The Message to send.
         * @param Producer Info for the sender of this message
         * @throws ConnectorException
         */
        virtual void send( cms::Message* message, ProducerInfo* producerInfo )
            throw ( ConnectorException );

        /**
         * Sends a set of Messages
         * @param List of Messages to send.
         * @param Producer Info for the sender of this message
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
                                  AckType ackType )
            throw ( ConnectorException );

        /**
         * Starts a new Transaction.
         * @param Session Information
         * @throws ConnectorException
         */
        virtual TransactionInfo* startTransaction(
            SessionInfo* session )
                throw ( ConnectorException );

        /**
         * Commits a Transaction.
         * @param The Transaction information
         * @param Session Information
         * @throws ConnectorException
         */
        virtual void commit( TransactionInfo* transaction,
                             SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Rolls back a Transaction.
         * @param The Transaction information
         * @param Session Information
         * @throws ConnectorException
         */
        virtual void rollback( TransactionInfo* transaction,
                               SessionInfo* session )
            throw ( ConnectorException );

        /**
         * Creates a new Message.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::Message* createMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new BytesMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::BytesMessage* createBytesMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new TextMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::TextMessage* createTextMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException );

        /**
         * Creates a new MapMessage.
         * @param Session Information
         * @param Transaction Info for this Message
         * @throws ConnectorException
         */
        virtual cms::MapMessage* createMapMessage(
            SessionInfo* session,
            TransactionInfo* transaction )
                throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Unsubscribe from a givenDurable Subscription
         * @param name of the Subscription
         * @throws ConnectorException
         */
        virtual void unsubscribe( const std::string& name )
            throw ( ConnectorException, exceptions::UnsupportedOperationException );

        /**
         * Closes the given connector resource, caller must still delete
         * the resource once its been closed.
         * @param resource the resource to be closed
         * @throws ConnectorException
         */
        virtual void closeResource( ConnectorResource* resource )
            throw ( ConnectorException );

        /**
         * Sets the listener of consumer messages.
         * @param listener the observer.
         */
        virtual void setConsumerMessageListener(
            ConsumerMessageListener* listener )
        {
            this->messageListener = listener;
        }

        /**
         * Sets the Listner of exceptions for this connector
         * @param ExceptionListener the observer.
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
         * @param command the received command object.
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

    private:

        long long getNextConsumerId();
        long long getNextProducerId();
        long long getNextTransactionId();
        long long getNextSessionId();
        long long getNextTempDestinationId();
        long long getNextProducerSequenceId();

        // Check for Connected State and Throw an exception if not.
        void enforceConnected() throw ( ConnectorException );

        /**
         * Sends a oneway message.
         * @param command The message to send.
         * @throws ConnectorException if not currently connected, or
         * if the operation fails for any reason.
         */
        void oneway( transport::Command* command ) throw (ConnectorException);

        /**
         * Sends a synchronous request and returns the response from the broker.
         * Converts any error responses into an exception.
         * @param command The request command.
         * @returns The response sent from the broker.
         * @throws ConnectorException thrown if an error response was received
         * from the broker, or if any other error occurred.
         */
        transport::Response* syncRequest( transport::Command* command )
            throw (ConnectorException);

        /**
         * Sends a message to the broker to dispose of the given resource.
         * @param objectId The ID of the resource to be released.
         * @throw ConnectorException if any problems occur from sending
         * the message.
         */
        void disposeOf( commands::DataStructure* objectId )
            throw ( ConnectorException );

        /**
         * Send the Destination Creation Request to the Broker, alerting it
         * that we've created a new Temporary Destination.
         * @param tempDestination - The new Temporary Destination
         */
        void createTemporaryDestination(
            commands::ActiveMQTempDestination* tempDestination )
                throw ( ConnectorException );

        /**
         * Send the Destination Destruction Request to the Broker, alerting
         * it that we've removed an existing Temporary Destination.
         * @param tempDestination - The Temporary Destination to remove
         */
        void destroyTemporaryDestination(
            commands::ActiveMQTempDestination* tempDestination )
                throw ( ConnectorException );

        /**
         * Creates a new Temporary Destination name using the connection id
         * and a rolling count.
         * @returns a unique Temporary Destination name
         */
        std::string createTemporaryDestinationName()
            throw ( ConnectorException );

        /**
         * Creates a commands::ConsumerInfo object.  Used for both standard
         * and durable consumers.
         * @param destination The destination on which to create the consumer
         * @param session the parent session context.
         */
        commands::ConsumerInfo* createConsumerInfo(
            const cms::Destination* destination,
            connector::SessionInfo* session ) throw ( ConnectorException );

        /**
         * Create a Transaction Id using the local context to create
         * the LocalTransactionId Command.
         * @returns a new TransactionId pointer, caller owns.
         */
        commands::TransactionId* createLocalTransactionId()
            throw ( ConnectorException );

        /**
         * Applies the destination options to the given consumer.
         */
        void applyDestinationOptions( commands::ConsumerInfo* info );

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_OPENWIRE_OPENWIRECONNECTOR_H_*/