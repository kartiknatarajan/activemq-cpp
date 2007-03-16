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

#include "command/ConnectionInfo.h"

using namespace ActiveMQ::Command;

/*
 *
 *  Marshalling code for Open Wire Format for ConnectionInfo
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Groovy scripts in the
 *         activemq-core module
 *
 */
bool
ConnectionInfo::isMarshalAware() const
{
    return false;
}

ConnectionInfo::ConnectionInfo()
{
    clientId_ = "";
    password_ = "";
    userName_ = "";
    brokerMasterConnector_ = 0;
    manageable_ = 0;
}

int
ConnectionInfo::getCommandType() const
{
    return TYPE;
}

ConnectionInfo::~ConnectionInfo()
{
}

boost::shared_ptr<const ConnectionId> ConnectionInfo::getConnectionId() const
{
    return connectionId_;
}

void ConnectionInfo::setConnectionId(const boost::shared_ptr<ConnectionId>& connectionId)
{
    connectionId_ = connectionId;
}

const std::string& ConnectionInfo::getClientId() const
{
    return clientId_;
}

void ConnectionInfo::setClientId(const std::string& clientId)
{
    clientId_ = clientId;
}

const std::string& ConnectionInfo::getPassword() const
{
    return password_;
}

void ConnectionInfo::setPassword(const std::string& password)
{
    password_ = password;
}

const std::string& ConnectionInfo::getUserName() const
{
    return userName_;
}

void ConnectionInfo::setUserName(const std::string& userName)
{
    userName_ = userName;
}

const std::vector<boost::shared_ptr<const BrokerId> >& ConnectionInfo::getBrokerPath() const
{
    return brokerPath_;
}

void ConnectionInfo::setBrokerPath(const std::vector<boost::shared_ptr<const BrokerId> >& brokerPath)
{
    brokerPath_ = brokerPath;
}

bool ConnectionInfo::isBrokerMasterConnector() const
{
    return brokerMasterConnector_;
}

void ConnectionInfo::setBrokerMasterConnector(bool brokerMasterConnector)
{
    brokerMasterConnector_ = brokerMasterConnector;
}

bool ConnectionInfo::isManageable() const
{
    return manageable_;
}

void ConnectionInfo::setManageable(bool manageable)
{
    manageable_ = manageable;
}