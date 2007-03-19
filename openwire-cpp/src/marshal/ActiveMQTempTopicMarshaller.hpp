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
#ifndef ActiveMQTempTopicMarshaller_hpp_
#define ActiveMQTempTopicMarshaller_hpp_

#include <string>

#include "command/IDataStructure.hpp"

/* we could cut this down  - for now include all possible headers */
#include "command/BrokerId.hpp"
#include "command/ConnectionId.hpp"
#include "command/ConsumerId.hpp"
#include "command/ProducerId.hpp"
#include "command/SessionId.hpp"

#include "io/BinaryReader.hpp"
#include "io/BinaryWriter.hpp"

#include "command/ActiveMQTempDestinationMarshaller.hpp"
#include "util/ifr/p.hpp"

#include "protocol/ProtocolFormat.hpp"

namespace apache
{
  namespace activemq
  {
    namespace client
    {
      namespace marshal
      {
        using namespace ifr ;
        using namespace apache::activemq::client::command;
        using namespace apache::activemq::client::io;
        using namespace apache::activemq::client::protocol;

/*
 *
 */
class ActiveMQTempTopicMarshaller : public ActiveMQTempDestinationMarshaller
{
public:
    ActiveMQTempTopicMarshaller() ;
    virtual ~ActiveMQTempTopicMarshaller() ;

    virtual IDataStructure* createCommand() ;
    virtual char getDataStructureType() ;
    
    virtual void unmarshal(ProtocolFormat& wireFormat, Object o, BinaryReader& dataIn, BooleanStream& bs) ;
    virtual int marshal1(ProtocolFormat& wireFormat, Object& o, BooleanStream& bs) ;
    virtual void marshal2(ProtocolFormat& wireFormat, Object& o, BinaryWriter& dataOut, BooleanStream& bs) ;
} ;

/* namespace */
     }
    }
  }
}
#endif /*ActiveMQTempTopicMarshaller_hpp_*/