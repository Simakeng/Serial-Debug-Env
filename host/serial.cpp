#include <Windows.h>
#include <stdio.h>
#include <port_api.h>

namespace Serial
{

    enum class Type
    {
        stdio,
        Physical,
    };
}

struct serial_t
{
    Serial::Type type;
    HANDLE hport;
};

extern "C"
{
    
}