#include "pch.h"
#include "Windows.h"
#include "general.h"


namespace JobObject::Windows1
{
	HANDLE CreateJobObjectWrapper(LPCTSTR lpName)
	{
		// Create a job object
		return CreateJobObject(NULL, lpName);
	}
	bool AssignProcessToJobObjectWrapper(HANDLE hJob, HANDLE hProcess)
	{
		// Assign a process to the job
		return AssignProcessToJobObject(hJob, hProcess);
	}

	bool SetJobObjectCpuRate(HANDLE hJob, DWORD maxCpuRate)
	{
		// Set various settings (this is an example setting Max CPU rate).
		JOBOBJECT_CPU_RATE_CONTROL_INFORMATION cpuRateInfo = { 0 };
		cpuRateInfo.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE;
		cpuRateInfo.CpuRate = maxCpuRate;

		return SetInformationJobObject(hJob, JobObjectCpuRateControlInformation, &cpuRateInfo, sizeof(cpuRateInfo));
	}

	bool SetJobObjectSchedulingClass(HANDLE hJob, DWORD schedulingClass)
	{
		// Set various settings (this is an example setting scheduling).

#if 1 // Use JOBOBJECT_BASIC_LIMIT_INFORMATION and JobObjectBasicLimitInformation
		JOBOBJECT_BASIC_LIMIT_INFORMATION basicLimit = { 0 };

		basicLimit.LimitFlags = JOB_OBJECT_LIMIT_SCHEDULING_CLASS;
		basicLimit.SchedulingClass = schedulingClass;

		return SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &basicLimit, sizeof(basicLimit));
#else // Use JOBOBJECT_EXTENDED_LIMIT_INFORMATION and JobObjectExtendedLimitInformation
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION extendedLimit = { 0 };
		extendedLimit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_SCHEDULING_CLASS;
		extendedLimit.BasicLimitInformation.SchedulingClass = NORMAL_PRIORITY_CLASS;
		return SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &extendedLimit, sizeof(extendedLimit));
#endif
	}
}

