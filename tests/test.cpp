#include "pch.h"
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


// Note:
// test functions running here are only example.
// They should be called with various conditions originally.
// ButI'm intentionally not implement them.
// Writing test functions with various conditinos is not my purpose here.

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

TEST(MultiThread, ActiveObject)
{
	EXPECT_NO_THROW(Run_ActiveObject());
}

TEST(MultiThread, DoubleCheckedLocking)
{
	EXPECT_NO_THROW(Run_DoubleCheckedLocking());
}

TEST(MultiThread, HalfSyncHalfAsync)
{
	EXPECT_NO_THROW(Run_HalfSyncHalfAsync());
}
