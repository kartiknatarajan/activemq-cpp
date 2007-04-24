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

#ifndef _ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORT_H_
#define _ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORT_H_

#include <activemq/transport/TransportFilter.h>
#include <activemq/util/Queue.h>
#include <activemq/concurrent/Runnable.h>

namespace activemq{
namespace transport{
namespace filters{

    class AsyncSendTransport : public TransportFilter,
                               public concurrent::Runnable {
    private:

        /**
         * Thread to send messages in when oneway is called.
         */
        concurrent::Thread* asyncThread;

        /**
         * Outgoing Message Queue
         */
        util::Queue<Command*> msgQueue;

        /**
         * boolean indicating that this transport is closed
         */
        bool closed;

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         * @param own - true if this filter owns the next and should delete it
         */
        AsyncSendTransport( Transport* next, bool own = true );

        virtual ~AsyncSendTransport();

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( Command* command )
            throw( CommandIOException, exceptions::UnsupportedOperationException );

        /**
         * Starts this transport object and creates the thread for
         * polling on the input stream for commands.  If this object
         * has been closed, throws an exception.  Before calling start,
         * the caller must set the IO streams and the reader and writer
         * objects.
         * @throws CMSException if an error occurs or if this transport
         * has already been closed.
         */
        virtual void start() throw( cms::CMSException );

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException );

    protected:

        /**
         * Run method that is called from the Thread class when this object
         * is registered with a Thread and started.  This function reads from
         * the outgoing message queue and dispatches calls to the connector that
         * is registered with this class.
         */
        virtual void run();

        /**
         * Starts the message processing thread to receive messages
         * asynchronously.  This thread is started when setMessageListener
         * is invoked, which means that the caller is choosing to use this
         * consumer asynchronously instead of synchronously (receive).
         */
        void startThread() throw ( exceptions::ActiveMQException );

        /**
         * Stops the asynchronous message processing thread if it's started.
         */
        void stopThread() throw ( exceptions::ActiveMQException );

        /**
         * Purges all messages currently in the queue.  This can be as a
         * result of a rollback, or of the consumer being shutdown.
         */
        virtual void purgeMessages() throw ( exceptions::ActiveMQException );

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FILTERS_ASYNCSENDTRANSPORT_H_*/