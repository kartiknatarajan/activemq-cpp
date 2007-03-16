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

#include "netinet/in.h"
#include "marshal/NetworkBridgeFilterMarshaller.h"
#include "command/NetworkBridgeFilter.h"
#include "boost/shared_ptr.hpp"

using namespace ActiveMQ::Marshalling;
using namespace ActiveMQ::Command;
using namespace ActiveMQ::IO;
using std::auto_ptr;
using boost::shared_ptr;

/*
 *  Marshalling code for Open Wire Format for NetworkBridgeFilter
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

NetworkBridgeFilterMarshaller::NetworkBridgeFilterMarshaller()
{
    // no-op
}

NetworkBridgeFilterMarshaller::~NetworkBridgeFilterMarshaller()
{
    // no-op
}

auto_ptr<IDataStructure> NetworkBridgeFilterMarshaller::createCommand() 
{
    return auto_ptr<IDataStructure>(new NetworkBridgeFilter());
}

char NetworkBridgeFilterMarshaller::getDataStructureType() 
{
    return NetworkBridgeFilter::TYPE;
}

/* 
 * Un-marshal an object instance from the data input stream
 */ 
void
NetworkBridgeFilterMarshaller::unmarshal(ProtocolFormat& wireFormat, IDataStructure& o, BinaryReader& dataIn, BooleanStream& bs) 
{

    NetworkBridgeFilter& info = (NetworkBridgeFilter&) o;
    info.setNetworkTTL( dataIn.readInt() );
    info.setNetworkBrokerId( shared_ptr<BrokerId>(static_cast<BrokerId*>(unmarshalCachedObject(wireFormat, dataIn, bs).release())) );

}

/*
 * Write the booleans that this object uses to a BooleanStream
 */
size_t
NetworkBridgeFilterMarshaller::marshal1(ProtocolFormat& wireFormat, const IDataStructure& o, BooleanStream& bs) {
    NetworkBridgeFilter& info = (NetworkBridgeFilter&) o;

    int rc = 0;
        rc += marshal1CachedObject(wireFormat, info.getNetworkBrokerId(), bs);

    return rc + 4;
}

/* 
 * Write a object instance to data output stream
 */
void
NetworkBridgeFilterMarshaller::marshal2(ProtocolFormat& wireFormat, const IDataStructure& o, BinaryWriter& dataOut, BooleanStream& bs) {
    // }


    NetworkBridgeFilter& info = (NetworkBridgeFilter&) o;
    dataOut.writeInt(info.getNetworkTTL());
    marshal2CachedObject(wireFormat, info.getNetworkBrokerId(), dataOut, bs);
}