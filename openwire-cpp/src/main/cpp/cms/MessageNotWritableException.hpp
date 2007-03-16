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
#ifndef Cms_MessageNotWritableException_hpp_
#define Cms_MessageNotWritableException_hpp_

#include "cms/CmsException.hpp"

namespace apache
{
  namespace cms
  {

/*
 * Signals that a message is in read-only mode.
 */
class MessageNotWritableException : public CmsException
{
public:
    MessageNotWritableException() : CmsException()
       { /* no-op */ } ;
    MessageNotWritableException(const char *msg) : CmsException(msg)
       { /* no-op */ } ;
} ;

/* namespace */
  }
}

#endif /*Cms_MessageNotWritableException_hpp_*/