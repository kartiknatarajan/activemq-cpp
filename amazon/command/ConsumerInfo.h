/*
  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at
  
  http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing,
  software distributed under the License is distributed on an
  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, either express or implied.  See the License for the
  specific language governing permissions and limitations
  under the License.
*/

#ifndef ConsumerInfo_h_
#define ConsumerInfo_h_

#include <string>
#include <vector>
#include <exception>
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

#include "command/BaseCommand.h"    
#include "ConsumerId.h"
#include "ActiveMQDestination.h"
#include "BrokerId.h"

namespace ActiveMQ {
  namespace Command {

    /*
     *
     *  Marshalling code for Open Wire Format for ConsumerInfo
     *
     *
     *  NOTE!: This file is autogenerated - do not modify!
     *         if you need to make a change, please see the Groovy scripts in the
     *         activemq-core module
     *
     */
    class ConsumerInfo : public BaseCommand
    {
      private:
        boost::shared_ptr<const ConsumerId> consumerId_;
        bool browser_;
        boost::shared_ptr<const ActiveMQDestination> destination_;
        int prefetchSize_;
        int maximumPendingMessageLimit_;
        bool dispatchAsync_;
        std::string selector_;
        std::string subcriptionName_;
        bool noLocal_;
        bool exclusive_;
        bool retroactive_;
        uint8_t priority_;
        std::vector<boost::shared_ptr<const BrokerId> > brokerPath_;
        bool networkSubscription_;
        bool optimizedAcknowledge_;
        bool noRangeAcks_;

      public:
        const static int TYPE = 5;
    
      public:
        ConsumerInfo();
        virtual ~ConsumerInfo();
    
        virtual bool isMarshalAware() const;
        virtual int getCommandType() const;

        virtual boost::shared_ptr<const ConsumerId> getConsumerId() const;
        virtual void setConsumerId(const boost::shared_ptr<ConsumerId>& consumerId);

        virtual bool isBrowser() const;
        virtual void setBrowser(bool browser);

        virtual boost::shared_ptr<const ActiveMQDestination> getDestination() const;
        virtual void setDestination(const boost::shared_ptr<ActiveMQDestination>& destination);

        virtual int getPrefetchSize() const;
        virtual void setPrefetchSize(int prefetchSize);

        virtual int getMaximumPendingMessageLimit() const;
        virtual void setMaximumPendingMessageLimit(int maximumPendingMessageLimit);

        virtual bool isDispatchAsync() const;
        virtual void setDispatchAsync(bool dispatchAsync);

        virtual const std::string& getSelector() const;
        virtual void setSelector(const std::string& selector);

        virtual const std::string& getSubcriptionName() const;
        virtual void setSubcriptionName(const std::string& subcriptionName);

        virtual bool isNoLocal() const;
        virtual void setNoLocal(bool noLocal);

        virtual bool isExclusive() const;
        virtual void setExclusive(bool exclusive);

        virtual bool isRetroactive() const;
        virtual void setRetroactive(bool retroactive);

        virtual uint8_t getPriority() const;
        virtual void setPriority(uint8_t priority);

        virtual const std::vector<boost::shared_ptr<const BrokerId> >& getBrokerPath() const;
        virtual void setBrokerPath(const std::vector<boost::shared_ptr<const BrokerId> >& brokerPath);

        virtual bool isNetworkSubscription() const;
        virtual void setNetworkSubscription(bool networkSubscription);

        virtual bool isOptimizedAcknowledge() const;
        virtual void setOptimizedAcknowledge(bool optimizedAcknowledge);

        virtual bool isNoRangeAcks() const;
        virtual void setNoRangeAcks(bool noRangeAcks);
    };
  }
}

#endif /*ConsumerInfo_h_*/