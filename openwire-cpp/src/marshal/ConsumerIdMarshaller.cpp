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
#include "marshal/ConsumerIdMarshaller.hpp"

using namespace apache::activemq::client::marshal;

/*
 *  Marshalling code for Open Wire Format for ConsumerId
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

ConsumerIdMarshaller::ConsumerIdMarshaller()
{
    // no-op
}

ConsumerIdMarshaller::~ConsumerIdMarshaller()
{
    // no-op
}



IDataStructure* ConsumerIdMarshaller::createObject() 
{
    return new ConsumerId();
}

char ConsumerIdMarshaller::getDataStructureType() 
{
    return ConsumerId.ID_ConsumerId;
}

    /* 
     * Un-marshal an object instance from the data input stream
     */ 
void ConsumerIdMarshaller::unmarshal(ProtocolFormat& wireFormat, Object o, BinaryReader& dataIn, BooleanStream& bs) 
{
    base.unmarshal(wireFormat, o, dataIn, bs);

    ConsumerId& info = (ConsumerId&) o;
        info.setConnectionId(tightUnmarshalString(dataIn, bs));
        info.setSessionId(tightUnmarshalLong(wireFormat, dataIn, bs));
        info.setValue(tightUnmarshalLong(wireFormat, dataIn, bs));

}


/*
 * Write the booleans that this object uses to a BooleanStream
 */
int ConsumerIdMarshaller::marshal1(ProtocolFormat& wireFormat, Object& o, BooleanStream& bs) {
    ConsumerId& info = (ConsumerId&) o;

    int rc = base.marshal1(wireFormat, info, bs);
    rc += writeString(info.getConnectionId(), bs);
    rc += marshal1Long(wireFormat, info.getSessionId(), bs);
    rc += marshal1Long(wireFormat, info.getValue(), bs);

    return rc + 0;
}

/* 
 * Write a object instance to data output stream
 */
void ConsumerIdMarshaller::marshal2(ProtocolFormat& wireFormat, Object& o, BinaryWriter& dataOut, BooleanStream& bs) {
    base.marshal2(wireFormat, o, dataOut, bs);

    ConsumerId& info = (ConsumerId&) o;
    writeString(info.getConnectionId(), dataOut, bs);
    marshal2Long(wireFormat, info.getSessionId(), dataOut, bs);
    marshal2Long(wireFormat, info.getValue(), dataOut, bs);

}