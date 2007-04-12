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

#ifndef _ACTIVEMQ_UTIL_URISUPPORT_H_
#define _ACTIVEMQ_UTIL_URISUPPORT_H_

#include <activemq/util/SimpleProperties.h>
#include <activemq/exceptions/IllegalArgumentException.h>

namespace activemq{
namespace util{

    class URISupport
    {
    public:
    
        virtual ~URISupport();
        
        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         * @param query. the query string to parse.
         * @returns SimpleProperties object with the parsed output.
         */
        static SimpleProperties parseQuery( std::string query )
            throw ( exceptions::IllegalArgumentException );
         
        /**
         * Parse the Query portion of a URI String and return a Simple
         * Properties object containing the parameter names as keys, and
         * the parameter values and values of the Properties.
         * @param query - the query string to parse.
         * @param properties - object pointer to get the parsed output.
         */
        static void parseQuery( std::string query,
                                util::Properties* properties )
            throw ( exceptions::IllegalArgumentException );

    };

}}

#endif /*_ACTIVEMQ_UTIL_URISUPPORT_H_*/