#pragma once
#ifndef JOBOBJECT_H_INCLUDED
#define JOBOBJECT_H_INCLUDED
#include "Windows.h"


namespace JobObject::Windows1
{
	HANDLE CreateJobObjectWrapper(LPCTSTR lpName);
	bool AssignProcessToJobObjectWrapper(HANDLE hJob, HANDLE hProcess);
	bool SetJobObjectCpuRate(HANDLE hJob, DWORD maxCpuRate);
	bool SetJobObjectSchedulingClass(HANDLE hJob, DWORD schedulingClass);
}





#endif // JOBOBJECT_H_INCLUDED