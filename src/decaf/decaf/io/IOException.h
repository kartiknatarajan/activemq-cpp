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
#ifndef _DECAF_IO_IOEXCEPTION_H
#define _DECAF_IO_IOEXCEPTION_H

#include <decaf/lang/Exception.h>

namespace decaf{
namespace io{

    /*
     * Signals that an I/O exception of some sort has occurred.
     */
    class IOException : public lang::Exception
    {
    public:

        /**
         * Default Constructor
         */
        IOException() throw() {}

        /**
         * Copy Constructor
         * @param ex the exception to copy
         */
        IOException( const lang::Exception& ex ) throw()
        : lang::Exception()
        {
            *(lang::Exception*)this = ex;
        }

        /**
         * Copy Constructor
         * @param ex the exception to copy, which is an instance of this type
         */
        IOException( const IOException& ex ) throw()
        : lang::Exception()
        {
            *(lang::Exception*)this = ex;
        }

        /**
         * Consturctor
         * @param file name of the file were the exception occured.
         * @param lineNumber line where the exception occured
         * @param msg the message that was generated
         */
        IOException( const char* file, const int lineNumber,
                     const char* msg, ... ) throw()
        : lang::Exception()
        {
            va_list vargs;
            va_start( vargs, msg );
            buildMessage( msg, vargs );

            // Set the first mark for this exception.
            setMark( file, lineNumber );
        }

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         */
        virtual IOException* clone() const{
            return new IOException( *this );
        }

        virtual ~IOException() throw() {}

    };

}}

#endif /*_DECAF_IO_IOEXCEPTION_H*/