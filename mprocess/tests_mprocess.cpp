#include "pch.h"
#include "general.h"
#include "pipe.h"

TEST(MultiProcess, MemoryMappedFile)
{
	wstring wsfilePath = GetTestFolderPath() + TEXT("Resources") + TEXT("\\") + TEXT("dijeh.bmp");
	BitmapFile bitmapFile(wsfilePath);

	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		bitmapFile.fileSize(),
		TEXT("LOCAL//MemoryMappedFile"));
	if (!hMapFile)
	{
		auto lastError = GetLastError();
		cerr << "CreateFileMapping() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return;
	}

	LPVOID pData = MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		bitmapFile.fileSize());
	if (!pData)
	{
		auto lastError = GetLastError();
		cerr << "MapViewOfFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		CloseHandle(hMapFile);
		return;
	}

	// Load the bitmap file
	CopyMemory(pData, bitmapFile.bitmapFileHeader(), bitmapFile.fileSize());
	BitmapFile mappedBitmap(pData);
	string expected = "aa629fbf81affc709e46dd2c69c2b3464e3f6e7adf6f7eb179094319d7e6550a";
	EXPECT_STREQ(expected.c_str(), mappedBitmap.hash().c_str());

	wstring path = GetRunningPath()
		+ TEXT("\\")
		+ TEXT("ProcessA.exe")
		+ TEXT(" MemoryMappedFile");

	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };
	if (!CreateProcess(
		NULL,
		(LPWSTR)path.c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED, // for debugging
		NULL,
		NULL,
		&si,
		&pi))
	{
		auto lastError = GetLastError();
		cerr << "MapViewOfFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		UnmapViewOfFile(pData);
		CloseHandle(hMapFile);
		return;
	}
	ResumeThread(pi.hThread); // for debugging. Attach ProcessA before running this.

	WaitForSingleObject(pi.hProcess, INFINITE);

	// The bitmap image has been overwritten. The hash must be different from before.
	string newExpected = "b8e325daea4a99f870f7c24f74eb0495a65e7f62a78649ef8948ecf7d44595ad";
	string newActual = mappedBitmap.hash();
	EXPECT_STREQ(newExpected.c_str(), newActual.c_str());

	UnmapViewOfFile(pData);
	CloseHandle(hMapFile);
}



TEST(MultiProcess, AnonymousPipe)
{
	HANDLE hReadPipe, hWritePipe;
	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreatePipe failed. (") << lastError << ") " << GetLastErrorMessageW(lastError) << endl;
		return;
	}

	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;

	wstring path = GetRunningPath()
		+ TEXT("\\")
		+ TEXT("ProcessA.exe")
		+ TEXT(" AnonymousPipe");

	if (!CreateProcess(
		NULL,
		(LPWSTR)path.c_str(),
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi))
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreteProcess failed. (") << lastError << ") " << GetLastErrorMessageW(lastError) << endl;
		return;
	}
	ResumeThread(pi.hThread); // for debugging. Attach ProcessA before running this.

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(hWritePipe);

	string expected = "Sending texts from Process A using cout.\r\nSending texts from Process A using cerr.\r\n";
	string actual;
	DWORD dwRead;
	char szBuf[4096];
	BOOL bSuccess = FALSE;
	while (true)
	{
		bSuccess = ReadFile(
			hReadPipe,
			szBuf,
			sizeof(szBuf),
			&dwRead,
			NULL);
		if (!bSuccess || (dwRead == 0))
		{
			break;
		}
		actual = string(szBuf, dwRead);
	}

	EXPECT_STREQ(expected.c_str(), actual.c_str());

	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

TEST(MultiProcess, NamedPipe)
{
	HANDLE hPipe;
	HANDLE hReadyToRead = CreateEvent( NULL, FALSE, FALSE, TEXT("ReadyToRead"));
	if (hReadyToRead == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreateEvent for ReadyToRead failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return;
	}
	HANDLE hPipeEnabled = CreateEvent( NULL, FALSE, FALSE, TEXT("PipeReady"));
	if (hPipeEnabled == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreateEvent for PipeReady failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return;
	}

	// Execute Process A. It will wait for the PipeReady event.
	wstring path = GetRunningPath()
		+ TEXT("\\")
		+ TEXT("ProcessA.exe")
		+ TEXT(" NamedPipe");

	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };
	if (!CreateProcess(
		NULL,
		(LPWSTR)path.c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED, // for debugging
		NULL,
		NULL,
		&si,
		&pi))
	{
		auto lastError = GetLastError();
		cerr << "CreateProcess failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		CloseHandle(hReadyToRead);
		CloseHandle(hPipeEnabled);
		return;
	}
	ResumeThread(pi.hThread); // for debugging. Attach ProcessA before running this.

	// Create named pipe.
	hPipe = CreateNamedPipe(
		TEXT("\\\\.\\pipe\\NamedPipe"), // lpName
		PIPE_ACCESS_INBOUND,			// dwOpenMode
		PIPE_TYPE_BYTE | PIPE_WAIT,		// dwPipeMode
		1,								// nMaxInstances
		sizeof(PipeData),				// nOutBufferSize
		sizeof(PipeData),				// nInBufferSize
		100,							// nDefaultTimeOut
		NULL);							// lpSecurityAttributes
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreateNamedPipe failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hReadyToRead);
		CloseHandle(hPipeEnabled);
		return;
	}

	// Pipe is ready, and let the Process A know it.
	SetEvent(hPipeEnabled);

	DWORD dwTimes = 0;
	while (1)
	{
		DWORD dwRead;
		PipeData receivedData;
		WaitForSingleObject(hReadyToRead, INFINITE);

		if (!ReadFile(
			hPipe,
			&receivedData,
			sizeof(receivedData),
			&dwRead,
			NULL))
		{
			auto lastError = GetLastError();
			cerr << "ReadFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
			break;
		}
		else
		{
			string message(receivedData.szMsg);
			if (message == "Exit")
			{
				break;
			}
			EXPECT_EQ(receivedData.dwVal, dwTimes);
			EXPECT_STREQ(receivedData.szMsg, "Message from Process A.");
			++dwTimes;
		}
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hReadyToRead);
	CloseHandle(hPipeEnabled);
	CloseHandle(hPipe);

	return;
}
