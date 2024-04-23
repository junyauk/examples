#pragma once
#ifndef TEST_ACTIVEOBJECT_H
#define TEST_ACTIVEOBJECT_H

#include "tests.h"
#include "activeobject.h"
namespace ActiveObject::Example1 { class Tests : public ITest { public: int run() override; }; }
namespace ActiveObject::Example2 { class Tests : public ITest { public: int run() override; }; }
namespace ActiveObject::Example3 { class Tests : public ITest { public: int run() override; }; }

#endif
