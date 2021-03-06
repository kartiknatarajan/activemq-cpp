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

#ifndef _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_
#define _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>

#include <decaf/lang/Throwable.h>
#include <decaf/util/concurrent/Callable.h>
#include <decaf/util/concurrent/ThreadFactory.h>
#include <decaf/util/concurrent/ExecutorService.h>
#include <decaf/util/concurrent/ThreadPoolExecutor.h>

namespace decaf {
namespace util {
namespace concurrent {

    using decaf::lang::Thread;
    using decaf::lang::Runnable;

    class ExecutorsTestSupport : public CppUnit::TestFixture {
    public:

        static const int SHORT_DELAY_MS;
        static const int SMALL_DELAY_MS;
        static const int MEDIUM_DELAY_MS;
        static const int LONG_DELAY_MS;

        static const std::string TEST_STRING;

    protected:

        bool threadFailed;

    public:

        ExecutorsTestSupport();
        virtual ~ExecutorsTestSupport();

        virtual void setUp();
        virtual void tearDown();

    public:

        void threadFail(const std::string& reason);
        void threadShouldThrow();
        void threadUnexpectedException();
        void threadUnexpectedException(decaf::lang::Throwable& ex);
        void threadAssertFalse(bool value);
        void threadAssertTrue(bool value);
        void threadAssertEquals(long long x, long long y);

        void unexpectedException();
        void shouldThrow();

        void joinPool(ExecutorService* exec);
        void joinPool(ExecutorService& exec);

        void destroyRemaining(ArrayList<decaf::lang::Runnable*> leftovers);

    public:

        class NoOpRunnable : public decaf::lang::Runnable {
        public:

            NoOpRunnable() : decaf::lang::Runnable() {
            }

            virtual ~NoOpRunnable() {}

            virtual void run() {
            }
        };

        template<typename E>
        class NoOpCallable : public Callable<E> {
        public:

            NoOpCallable() : decaf::util::concurrent::Callable<E>() {
            }

            virtual ~NoOpCallable() {}

            virtual E call() {
                return E();
            }
        };

        class ShortRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            ShortRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~ShortRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SHORT_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class SmallRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            SmallRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~SmallRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        template<typename E>
        class SmallCallable : public Callable<E> {
        private:

            ExecutorsTestSupport* parent;

        public:

            SmallCallable(ExecutorsTestSupport* parent) : decaf::util::concurrent::Callable<E>(), parent(parent) {
            }

            virtual ~SmallCallable() {}

            virtual E call() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }

                return E();
            }
        };

        class SmallInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            SmallInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~SmallInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                    parent->threadShouldThrow();
                } catch(decaf::lang::Exception& e) {
                }
            }
        };

        class SmallPossiblyInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            SmallPossiblyInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~SmallPossiblyInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                } catch(decaf::lang::exceptions::InterruptedException& e) {
                }
            }
        };

        class MediumRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            MediumRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~MediumRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(MEDIUM_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class MediumInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            MediumInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~MediumInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(MEDIUM_DELAY_MS);
                    parent->threadShouldThrow();
                } catch(decaf::lang::Exception& e) {
                }
            }
        };

        class MediumPossiblyInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            MediumPossiblyInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~MediumPossiblyInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(MEDIUM_DELAY_MS);
                } catch(decaf::lang::exceptions::InterruptedException& e) {
                }
            }
        };

        class LongRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            LongRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~LongRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(LONG_DELAY_MS);
                } catch(decaf::lang::Exception& e) {
                    parent->threadUnexpectedException(e);
                }
            }
        };

        class LongInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            LongInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~LongInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(LONG_DELAY_MS);
                    parent->threadShouldThrow();
                } catch(decaf::lang::Exception& e) {
                }
            }
        };

        class LongPossiblyInterruptedRunnable : public decaf::lang::Runnable {
        private:

            ExecutorsTestSupport* parent;

        public:

            LongPossiblyInterruptedRunnable(ExecutorsTestSupport* parent) : decaf::lang::Runnable() {
            }

            virtual ~LongPossiblyInterruptedRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(LONG_DELAY_MS);
                } catch(decaf::lang::exceptions::InterruptedException& e) {
                }
            }
        };


        class SimpleThreadFactory : public ThreadFactory {
        public:

            SimpleThreadFactory() : ThreadFactory() {
            }

            virtual ~SimpleThreadFactory() {}

            virtual Thread* newThread(Runnable* task) {
                return new Thread(task);
            }
        };

        class NoOpREHandler : public RejectedExecutionHandler {
        public:

            NoOpREHandler() : RejectedExecutionHandler() {
            }

            virtual ~NoOpREHandler() {}

            virtual void rejectedExecution(Runnable* r, ThreadPoolExecutor* executor) {
            }
        };

        class TrackedShortRunnable : public Runnable {
        private:

            bool* done;

        public:

            TrackedShortRunnable(bool* done) : decaf::lang::Runnable(), done(done) {
            }

            virtual ~TrackedShortRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(SMALL_DELAY_MS);
                    *done = true;
                } catch(decaf::lang::Exception& e) {
                }
            }
        };

        class TrackedNoOpRunnable : public Runnable {
        private:

            bool* done;

        public:

            TrackedNoOpRunnable(bool* done) : decaf::lang::Runnable(), done(done) {
            }

            virtual ~TrackedNoOpRunnable() {}

            virtual void run() {
                *done = true;
            }
        };

        class TrackedLongRunnable : public decaf::lang::Runnable {
        private:

            bool* done;

        public:

            TrackedLongRunnable(bool* done) : decaf::lang::Runnable(), done(done) {
            }

            virtual ~TrackedLongRunnable() {}

            virtual void run() {
                try {
                    Thread::sleep(LONG_DELAY_MS);
                    *done = true;
                } catch(decaf::lang::Exception& e) {
                }
            }
        };

        class StringTask : public decaf::util::concurrent::Callable<std::string> {
        public:

            StringTask() : decaf::util::concurrent::Callable<std::string>() {
            }

            virtual ~StringTask() {}

            std::string call() {
                return TEST_STRING;
            }
        };

    };

}}}

#endif /* _DECAF_UTIL_CONCURRENT_EXECUTORSTESTSUPPORT_H_ */
