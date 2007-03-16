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
#include "marshal/DataArrayResponseMarshaller.h"
#include "command/DataArrayResponse.h"
#include "boost/shared_ptr.hpp"

using namespace ActiveMQ::Marshalling;
using namespace ActiveMQ::Command;
using namespace ActiveMQ::IO;
using std::auto_ptr;
using boost::shared_ptr;

/*
 *  Marshalling code for Open Wire Format for DataArrayResponse
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

DataArrayResponseMarshaller::DataArrayResponseMarshaller()
{
    // no-op
}

DataArrayResponseMarshaller::~DataArrayResponseMarshaller()
{
    // no-op
}

auto_ptr<IDataStructure> DataArrayResponseMarshaller::createCommand() 
{
    return auto_ptr<IDataStructure>(new DataArrayResponse());
}

char DataArrayResponseMarshaller::getDataStructureType() 
{
    return DataArrayResponse::TYPE;
}

/* 
 * Un-marshal an object instance from the data input stream
 */ 
void
DataArrayResponseMarshaller::unmarshal(ProtocolFormat& wireFormat, IDataStructure& o, BinaryReader& dataIn, BooleanStream& bs) 
{
    ResponseMarshaller::unmarshal(wireFormat, o, dataIn, bs);

    DataArrayResponse& info = (DataArrayResponse&) o;

    if (bs.readBoolean()) {
        short size = dataIn.readShort();
        std::vector<shared_ptr<const IDataStructure> > value;
        for( int i=0; i < size; i++ )
            value.push_back(shared_ptr<IDataStructure>(static_cast<IDataStructure*>(unmarshalNestedObject(wireFormat,dataIn, bs).release())));
        info.setData( value );
    }

}

/*
 * Write the booleans that this object uses to a BooleanStream
 */
size_t
DataArrayResponseMarshaller::marshal1(ProtocolFormat& wireFormat, const IDataStructure& o, BooleanStream& bs) {
    DataArrayResponse& info = (DataArrayResponse&) o;

    int rc = ResponseMarshaller::marshal1(wireFormat, info, bs);
    rc += marshal1ObjectArray(wireFormat, info.getData(), bs);

    return rc + 0;
}

/* 
 * Write a object instance to data output stream
 */
void
DataArrayResponseMarshaller::marshal2(ProtocolFormat& wireFormat, const IDataStructure& o, BinaryWriter& dataOut, BooleanStream& bs) {
    // }

    ResponseMarshaller::marshal2(wireFormat, o, dataOut, bs);

    DataArrayResponse& info = (DataArrayResponse&) o;
    marshal2ObjectArray(wireFormat, info.getData(), dataOut, bs);
}