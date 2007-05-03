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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQTRANSACTION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQTRANSACTION_H_

#include <map>
#include <list>

#include <cms/Message.h>
#include <cms/CMSException.h>

#include <activemq/concurrent/Mutex.h>
#include <activemq/connector/TransactionInfo.h>
#include <activemq/exceptions/InvalidStateException.h>
#include <activemq/exceptions/IllegalArgumentException.h>
#include <activemq/util/Properties.h>

namespace activemq{
namespace core{

    class ActiveMQConnection;
    class ActiveMQSession;
    class ActiveMQMessage;
    class ActiveMQConsumer;

    /**
     * Transaction Management class, hold messages that are to be redelivered
     * upon a request to rollback.  The Tranasction represents an always
     * running transaction, when it is committed or rolled back it silently
     * creates a new transaction for the next set of messages.  The only
     * way to permanently end this tranaction is to delete it.
     *
     * Configuration options
     *
     * transaction.maxRedeliveryCount
     *   Max number of times a message can be redelivered, if the session is
     *   rolled back more than this many time, the message is dropped.
     */
    class ActiveMQTransaction : public connector::TransactionInfo
    {
    private:

        // List type for holding messages
        typedef std::list< ActiveMQMessage* > MessageList;

        // Mapping of MessageListener Ids to Lists of Messages that are
        // redelivered on a Rollback
        typedef std::map< ActiveMQConsumer*, MessageList > RollbackMap;

    private:

        // Connection this Transaction is associated with
        ActiveMQConnection* connection;

        // Session this Transaction is associated with
        ActiveMQSession* session;

        // Transaction Info for the current Transaction
        connector::TransactionInfo* transactionInfo;

        // Map of ActiveMQMessageConsumer to Messages to Rollback
        RollbackMap rollbackMap;

        // Lock object to protect the rollback Map
        concurrent::Mutex rollbackLock;

        // Max number of redeliveries before we quit
        int maxRedeliveries;

        // Mutex that is signaled when all tasks complete.
        concurrent::Mutex tasksDone;

    public:

        /**
         * Constructor
         * @param connection - Connection to the Broker
         * @param session - the session that contains this transaction
         * @param properties - configuratoin parameters for this object
         */
        ActiveMQTransaction( ActiveMQConnection* connection,
                             ActiveMQSession* session,
                             const util::Properties& properties );

        virtual ~ActiveMQTransaction();

        /**
         * Adds the Message as a part of the Transaction for the specified
         * ActiveMQConsumer.
         * @param message - Message to Transact
         * @param consumer - Listener to redeliver to on Rollback
         */
        virtual void addToTransaction( ActiveMQMessage* message,
                                       ActiveMQConsumer* consumer );

        /**
         * Removes the ActiveMQConsumer and all of its transacted
         * messages from the Transaction, this is usually only done when
         * an ActiveMQConsumer is destroyed.
         * @param listener - consumer who is to be removed.
         */
        virtual void removeFromTransaction( ActiveMQConsumer* listener );

        /**
         * Removes the ActiveMQConsumer and all of its transacted
         * messages from the Transaction, this is usually only done when
         * an ActiveMQConsumer is destroyed.
         * @param listener - consumer who is to be removed.
         */
        virtual void removeFromTransaction( long long consumerId );

        /**
         * Commit the current Transaction
         * @throw CMSException
         */
        virtual void commit() throw ( exceptions::ActiveMQException );

        /**
         * Rollback the current Transaction
         * @throw CMSException
         */
        virtual void rollback() throw ( exceptions::ActiveMQException );

        /**
         * Get the Transaction Information object for the current
         * Transaction, returns NULL if no transaction is running
         * @return TransactionInfo
         */
        virtual connector::TransactionInfo* getTransactionInfo(void) const {
            return transactionInfo;
        }

    public:   // TransactionInfo Interface

        /**
         * Gets the Transction Id
         * @return integral value of Id
         */
        virtual long long getTransactionId() const {
            return transactionInfo->getTransactionId();
        }

        /**
         * Sets the Transction Id
         * @param id - integral value of Id
         */
        virtual void setTransactionId( long long id ) {
            transactionInfo->setTransactionId( id );
        }

        /**
         * Gets the Session Info that this transaction is attached too
         * @return SessionnInfo pointer
         */
        virtual const connector::SessionInfo* getSessionInfo(void) const {
            return transactionInfo->getSessionInfo();
        }

        /**
         * Gets the Session Info that this transaction is attached too
         * @param session - SessionnInfo pointer
         */
        virtual void setSessionInfo( const connector::SessionInfo* session ) {
            transactionInfo->setSessionInfo( session );
        }

    protected:

        /**
         * Clean out all Messages from the Rollback Map, deleting the
         * messages as it goes.  Destroys the Transaction Info object as
         * well.
         * @throw ActiveMQException
         */
        virtual void clearTransaction();

        /**
         * Redelivers each message that is in the Message List to the specified
         * consumer, throwing messages away as they hit their max redilviery 
         * count.
         * @param consumer - the ActiveMQConsumer to redeliver to
         * @param messages - the list of messages that should be sent.
         * @throws ActiveMQException if an error occurs.
         */
        virtual void redeliverMessages( ActiveMQConsumer* consumer,
                                        MessageList& messages ) 
                                            throw ( exceptions::ActiveMQException );
    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQTRANSACTION_H_*/