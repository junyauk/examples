#include "pch.h"
#include "basic.h"
#include "balking.h"
#include "multithreadsum.h"
#include "singlethreadexecution.h"
#include "guardedsuspention.h"
#include "conditionvariableexample.h"
#include "futurepromise.h"
#include "producerconsumer.h"
#include "readlockwritelock.h"
#include "threadpermessage.h"


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

TEST(MultiThread, GuardedSuspention)
{
	EXPECT_NO_THROW(Run_GuardedSuspention());
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

