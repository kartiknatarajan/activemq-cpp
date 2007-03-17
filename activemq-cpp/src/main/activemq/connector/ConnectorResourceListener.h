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

#ifndef _ACTIVEMQ_CONNECTOR_CONNECTORRESOURCELISTENER_H_
#define _ACTIVEMQ_CONNECTOR_CONNECTORRESOURCELISTENER_H_

#include <cms/CMSException.h>

namespace activemq{
namespace connector{

    class ConnectorResource;

    /**
     * Listener of events from a connector resource.
     */
    class ConnectorResourceListener {

    public:

        virtual ~ConnectorResourceListener() {}

        /**
         * When a Connector Resouce is closed it will notify any registered
         * Listeners of its close so that they can take the appropriate
         * action.
         * @param resource - The ConnectorResource that was closed.
         */
        virtual void onConnectorResourceClosed(
            const ConnectorResource* resource ) throw ( cms::CMSException ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CONNECTOR_CONNECTORRESOURCELISTENER_H_*/
