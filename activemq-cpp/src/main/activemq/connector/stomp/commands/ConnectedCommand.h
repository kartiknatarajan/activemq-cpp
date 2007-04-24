/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ACTIVEMQ_COMMAND_STOMP_COMMANDS_CONNECTEDCOMMAND_H_
#define ACTIVEMQ_COMMAND_STOMP_COMMANDS_CONNECTEDCOMMAND_H_
 
#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Response.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{
	
    /**
     * The stomp command returned from the broker indicating
     * a connection has been established.
     */
    class ConnectedCommand : public AbstractCommand< transport::Response >
    {
    public:
   
        ConnectedCommand() :
            AbstractCommand< transport::Response >() {
                initialize( getFrame() );
        }

        ConnectedCommand( StompFrame* frame ) : 
            AbstractCommand< transport::Response >( frame ) {
                validate( getFrame() );
        }

        virtual ~ConnectedCommand() {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new ConnectedCommand( getFrame().clone() );
        }

        /**
         * Sets the Correlation Id if this Command
         * @param corrId Id
         */
        virtual void setCorrelationId( int corrId ) {
            setPropertyValue(
                CommandConstants::toString( 
                    CommandConstants::HEADER_RESPONSEID),
                 util::Integer::toString( corrId ) );
        }
        
        /**
         * Get the Session Id
         * @return the mew Sessoin Id String
         */      
        virtual const char* getSessionId() const {
            return getPropertyValue( 
                CommandConstants::toString( 
                    CommandConstants::HEADER_SESSIONID) );
        }
      
        /**
         * Set the Session Id
         * @param session string containing the session id
         */
        virtual void setSessionId( const std::string& session ) {
            setPropertyValue( 
                CommandConstants::toString( 
                    CommandConstants::HEADER_SESSIONID),
                session );
        }

    protected:
    
        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame )
        {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::CONNECTED ) );
        }

        /**
         * Inheritors are required to override this method to validate 
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const
        {
            if(frame.getCommand() == 
               CommandConstants::toString( CommandConstants::CONNECTED ) )
            {
                return true;
            }

            return false;
        }

    };

}}}}

#endif /*ACTIVEMQ_COMMAND_STOMP_COMMANDS_CONNECTEDCOMMAND_H_*/