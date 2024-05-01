#pragma once
#ifndef PLUGINDLL_H_INCLUDED
#define PLUGINDLL_H_INCLUDED
#include <Windows.h>


#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void run();


#ifdef __cplusplus
}
#endif


#endif // PLUGINDLL_H_INCLUDED