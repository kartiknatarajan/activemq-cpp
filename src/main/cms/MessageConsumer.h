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
 
#ifndef _CMS_MESSAGECONSUMER_H_
#define _CMS_MESSAGECONSUMER_H_

#include <cms/MessageListener.h>
#include <cms/Message.h>
#include <cms/Closeable.h>

namespace cms
{

    class MessageConsumer : public Closeable
    {
    public:

        virtual ~MessageConsumer() {}
      
        /**
         * Synchronously Receive a Message
         * @return new message
         * @throws CMSException
         */
        virtual Message* receive() throw ( CMSException ) = 0;

        /**
         * Synchronously Receive a Message, time out after defined interval.
         * Returns null if nothing read.
         * @return new message
         * @throws CMSException
         */
        virtual Message* receive( int millisecs ) throw ( CMSException ) = 0;

        /**
         * Receive a Message, does not wait if there isn't a new message
         * to read, returns NULL if nothing read.
         * @return new message
         * @throws CMSException
         */
        virtual Message* receiveNoWait() throw ( CMSException ) = 0;

        /**
         * Sets the MessageListener that this class will send notifs on
         * @param MessageListener interface pointer
         */
        virtual void setMessageListener( MessageListener* listener ) = 0;
      
        /**
         * Gets the MessageListener that this class will send notifs on
         * @param MessageListener interface pointer
         */
        virtual MessageListener* getMessageListener() const = 0;
      
        /**
         * Gets this message consumer's message selector expression.
         * @return This Consumer's selector expression or "".
         * @throws cms::CMSException
         */
        virtual std::string getMessageSelector() const 
            throw ( cms::CMSException ) = 0;
            
    };

}

#endif /*_CMS_MESSAGECONSUMER_H_*/