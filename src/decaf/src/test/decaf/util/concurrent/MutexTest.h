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

#ifndef _DECAF_UTIL_CONCURRENT_MUTEXTEST_H_
#define _DECAF_UTIL_CONCURRENT_MUTEXTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Thread.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <time.h>

namespace decaf{
namespace util{
namespace concurrent{

    class MutexTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( MutexTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST( testWait );
        CPPUNIT_TEST( testTimedWait );
        CPPUNIT_TEST( testNotify );
        CPPUNIT_TEST( testNotifyAll );
        CPPUNIT_TEST( testRecursiveLock );
        CPPUNIT_TEST( testDoubleLock );
        CPPUNIT_TEST_SUITE_END();

    public:

        class MyThread
        :
            public lang::Thread,
            public Synchronizable{

        private:

            Mutex mutex;

        public:

            int value;
            MyThread(){ value = 0;}
            virtual ~MyThread(){}

            virtual void lock() throw(lang::Exception){
                mutex.lock();
            }
            virtual void unlock() throw(lang::Exception){
                mutex.unlock();
            }
            virtual void wait() throw(lang::Exception){
                mutex.wait();
            }
            virtual void wait(unsigned long millisecs) throw(lang::Exception){
                mutex.wait( millisecs );
            }
            virtual void notify() throw(lang::Exception){
                mutex.notify();
            }
            virtual void notifyAll() throw(lang::Exception){
                mutex.notifyAll();
            }

            virtual void run(){

                {
                    Lock lock (this);

                    value = value * 25;
                }
            }

        };

        class MyWaitingThread
        :
            public lang::Thread,
            public Synchronizable{

        private:

            Mutex mutex;

        public:

            int value;
            MyWaitingThread(){ value = 0;}
            virtual ~MyWaitingThread(){}
            virtual void lock() throw(lang::Exception){
                mutex.lock();
            }
            virtual void unlock() throw(lang::Exception){
                mutex.unlock();
            }
            virtual void wait() throw(lang::Exception){
                mutex.wait();
            }
            virtual void wait(unsigned long millisecs) throw(lang::Exception){
                mutex.wait( millisecs );
            }
            virtual void notify() throw(lang::Exception){
                mutex.notify();
            }
            virtual void notifyAll() throw(lang::Exception){
                mutex.notifyAll();
            }

            virtual void run(){

                try
                {
                    synchronized(this)
                    {
                        this->wait();

                        std::cout.flush();

                        value = value * 25;
                    }
                }
                catch(lang::Exception& ex)
                {
                    ex.setMark( __FILE__, __LINE__ );
                }
            }
      };

      class MyTimedWaitingThread
      :
         public lang::Thread,
         public Synchronizable{

      private:

         Mutex mutex;

      public:

         int value;
         MyTimedWaitingThread(){ value = 0;}
         virtual ~MyTimedWaitingThread(){}
         virtual void lock() throw(lang::Exception){
             mutex.lock();
         }
         virtual void unlock() throw(lang::Exception){
             mutex.unlock();
         }
         virtual void wait() throw(lang::Exception){
             mutex.wait();
         }
         virtual void wait(unsigned long millisecs) throw(lang::Exception){
             mutex.wait( millisecs );
         }
         virtual void notify() throw(lang::Exception){
             mutex.notify();
         }
         virtual void notifyAll() throw(lang::Exception){
             mutex.notifyAll();
         }

         virtual void run(){

            try
            {
               synchronized(this)
               {
                  this->wait(2000);

                  value = 666;
               }
            }
            catch(lang::Exception& ex)
            {
                ex.setMark( __FILE__, __LINE__ );
            }
         }
      };

      class MyNotifiedThread
      :
         public lang::Thread,
         public Synchronizable{

      public:

         bool done;
         Mutex* mutex;
         Mutex* started;
         Mutex* completed;

      public:

         int value;
         MyNotifiedThread(Mutex* mutex, Mutex* started, Mutex* completed ){
            this->mutex = mutex;
            this->started = started;
            this->completed = completed;
            this->done = false;
         }
         virtual ~MyNotifiedThread(){}
         virtual void lock() throw(lang::Exception){
             mutex->lock();
         }
         virtual void unlock() throw(lang::Exception){
             mutex->unlock();
         }
         virtual void wait() throw(lang::Exception){
             mutex->wait();
         }
         virtual void wait(unsigned long millisecs) throw(lang::Exception){
             mutex->wait( millisecs );
         }
         virtual void notify() throw(lang::Exception){
             mutex->notify();
         }
         virtual void notifyAll() throw(lang::Exception){
             mutex->notifyAll();
         }

         virtual void run(){

            try
            {
               done = false;
               synchronized(this)
               {
                  synchronized( started )
                  {
                     started->notify();
                  }

                  this->wait();
                  done = true;

                  synchronized( completed )
                  {
                     completed->notify();
                  }
               }
            }
            catch(lang::Exception& ex)
            {
               ex.setMark( __FILE__, __LINE__ );
            }
         }
      };

      class MyRecursiveLockThread
      :
         public lang::Thread,
         public Synchronizable{

      public:

         bool done;
         Mutex* mutex;

      public:

         int value;
         MyRecursiveLockThread(Mutex* mutex){ this->mutex = mutex; done = false; }
         virtual ~MyRecursiveLockThread(){}
         virtual void lock() throw(lang::Exception){
             mutex->lock();
         }
         virtual void unlock() throw(lang::Exception){
             mutex->unlock();
         }
         virtual void wait() throw(lang::Exception){
             mutex->wait();
         }
         virtual void wait(unsigned long millisecs) throw(lang::Exception){
             mutex->wait( millisecs );
         }
         virtual void notify() throw(lang::Exception){
             mutex->notify();
         }
         virtual void notifyAll() throw(lang::Exception){
             mutex->notifyAll();
         }

         virtual void run(){

            try
            {
               done = false;
               synchronized(this)
               {
                  synchronized(this)
                  {
                     this->wait();
                     done = true;
                  }
               }
            }
            catch(lang::Exception& ex)
            {
               ex.setMark( __FILE__, __LINE__ );
            }
         }
      };

      class MyDoubleLockThread
      :
         public lang::Thread
      {

      public:

         bool done;
         Mutex* mutex1;
         Mutex* mutex2;

      public:

         int value;
         MyDoubleLockThread(Mutex* mutex1, Mutex* mutex2)
         {
            this->mutex1 = mutex1;
            this->mutex2 = mutex2;
            done = false;
         }

         virtual ~MyDoubleLockThread(){}

         virtual void run(){

            try
            {
               done = false;
               synchronized(mutex1)
               {
                  synchronized(mutex2)
                  {
                     mutex2->wait();
                     done = true;
                  }
               }
            }
            catch(lang::Exception& ex)
            {
               ex.setMark( __FILE__, __LINE__ );
            }
         }
      };

    public:

        virtual ~MutexTest(){}
        virtual void setUp(){}
        virtual void tearDown(){}

        void testTimedWait();
        void testWait();
        void test();
        void testNotify();
        void testNotifyAll();
        void testRecursiveLock();
        void testDoubleLock();

    };

}}}

#endif /*_DECAF_UTIL_CONCURRENT_MUTEXTEST_H_*/