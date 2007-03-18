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
#ifndef _ACTIVEMQ_CONCURRENT_POOLEDTHREAD_H_
#define _ACTIVEMQ_CONCURRENT_POOLEDTHREAD_H_

#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Runnable.h>
#include <activemq/concurrent/PooledThreadListener.h>
#include <activemq/logger/LoggerDefines.h>

#include <cms/Stoppable.h>
#include <cms/CMSException.h>

namespace activemq{
namespace concurrent{

    class ThreadPool;

    class PooledThread : public Thread, public cms::Stoppable
    {
    private:
   
        // Is this thread currently processing something
        bool busy;
      
        // Boolean flag indicating thread should stop
        bool done;
      
        // Listener for Task related events
        PooledThreadListener* listener;
      
        // The thread pool this Pooled Thread is Servicing
        ThreadPool* pool;

        // Logger Init
        LOGCMS_DECLARE(logger)
      
     public:
   
        /**
         * Constructor
         * @param pool the parant ThreadPool object
         */
        PooledThread( ThreadPool* pool );

        virtual ~PooledThread(void);

        /**
         * Run Method for this object waits for something to be
         * enqueued on the ThreadPool and then grabs it and calls 
         * its run method.
         */
        virtual void run(void);
      
        /**
         * Stops the Thread, thread will complete its task if currently
         * running one, and then die.  Does not block.
         * @throws CMSException
         */
        virtual void stop(void) throw ( cms::CMSException );
      
        /**
         * Checks to see if the thread is busy, if busy it means
         * that this thread has taken a task from the ThreadPool's
         * queue and is processing it.
         * @returns true if the Thread is busy
         */
        virtual bool isBusy(void) { return busy; }

        /**
         * Adds a listener to this <code>PooledThread</code> to be
         * notified when this thread starts and completes a task.
         * @param listener the listener to send notifications to.
         */
        virtual void setPooledThreadListener( PooledThreadListener* listener )
        {
            this->listener = listener;
        }

        /**
         * Removes a listener for this <code>PooledThread</code> to be
         * notified when this thread starts and completes a task.
         * @return a pointer to this thread's listener or NULL
         */
        virtual PooledThreadListener* getPooledThreadListener(void)
        {
            return this->listener;
        }
    };

}}

#endif /*_ACTIVEMQ_CONCURRENT_POOLEDTHREAD_H_*/