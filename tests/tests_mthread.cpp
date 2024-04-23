#include "pch.h"
#include <gtest/gtest.h>

#include "activeobject.h"
#include "basic.h"
#include "balking.h"
#include "multithreadsum.h"
#include "singlethreadexecution.h"
#include "guardedsuspension.h"
#include "conditionvariableexample.h"
#include "futurepromise.h"
#include "producerconsumer.h"
#include "readlockwritelock.h"
#include "threadpermessage.h"
#include "threadspecificstorage.h"
#include "twophasetermination.h"
#include "workerthread.h"
#include "doublecheckedlocking.h"
#include "halfsynchalfasync.h"
#include "leaderfollower.h"
#include "monitorobject.h"
#include "reactor.h"
#include "readerwriter.h"
#include "barrier.h"
#include "threadpool.h"

#include "tests.h"
#include "tests_activeobject.h"
// Note:
// These tests are for testing multi thread examples
// They should be called with various conditions originally.
// But currently only runs basic ones, will be tested with various conditions


class MultiThreadTests : public ::testing::TestWithParam<vector<shared_ptr<ITest>>>
{
protected:
	void SetUp() override {
		tests = GetParam();
	}

	int run() {
		for (const auto& test : tests) {
			test->run();
		}
		return 0; // Return success
	}

private:
	vector<shared_ptr<ITest>> tests;
};

// Define a static std::vector<std::shared_ptr<ITest>> named testInstances
static std::vector<std::shared_ptr<ITest>> testInstances =
{ 
	std::make_shared<ActiveObject::Example1::Test>(),
	std::make_shared<ActiveObject::Example2::Test>(),
	std::make_shared<ActiveObject::Example3::Test>()
};

TEST_P(MultiThreadTests, RunTests) {
	int ret = 0;
	EXPECT_NO_THROW(ret = run());
	EXPECT_EQ(ret, 0);
}


TEST(MultiThread, Basic)
{
	EXPECT_NO_THROW(Run_Basic());
}

TEST(MultiThread, Balking)
{
	EXPECT_NO_THROW(Run_Balking());
}

TEST(MultiThread, MThread)
{
	EXPECT_NO_THROW(Run_MThread_Sum());
}

TEST(MultiThread, SingleThreadExecution)
{
	EXPECT_NO_THROW(Run_SingleThreadExecution());
}

TEST(MultiThread, GuardedSuspension)
{
	EXPECT_NO_THROW(Run_GuardedSuspension());
}

TEST(MultiThread, ConditionVariableExample)
{
	EXPECT_NO_THROW(Run_ConditionVariable());
}

TEST(MultiThread, FuturePromise)
{
	EXPECT_NO_THROW(Run_FuturePromise());
}

TEST(MultiThread, ProducerConsumer)
{
	EXPECT_NO_THROW(Run_ProducerConsumer());
}

TEST(MultiThread, ReadLockWriteLock)
{
	EXPECT_NO_THROW(Run_ReadLockWriteLock());
}

TEST(MultiThread, ThreadPerMessage)
{
	EXPECT_NO_THROW(Run_ThreadPerMessage());
}

TEST(MultiThread, WorkerThread)
{
	EXPECT_NO_THROW(Run_WorkerThread());
}

TEST(MultiThread, TwoPhaseTermination)
{
	EXPECT_NO_THROW(Run_TwoPhaseTermination());
}

TEST(MultiThread, ThreadSpecificStorage)
{
	EXPECT_NO_THROW(Run_ThreadSpecificStorage());
}

TEST(MultiThread, ActiveObjectBasic1)
{
	using namespace ActiveObject::Basic1;
	Tests tests;
	EXPECT_NO_THROW(tests.run());
}
TEST(MultiThread, ActiveObjectBasic2)
{
	using namespace ActiveObject::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, DoubleCheckedLocking)
{
	EXPECT_NO_THROW(Run_DoubleCheckedLocking());
}

TEST(MultiThread, HalfSyncHalfAsync)
{
	EXPECT_NO_THROW(Run_HalfSyncHalfAsync());
}

TEST(MultiThread, LeaderFollower)
{
	EXPECT_NO_THROW(Run_LeaderFollower());
}

TEST(MultiThread, MonitorObject)
{
	EXPECT_NO_THROW(Run_MonitorObject());
}

TEST(MultiThread, Reactor)
{
	EXPECT_NO_THROW(Run_Reactor());
}

TEST(MultiThread, ReaderWriterBasic1)
{
	using namespace ReaderWriter::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ReaderWriterBasic1Step2)
{
	using namespace ReaderWriter::Basic1Step2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, BarrierBasic1)
{
	using namespace Barrier::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, BarrierBasic2)
{
	using namespace Barrier::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ThreadPoolBasic1)
{
	using namespace ThreadPool::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ThreadPoolProducerConsumer)
{
	using namespace ThreadPool::ProducerConsumer;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ThreadPoolStrategy)
{
	using namespace ThreadPool::Strategy;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, MultiThreadObserver)
{
	using namespace MultiThread::Observer;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ReadLockWriteLockSingletonBasic1)
{
	using namespace ReadLockWriteLock::Singleton::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ReadLockWriteLockSingletonBasic2)
{
	using namespace ReadLockWriteLock::Singleton::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ThreadSpecificStorageFactoryMethodBasic1)
{
	using namespace ThreadSpecificStorage::FactoryMethod::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiThread, ThreadSpecificStorageFactoryMethodBasic2)
{
	using namespace ThreadSpecificStorage::FactoryMethod::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

