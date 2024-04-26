#include "pch.h"
#include "Windows.h"
#include "general.h"


#include "tests.h"
#include "tests_jobobject.h"

namespace JobObject::Windows1
{
	static shared_ptr<PROCESS_INFORMATION> CreateEmptyProcess()
	{
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi;

		std::wstring batPath = GetTestFolderPath() + L"emptyprocess.bat";

		if (!CreateProcess(batPath.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			DWORD lastError = GetLastError();
			std::cerr << "Failed to create process: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
			return nullptr;
		}
		return make_shared<PROCESS_INFORMATION>(pi);
	}

	int Tests::run()
	{
		// Job Object Windows APIs are used for managing a group of processes as a job.
		// 1. Create a job as a group
		// 2. Assign process to the job
		// 3. Specify settings to processes assigned to the job

		// 1. Create a job as a group
		HANDLE hJob = CreateJobObjectWrapper(NULL);
		if (!hJob)
		{
			DWORD lastError = GetLastError();
			cout << "Failed to create a job object: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
			return 1;
		}

		// Create empty processes and assign to the job object
		const int numChildProcesses = 3;
		vector<shared_ptr<PROCESS_INFORMATION>> processIds;
		for (auto i = 0; i < numChildProcesses; ++i)
		{
			auto pi = CreateEmptyProcess();
			if (!pi)
			{
				cerr << "Failed to create an empty process.\n";
				// Terminate and close handles for each process;
				for (auto& p : processIds)
				{
					TerminateProcess(p->hProcess, 0);
					CloseHandle(p->hProcess);
					CloseHandle(p->hThread);
				}
				CloseHandle(hJob);
				return 1;
			}

			// Pretending booting the process to make sure the process is available
			Sleep(1500);

			// 2. Assign process to the job
			if (AssignProcessToJobObjectWrapper(hJob, pi->hProcess))
			{
				cerr << "Failed to assign process to job object.\n";
				// Terminate and close handles for each process;
				for (auto& p : processIds)
				{
					TerminateProcess(p->hProcess, 0);
					CloseHandle(p->hProcess);
					CloseHandle(p->hThread);
				}
				CloseHandle(hJob);
				return 1;
			}

			processIds.push_back(pi);
		}

		cout << "Empty processes were created and assigned to job object.\n";

		// 3. Specify settings (process priority) to processes assigned to the job
		if (!SetJobObjectSchedulingClass(hJob, 4)) // range 0 to 9
		{
			DWORD lastError = GetLastError();
			cerr << "Failed to set Job Object scheduling class: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;

			// Close handle
			CloseHandle(hJob);
			return 1;
		}

		// 3. Specify settings (CPU rate 50%) to processes assigned to the job
		if (!SetJobObjectCpuRate(hJob, 50))
		{
			cerr << "Failed to set Job Object CPU rate limit: " << GetLastError() << endl;

			// Terminate and close handles for each process;
			for (auto& p : processIds)
			{
				TerminateProcess(p->hProcess, 0);
				CloseHandle(p->hProcess);
				CloseHandle(p->hThread);
			}
			CloseHandle(hJob);
			return 1;
		}

		cout << "CPU rate control set to job object.\n";

		Sleep(2000);

		// Terminate and close handles for each process;
		for (auto& p : processIds)
		{
			TerminateProcess(p->hProcess, 0);
			CloseHandle(p->hProcess);
			CloseHandle(p->hThread);
		}
		CloseHandle(hJob);

		return 0;
	}
}

