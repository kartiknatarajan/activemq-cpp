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
#include "marshal/ProducerInfoMarshaller.hpp"

using namespace apache::activemq::client::marshal;

/*
 *  Marshalling code for Open Wire Format for ProducerInfo
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

ProducerInfoMarshaller::ProducerInfoMarshaller()
{
    // no-op
}

ProducerInfoMarshaller::~ProducerInfoMarshaller()
{
    // no-op
}



IDataStructure* ProducerInfoMarshaller::createObject() 
{
    return new ProducerInfo();
}

char ProducerInfoMarshaller::getDataStructureType() 
{
    return ProducerInfo.ID_ProducerInfo;
}

    /* 
     * Un-marshal an object instance from the data input stream
     */ 
void ProducerInfoMarshaller::unmarshal(ProtocolFormat& wireFormat, Object o, BinaryReader& dataIn, BooleanStream& bs) 
{
    base.unmarshal(wireFormat, o, dataIn, bs);

    ProducerInfo& info = (ProducerInfo&) o;
        info.setProducerId((org.apache.activemq.command.ProducerId) tightUnmarsalCachedObject(wireFormat, dataIn, bs));
        info.setDestination((org.apache.activemq.command.ActiveMQDestination) tightUnmarsalCachedObject(wireFormat, dataIn, bs));

        if (bs.readBoolean()) {
            short size = dataIn.readShort();
            org.apache.activemq.command.BrokerId value[] = new org.apache.activemq.command.BrokerId[size];
            for( int i=0; i < size; i++ ) {
                value[i] = (org.apache.activemq.command.BrokerId) tightUnmarsalNestedObject(wireFormat,dataIn, bs);
            }
            info.setBrokerPath(value);
        }
        else {
            info.setBrokerPath(null);
        }

}


/*
 * Write the booleans that this object uses to a BooleanStream
 */
int ProducerInfoMarshaller::marshal1(ProtocolFormat& wireFormat, Object& o, BooleanStream& bs) {
    ProducerInfo& info = (ProducerInfo&) o;

    int rc = base.marshal1(wireFormat, info, bs);
    rc += marshal1CachedObject(wireFormat, info.getProducerId(), bs);
    rc += marshal1CachedObject(wireFormat, info.getDestination(), bs);
    rc += marshalObjectArray(wireFormat, info.getBrokerPath(), bs);

    return rc + 0;
}

/* 
 * Write a object instance to data output stream
 */
void ProducerInfoMarshaller::marshal2(ProtocolFormat& wireFormat, Object& o, BinaryWriter& dataOut, BooleanStream& bs) {
    base.marshal2(wireFormat, o, dataOut, bs);

    ProducerInfo& info = (ProducerInfo&) o;
    marshal2CachedObject(wireFormat, info.getProducerId(), dataOut, bs);
    marshal2CachedObject(wireFormat, info.getDestination(), dataOut, bs);
    marshalObjectArray(wireFormat, info.getBrokerPath(), dataOut, bs);

}