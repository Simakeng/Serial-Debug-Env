#include <Windows.h>

int main(int argc, char *argv[])
{
    HANDLE hport = CreateFile(L"COM1",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hport, &dcbSerialParams))
    {
        // error getting state
    }
    dcbSerialParams.BaudRate = CBR_19200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hport, &dcbSerialParams))
    {
        // error setting serial port state
    }
    return 0;
}
