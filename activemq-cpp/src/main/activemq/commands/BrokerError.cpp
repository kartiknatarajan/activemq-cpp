/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/BrokerError.h>
#include <activemq/state/CommandVisitor.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
BrokerError::BrokerError() : BaseCommand(), message(), exceptionClass(), stackTraceElements(), cause() {
}

////////////////////////////////////////////////////////////////////////////////
BrokerError::~BrokerError() {
    this->stackTraceElements.clear();
}

////////////////////////////////////////////////////////////////////////////////
void BrokerError::copyDataStructure( const DataStructure* src ) {

    const BrokerError* srcErr = dynamic_cast<const BrokerError*> (src);

    if (srcErr == NULL || src == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "BrokerError::copyCommand - src is NULL or invalid");
    }

    this->setMessage(srcErr->getMessage());
    this->setExceptionClass(srcErr->getExceptionClass());
    this->setStackTraceElements(srcErr->getStackTraceElements());
    this->setCause(srcErr->getCause());
}

////////////////////////////////////////////////////////////////////////////////
Pointer<commands::Command> BrokerError::visit( activemq::state::CommandVisitor* visitor ) {
    return visitor->processBrokerError(this);
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Exception BrokerError::createExceptionObject() {

    // TODO Would be nice to actually create an exception matching the broker reported
    // type as well as preserving the supplied stack trace and possible embedded cause.
    Exception theCause(__FILE__, __LINE__, this->getMessage().c_str());
    return theCause;
}
