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

#include <activemq/connector/openwire/marshal/v2/ConnectionErrorMarshaller.h>

#include <activemq/connector/openwire/commands/ConnectionError.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::io;
using namespace activemq::connector;
using namespace activemq::connector::openwire;
using namespace activemq::connector::openwire::commands;
using namespace activemq::connector::openwire::marshal;
using namespace activemq::connector::openwire::utils;
using namespace activemq::connector::openwire::marshal::v2;

///////////////////////////////////////////////////////////////////////////////
DataStructure* ConnectionErrorMarshaller::createObject() const {
    return new ConnectionError();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char ConnectionErrorMarshaller::getDataStructureType() const {
    return ConnectionError::ID_CONNECTIONERROR;
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionErrorMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) throw( io::IOException ) {

    try {

        BaseCommandMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

        ConnectionError* info =
            dynamic_cast<ConnectionError*>( dataStructure );
        info->setException( dynamic_cast< BrokerError* >(
            tightUnmarshalBrokerError( wireFormat, dataIn, bs ) ) );
        info->setConnectionId( dynamic_cast< ConnectionId* >(
            tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) );
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
int ConnectionErrorMarshaller::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) throw( io::IOException ) {

    try {

        ConnectionError* info =
            dynamic_cast<ConnectionError*>( dataStructure );

        int rc = BaseCommandMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
        rc += tightMarshalBrokerError1( wireFormat, info->getException(), bs );
        rc += tightMarshalNestedObject1( wireFormat, info->getConnectionId(), bs );

        return rc + 0;
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionErrorMarshaller::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) throw( io::IOException ) {

    try {

        BaseCommandMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

        ConnectionError* info =
            dynamic_cast<ConnectionError*>( dataStructure );
        tightMarshalBrokerError2( wireFormat, info->getException(), dataOut, bs );
        tightMarshalNestedObject2( wireFormat, info->getConnectionId(), dataOut, bs );
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionErrorMarshaller::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) throw( io::IOException ) {

    try {

        BaseCommandMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
        ConnectionError* info =
            dynamic_cast<ConnectionError*>( dataStructure );
        info->setException( dynamic_cast< BrokerError* >(
            looseUnmarshalBrokerError( wireFormat, dataIn ) ) );
        info->setConnectionId( dynamic_cast< ConnectionId* >( 
            looseUnmarshalNestedObject( wireFormat, dataIn ) ) );
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionErrorMarshaller::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) throw( io::IOException ) {

    try {

        ConnectionError* info =
            dynamic_cast<ConnectionError*>( dataStructure );
         BaseCommandMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

        looseMarshalBrokerError( wireFormat, info->getException(), dataOut );
        looseMarshalNestedObject( wireFormat, info->getConnectionId(), dataOut );
    }
    AMQ_CATCH_RETHROW( io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, io::IOException )
    AMQ_CATCHALL_THROW( io::IOException )
}
