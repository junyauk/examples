#include "pch.h"
#include <Windows.h>

#include "general.h"
#include "plugin1.h"

Plugin1::Plugin1()
{
    cout << "Plugin1 constructor\n";
}
Plugin1::~Plugin1()
{
    cout << "Plugin1 destructor\n";
}
int Plugin1::run()
{
    cout << "run() in plugin DLL 1 was called\n";
    return 0;
}

extern "C" {
    __declspec(dllexport) IPlugin* createPlugin()
    {
        return new Plugin1();
    }
}


