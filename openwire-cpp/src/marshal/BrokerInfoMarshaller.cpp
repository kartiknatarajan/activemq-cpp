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
#include "marshal/BrokerInfoMarshaller.hpp"

using namespace apache::activemq::client::marshal;

/*
 *  Marshalling code for Open Wire Format for BrokerInfo
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

BrokerInfoMarshaller::BrokerInfoMarshaller()
{
    // no-op
}

BrokerInfoMarshaller::~BrokerInfoMarshaller()
{
    // no-op
}



IDataStructure* BrokerInfoMarshaller::createObject() 
{
    return new BrokerInfo();
}

char BrokerInfoMarshaller::getDataStructureType() 
{
    return BrokerInfo.ID_BrokerInfo;
}

    /* 
     * Un-marshal an object instance from the data input stream
     */ 
void BrokerInfoMarshaller::unmarshal(ProtocolFormat& wireFormat, Object o, BinaryReader& dataIn, BooleanStream& bs) 
{
    base.unmarshal(wireFormat, o, dataIn, bs);

    BrokerInfo& info = (BrokerInfo&) o;
        info.setBrokerId((org.apache.activemq.command.BrokerId) tightUnmarsalCachedObject(wireFormat, dataIn, bs));
        info.setBrokerURL(tightUnmarshalString(dataIn, bs));

        if (bs.readBoolean()) {
            short size = dataIn.readShort();
            org.apache.activemq.command.BrokerInfo value[] = new org.apache.activemq.command.BrokerInfo[size];
            for( int i=0; i < size; i++ ) {
                value[i] = (org.apache.activemq.command.BrokerInfo) tightUnmarsalNestedObject(wireFormat,dataIn, bs);
            }
            info.setPeerBrokerInfos(value);
        }
        else {
            info.setPeerBrokerInfos(null);
        }
        info.setBrokerName(tightUnmarshalString(dataIn, bs));
        info.setSlaveBroker(bs.readBoolean());
        info.setMasterBroker(bs.readBoolean());
        info.setFaultTolerantConfiguration(bs.readBoolean());

}


/*
 * Write the booleans that this object uses to a BooleanStream
 */
int BrokerInfoMarshaller::marshal1(ProtocolFormat& wireFormat, Object& o, BooleanStream& bs) {
    BrokerInfo& info = (BrokerInfo&) o;

    int rc = base.marshal1(wireFormat, info, bs);
    rc += marshal1CachedObject(wireFormat, info.getBrokerId(), bs);
    rc += writeString(info.getBrokerURL(), bs);
    rc += marshalObjectArray(wireFormat, info.getPeerBrokerInfos(), bs);
    rc += writeString(info.getBrokerName(), bs);
    bs.writeBoolean(info.isSlaveBroker());
    bs.writeBoolean(info.isMasterBroker());
    bs.writeBoolean(info.isFaultTolerantConfiguration());

    return rc + 0;
}

/* 
 * Write a object instance to data output stream
 */
void BrokerInfoMarshaller::marshal2(ProtocolFormat& wireFormat, Object& o, BinaryWriter& dataOut, BooleanStream& bs) {
    base.marshal2(wireFormat, o, dataOut, bs);

    BrokerInfo& info = (BrokerInfo&) o;
    marshal2CachedObject(wireFormat, info.getBrokerId(), dataOut, bs);
    writeString(info.getBrokerURL(), dataOut, bs);
    marshalObjectArray(wireFormat, info.getPeerBrokerInfos(), dataOut, bs);
    writeString(info.getBrokerName(), dataOut, bs);
    bs.readBoolean();
    bs.readBoolean();
    bs.readBoolean();

}