/*
 *
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
#include "marshal/JournalQueueAckMarshaller.hpp"

using namespace apache::activemq::client::marshal;

/*
 *  Marshalling code for Open Wire Format for JournalQueueAck
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

JournalQueueAckMarshaller::JournalQueueAckMarshaller()
{
    // no-op
}

JournalQueueAckMarshaller::~JournalQueueAckMarshaller()
{
    // no-op
}



IDataStructure* JournalQueueAckMarshaller::createObject() 
{
    return new JournalQueueAck();
}

char JournalQueueAckMarshaller::getDataStructureType() 
{
    return JournalQueueAck.ID_JournalQueueAck;
}

    /* 
     * Un-marshal an object instance from the data input stream
     */ 
void JournalQueueAckMarshaller::unmarshal(ProtocolFormat& wireFormat, Object o, BinaryReader& dataIn, BooleanStream& bs) 
{
    base.unmarshal(wireFormat, o, dataIn, bs);

    JournalQueueAck& info = (JournalQueueAck&) o;
        info.setDestination((org.apache.activemq.command.ActiveMQDestination) tightUnmarsalNestedObject(wireFormat, dataIn, bs));
        info.setMessageAck((org.apache.activemq.command.MessageAck) tightUnmarsalNestedObject(wireFormat, dataIn, bs));

}


/*
 * Write the booleans that this object uses to a BooleanStream
 */
int JournalQueueAckMarshaller::marshal1(ProtocolFormat& wireFormat, Object& o, BooleanStream& bs) {
    JournalQueueAck& info = (JournalQueueAck&) o;

    int rc = base.marshal1(wireFormat, info, bs);
    rc += marshal1NestedObject(wireFormat, info.getDestination(), bs);
    rc += marshal1NestedObject(wireFormat, info.getMessageAck(), bs);

    return rc + 0;
}

/* 
 * Write a object instance to data output stream
 */
void JournalQueueAckMarshaller::marshal2(ProtocolFormat& wireFormat, Object& o, BinaryWriter& dataOut, BooleanStream& bs) {
    base.marshal2(wireFormat, o, dataOut, bs);

    JournalQueueAck& info = (JournalQueueAck&) o;
    marshal2NestedObject(wireFormat, info.getDestination(), dataOut, bs);
    marshal2NestedObject(wireFormat, info.getMessageAck(), dataOut, bs);

}