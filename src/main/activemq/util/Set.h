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

#ifndef ACTIVEMQ_UTIL_SET_H_
#define ACTIVEMQ_UTIL_SET_H_

#include <set>
#include <vector>
#include <activemq/exceptions/NoSuchElementException.h>
#include <activemq/concurrent/Synchronizable.h>
#include <activemq/concurrent/Mutex.h>
#include <activemq/util/Iterator.h>

namespace activemq{
namespace util{

    /**
     * Map template that wraps around a std::map to provide
     * a more user-friendly interface and to provide common
     * functions that do not exist in std::map.
     */
    template <typename E> class Set : public concurrent::Synchronizable
    {
    private:

        std::set<E> values;
        concurrent::Mutex mutex;

    private:

        class SetIterator : public Iterator<E> {
        private:

            typename std::set<E>::iterator current;
            typename std::set<E>::iterator previous;
            typename std::set<E>* set;

        public:

            SetIterator( typename std::set<E>* set ) {
                this->current = set->begin();
                this->previous = set->end();
                this->set = set;
            }
            virtual ~SetIterator() {}

            virtual E next() throw( exceptions::NoSuchElementException ){
                if( this->current == set->end() ) {
                    throw exceptions::NoSuchElementException(
                        __FILE__, __LINE__,
                        "Set::Iterator::next - No more elements to return" );
                }

                this->previous = this->current;
                return *( this->current++ );
            }

            virtual bool hasNext() const {
                return ( this->current != set->end() );
            }

            virtual void remove() throw ( exceptions::IllegalStateException,
                                          exceptions::UnsupportedOperationException ){
                if( this->previous == set->end() ) {
                    throw exceptions::IllegalStateException(
                        __FILE__, __LINE__,
                        "Set::Iterator::remove - Invalid State to call remove" );
                }

                this->set->erase( this->previous );
                this->previous = this->set->end();
            }
        };

    public:

        /**
         * Default constructor - does nothing.
         */
        Set(){}

        /**
         * Copy constructor - copies the content of the given set into this
         * one.
         * @param source The source set.
         */
        Set( const Set& source ){
            copy( source );
        }

        virtual ~Set(){}

        /**
         * Returns an iterator for this collection.  The order of Iteration
         * is in no particular order other than the natural ording of the
         * elements in the Set class.
         * @returns Iterator<E> for this collection
         */
        Iterator<E>* iterator() {
            return new SetIterator( &values );
        }

        /**
         * Copies the content of the source set into this set.  Erases
         * all existing data in this st.
         * @param source The source object to copy from.
         */
        virtual void copy( const Set& source ) {
            // Add all of the entries to this map.
            values = source.values;
        }

        /**
         * Removes all values from this set.
         */
        virtual void clear() {
            values.clear();
        }

        /**
         * Indicates whether or this set contains the given value.
         * @param value The value to look up.
         * @return true if this set contains the value, otherwise false.
         */
        virtual bool contains( const E& value ) const {
            typename std::set<E>::const_iterator iter;
            iter = values.find( value );
            return iter != values.end();
        }

        /**
         * @return if the set contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const {
            return values.empty();
        }

        /**
         * @return The number of elements in this set.
         */
        virtual std::size_t size() const {
            return values.size();
        }

        /**
         * Adds the given value to the set.
         * @param value The value to add.
         */
        virtual void add( const E& value ) {
            values.insert( value );
        }

        /**
         * Removes the value from the set.
         * @param value The value to be removed.
         */
        virtual void remove( const E& value ) {
            values.erase( value );
        }

        /**
         * @return the all values in this set as a std::vector.
         */
        virtual std::vector<E> toArray() const {
            std::vector<E> valueArray( values.size() );

            typename std::set<E>::const_iterator iter;
            iter=values.begin();
            for( int ix=0; iter != values.end(); ++iter, ++ix ){
                valueArray[ix] = *iter;
            }

            return valueArray;
        }

    public:     // Methods from Synchronizable

        /**
         * Locks the object.
         * @throws ActiveMQException
         */
        virtual void lock() throw(exceptions::ActiveMQException) {
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws ActiveMQException
         */
        virtual void unlock() throw(exceptions::ActiveMQException) {
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void wait() throw(exceptions::ActiveMQException) {
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in millisecsonds to wait, or
         * WAIT_INIFINITE
         * @throws ActiveMQException
         */
        virtual void wait( unsigned long millisecs )
            throw(exceptions::ActiveMQException) {
            mutex.wait( millisecs );
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notify() throw( exceptions::ActiveMQException ) {
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notifyAll() throw( exceptions::ActiveMQException ) {
            mutex.notifyAll();
        }
    };

}}

#endif /*ACTIVEMQ_UTIL_SET_H_*/