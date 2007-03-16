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

#ifndef ControlCommand_h_
#define ControlCommand_h_

#include <string>
#include <vector>
#include <exception>
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

#include "command/BaseCommand.h"    

namespace ActiveMQ {
  namespace Command {

    /*
     *
     *  Marshalling code for Open Wire Format for ControlCommand
     *
     *
     *  NOTE!: This file is autogenerated - do not modify!
     *         if you need to make a change, please see the Groovy scripts in the
     *         activemq-core module
     *
     */
    class ControlCommand : public BaseCommand
    {
      private:
        std::string command_;

      public:
        const static int TYPE = 14;
    
      public:
        ControlCommand();
        virtual ~ControlCommand();
    
        virtual bool isMarshalAware() const;
        virtual int getCommandType() const;

        virtual const std::string& getCommand() const;
        virtual void setCommand(const std::string& command);
    };
  }
}

#endif /*ControlCommand_h_*/