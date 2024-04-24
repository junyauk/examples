#include "pch.h"
#include "Windows.h"
#include "tests.h"

wstring GetTestFolderPath()
{
	wstring path = FindFolderInPath(L"examples");
	return path + L"\\tests\\";
}
