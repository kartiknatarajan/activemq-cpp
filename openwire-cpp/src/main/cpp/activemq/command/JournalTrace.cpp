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
#include "activemq/command/JournalTrace.hpp"

using namespace apache::activemq::command;

/*
 *
 *  Command and marshalling code for OpenWire format for JournalTrace
 *
 *
 *  NOTE!: This file is autogenerated - do not modify!
 *         if you need to make a change, please see the Groovy scripts in the
 *         activemq-core module
 *
 */
JournalTrace::JournalTrace()
{
    this->message = NULL ;
}

JournalTrace::~JournalTrace()
{
}

unsigned char JournalTrace::getDataStructureType()
{
    return JournalTrace::TYPE ; 
}

        
p<string> JournalTrace::getMessage()
{
    return message ;
}

void JournalTrace::setMessage(p<string> message)
{
    this->message = message ;
}

int JournalTrace::marshal(p<IMarshaller> marshaller, int mode, p<IOutputStream> ostream) throw (IOException)
{
    int size = 0 ;

    size += BaseDataStructure::marshal(marshaller, mode, ostream) ; 
    size += marshaller->marshalString(message, mode, ostream) ; 
    return size ;
}

void JournalTrace::unmarshal(p<IMarshaller> marshaller, int mode, p<IInputStream> istream) throw (IOException)
{
    BaseDataStructure::unmarshal(marshaller, mode, istream) ; 
    message = p_cast<string>(marshaller->unmarshalString(mode, istream)) ; 
}