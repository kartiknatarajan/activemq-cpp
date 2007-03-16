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

#include "command/DiscoveryEvent.h"

using namespace ActiveMQ::Command;

/*
 *
 *  Marshalling code for Open Wire Format for DiscoveryEvent
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Groovy scripts in the
 *         activemq-core module
 *
 */
bool
DiscoveryEvent::isMarshalAware() const
{
    return false;
}

DiscoveryEvent::DiscoveryEvent()
{
    serviceName_ = "";
    brokerName_ = "";
}

int
DiscoveryEvent::getCommandType() const
{
    return TYPE;
}

DiscoveryEvent::~DiscoveryEvent()
{
}

const std::string& DiscoveryEvent::getServiceName() const
{
    return serviceName_;
}

void DiscoveryEvent::setServiceName(const std::string& serviceName)
{
    serviceName_ = serviceName;
}

const std::string& DiscoveryEvent::getBrokerName() const
{
    return brokerName_;
}

void DiscoveryEvent::setBrokerName(const std::string& brokerName)
{
    brokerName_ = brokerName;
}