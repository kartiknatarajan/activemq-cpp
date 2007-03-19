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
#include "marshal/XATransactionIdMarshaller.h"
#include "command/XATransactionId.h"
#include "boost/shared_ptr.hpp"

using namespace ActiveMQ::Marshalling;
using namespace ActiveMQ::Command;
using namespace ActiveMQ::IO;
using std::auto_ptr;
using boost::shared_ptr;

/*
 *  Marshalling code for Open Wire Format for XATransactionId
 *
 * NOTE!: This file is autogenerated - do not modify!
 *        if you need to make a change, please see the Groovy scripts in the
 *        activemq-core module
 */

XATransactionIdMarshaller::XATransactionIdMarshaller()
{
    // no-op
}

XATransactionIdMarshaller::~XATransactionIdMarshaller()
{
    // no-op
}

auto_ptr<IDataStructure> XATransactionIdMarshaller::createCommand() 
{
    return auto_ptr<IDataStructure>(new XATransactionId());
}

char XATransactionIdMarshaller::getDataStructureType() 
{
    return XATransactionId::TYPE;
}

/* 
 * Un-marshal an object instance from the data input stream
 */ 
void
XATransactionIdMarshaller::unmarshal(ProtocolFormat& wireFormat, IDataStructure& o, BinaryReader& dataIn, BooleanStream& bs) 
{
    TransactionIdMarshaller::unmarshal(wireFormat, o, dataIn, bs);

    XATransactionId& info = (XATransactionId&) o;
    info.setFormatId( dataIn.readInt() );
    info.setGlobalTransactionId( unmarshalByteSequence(wireFormat, dataIn, bs) );
    info.setBranchQualifier( unmarshalByteSequence(wireFormat, dataIn, bs) );

}

/*
 * Write the booleans that this object uses to a BooleanStream
 */
size_t
XATransactionIdMarshaller::marshal1(ProtocolFormat& wireFormat, const IDataStructure& o, BooleanStream& bs) {
    XATransactionId& info = (XATransactionId&) o;

    int rc = TransactionIdMarshaller::marshal1(wireFormat, info, bs);
        bs.writeBoolean(!(info.getGlobalTransactionId().empty()));
    rc += info.getGlobalTransactionId().empty() ? 0 : info.getGlobalTransactionId().size()+4;
    bs.writeBoolean(!(info.getBranchQualifier().empty()));
    rc += info.getBranchQualifier().empty() ? 0 : info.getBranchQualifier().size()+4;

    return rc + 4;
}

/* 
 * Write a object instance to data output stream
 */
void
XATransactionIdMarshaller::marshal2(ProtocolFormat& wireFormat, const IDataStructure& o, BinaryWriter& dataOut, BooleanStream& bs) {
    // }

    TransactionIdMarshaller::marshal2(wireFormat, o, dataOut, bs);

    XATransactionId& info = (XATransactionId&) o;
    dataOut.writeInt(info.getFormatId());
    if(bs.readBoolean()) {
       dataOut.writeInt(info.getGlobalTransactionId().size());
       dataOut.write(&((info.getGlobalTransactionId())[0]), info.getGlobalTransactionId().size());
    }
    if(bs.readBoolean()) {
       dataOut.writeInt(info.getBranchQualifier().size());
       dataOut.write(&((info.getBranchQualifier())[0]), info.getBranchQualifier().size());
    }
}