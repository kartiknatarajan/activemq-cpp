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

#include <decaf/util/Date.h>
#include <decaf/util/Config.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h>
#endif

#include <decaf/lang/exceptions/UnsupportedOperationException.h>

using namespace stl;
using namespace decaf;
using namespace decaf::util;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
long long Date::getCurrentTimeMilliseconds(){

#if defined (HAVE_GETTIMEOFDAY)
    timeval tv;
    gettimeofday (&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
#elif defined (HAVE_TIME)
    return time (NULL) * 1000LL;
#elif defined (HAVE_FTIME)
    struct timeb t;
    ftime (&t);
    return (t.time * 1000LL) + t.millitm;
#else

    // This platform doesn't support any of the standard time methods
    // ... should never get here.
    #error "No current time function available on the local platform";

#endif
}
