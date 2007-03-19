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

#include "command/SessionId.h"

using namespace ActiveMQ::Command;

/*
 *
 *  Marshalling code for Open Wire Format for SessionId
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Groovy scripts in the
 *         activemq-core module
 *
 */
bool
SessionId::isMarshalAware() const
{
    return false;
}

SessionId::SessionId()
{
    connectionId_ = "";
    value_ = 0;
}

int
SessionId::getCommandType() const
{
    return TYPE;
}

SessionId::~SessionId()
{
}

const std::string& SessionId::getConnectionId() const
{
    return connectionId_;
}

void SessionId::setConnectionId(const std::string& connectionId)
{
    connectionId_ = connectionId;
}

int64_t SessionId::getValue() const
{
    return value_;
}

void SessionId::setValue(int64_t value)
{
    value_ = value;
}