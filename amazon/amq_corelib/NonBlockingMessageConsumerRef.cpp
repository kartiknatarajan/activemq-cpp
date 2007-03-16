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

#include "NonBlockingMessageConsumerRef.h"
#include "NonBlockingMessageConsumer.h"
#include "CoreLib.h"
#include "Exception.h"
#include "RCSID.h"

using namespace ActiveMQ;
using std::auto_ptr;

RCSID(NonBlockingMessageConsumerRef, "$Id$");

NonBlockingMessageConsumerRef::NonBlockingMessageConsumerRef()
    : MessageConsumerRef(),
      cons_(NULL)
  {}

NonBlockingMessageConsumerRef::NonBlockingMessageConsumerRef
    (CoreLib *a, NonBlockingMessageConsumer *q) :
    MessageConsumerRef(a), cons_(q) {
    if (isValid())
        owner_->registerRef(this);
}

NonBlockingMessageConsumerRef::NonBlockingMessageConsumerRef
(const NonBlockingMessageConsumerRef& oth)
    : MessageConsumerRef(oth), cons_(oth.cons_)
{
    if (isValid())
        owner_->registerRef(this);
}

NonBlockingMessageConsumerRef &
NonBlockingMessageConsumerRef::operator=
    (const NonBlockingMessageConsumerRef& oth) {
    if (this == &oth)
        return *this;
    cons_ = oth.cons_;
    MessageConsumerRef::operator=(oth);
    return *this;
}

NonBlockingMessageConsumerRef::~NonBlockingMessageConsumerRef() {
    if (isValid())
        owner_->deregisterRef(this);
}

int
NonBlockingMessageConsumerRef::getNumReadyMessages() const {
    if (!isValid())
        throw Exception("getNumReadyMessages called on invalid reference!");
    return cons_->getNumReadyMessages();
}

auto_ptr<Message>
NonBlockingMessageConsumerRef::receive() {
    if (!isValid())
        throw Exception("receive called on invalid reference!");
    return cons_->receive();
}

int
NonBlockingMessageConsumerRef::getEventFD() const {
    if (!isValid())
        throw Exception("getEventFD called on invalid reference!");
    return cons_->getEventFD();
}

bool
NonBlockingMessageConsumerRef::isValid() const {
    return owner_ != NULL && cons_ != NULL;
}

MessageConsumer *
NonBlockingMessageConsumerRef::getConsumer() const {
    return cons_;
}