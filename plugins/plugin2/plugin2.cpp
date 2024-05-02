#include "pch.h"
#include <Windows.h>

#include "general.h"
#include "plugin2.h"

Plugin2::Plugin2()
{
    cout << "Plugin2 constructor\n";
}
Plugin2::~Plugin2()
{
    cout << "Plugin2 destructor\n";
}
int Plugin2::run()
{
    cout << "run() in plugin DLL 2 was called\n";
    return 0;
}

extern "C" {
    __declspec(dllexport) IPlugin* createPlugin()
    {
        return new Plugin2();
    }
}


