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

#ifndef _DECAF_LANG_THROWABLE_H_
#define _DECAF_LANG_THROWABLE_H_

// Includes
#include <string>
#include <vector>
#include <iostream>
#include <exception>

namespace cms{

    /**
     * This class represents an error that has occurred.
     */
    class Throwable : public std::exception {

    public:

        Throwable() throw() {}

        virtual ~Throwable() throw() {}

        /**
         * Gets the cause of the error.
         * @return string errors message
         */
        virtual std::string getMessage() const = 0;

        /**
         * Adds a file/line number to the stack trace.
         * @param file The name of the file calling this method (use __FILE__).
         * @param lineNumber The line number in the calling file (use __LINE__).
         */
        virtual void setMark( const char* file, const int lineNumber ) = 0;

        /**
         * Clones this exception.  This is useful for cases where you need
         * to preserve the type of the original exception as well as the message.
         * All subclasses should override.
         * @return Copy of this Exception object
         */
        virtual Throwable* clone() const = 0;

        /**
         * Provides the stack trace for every point where
         * this exception was caught, marked, and rethrown.
         * @return vector containing stack trace strings
         */
        virtual std::vector< std::pair< std::string, int> > getStackTrace() const = 0;

        /**
         * Prints the stack trace to std::err
         */
        virtual void printStackTrace() const = 0;

        /**
         * Prints the stack trace to the given output stream.
         * @param stream the target output stream.
         */
        virtual void printStackTrace( std::ostream& stream ) const = 0;

        /**
         * Gets the stack trace as one contiguous string.
         * @return string with formatted stack trace data
         */
        virtual std::string getStackTraceString() const = 0;

    };

}

#endif /*_DECAF_LANG_THROWABLE_H_*/