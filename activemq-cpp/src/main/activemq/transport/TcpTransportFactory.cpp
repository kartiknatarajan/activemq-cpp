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
 
#include "TcpTransportFactory.h"

#include <activemq/transport/TcpTransport.h>
#include <activemq/transport/ResponseCorrelator.h>
#include <activemq/transport/LoggingTransport.h>

using namespace activemq;
using namespace activemq::transport;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
TransportFactory& TcpTransportFactory::getInstance(void)
{
    // Create the one and only instance of the registrar
    static TransportFactoryMapRegistrar registrar(
        "tcp", new TcpTransportFactory() );
        
    return registrar.getFactory();
}

////////////////////////////////////////////////////////////////////////////////
Transport* TcpTransportFactory::createTransport( 
    const activemq::util::Properties& properties )
        throw ( ActiveMQException )
{
    try
    {
        TransportFactory* factory = 
            TransportFactoryMap::getInstance().lookup( "io" );
    
        if( factory == NULL ){
            throw ActiveMQException( 
                __FILE__, __LINE__, 
                "TcpTransport::createTransport - "
                "unknown transport factory");
        }
    
        Transport* transport = new TcpTransport( 
            properties, factory->createTransport( properties ) );
        
        // Create a response correlator.  This will wrap around our 
        // transport and manage its lifecycle - we don't need the 
        // internal transport anymore, so we can reuse its pointer.
        transport = new ResponseCorrelator( transport );
        
        // If command tracing was enabled, wrap the transport with a logging transport.
        if( properties.getProperty( "commandTracingEnabled", "false" ) == "true" ) {
            transport = new LoggingTransport( transport );
        }
        
        return transport;
    }
    AMQ_CATCH_RETHROW( ActiveMQException )
    AMQ_CATCHALL_THROW( ActiveMQException )
}