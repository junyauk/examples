#pragma once
#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED
#include <Windows.h>
#include "general.h"


// For test
struct PipeData
{
	DWORD dwVal;
	char szMsg[256];
};

#endif // PIPE_H_INCLUDED
