#include <Windows.h>
#include <stdio.h>
#include <port_api.h>
#include "host_error.hpp"
#include <mutex>
#include <deque>

namespace Serial
{

    enum class Type
    {
        Unknown = 0,
        Console,
        Physical,
    };
}

struct serial_t
{
    Serial::Type type;
    HANDLE hport;
    HANDLE hthread;
};

// a little notice to mind you that serial_t is using the memory of static allocated
// by uart_device_t;
static_assert(sizeof(serial_t) <= sizeof(uart_device_t), "uart_device_t is too small");

extern "C"
{
    void uart_init_stdio(uart_device_t *device)
    {
        if (device == nullptr)
            error_exit("device is nullptr\n");

        static void *device_handle = nullptr;

        if (device_handle != nullptr)
        {
            error_exit("stdio has already be bind to device %p.\n", device_handle);
        }

        device_handle = device;

        auto serial = reinterpret_cast<serial_t *>(device);
        memset(serial, 0, sizeof(serial_t));
        serial->type = Serial::Type::Console;

        return;
    }
}

void uart_init_physical(uart_device_t *device, uart_init_physical_t *init)
{
    if (device == nullptr)
        error_exit("device is nullptr\n");

    if (init == nullptr)
        error_exit("init is nullptr\n");

    auto serial = reinterpret_cast<serial_t *>(device);
    memset(serial, 0, sizeof(serial_t));
    serial->type = Serial::Type::Physical;

    // create port
    auto hport = CreateFileA(init->port_name,
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);

    if (hport == INVALID_HANDLE_VALUE)
        error_system_api_exit("failed to open port '%s'.\n", init->port_name);

    serial->hport = hport;

    DCB dcbSerialParams = {0};
    memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
    dcbSerialParams.BaudRate = init->buadrate;
    dcbSerialParams.ByteSize = init->data_bits;
    dcbSerialParams.StopBits = init->stop_bits;
    dcbSerialParams.Parity = init->parity;
    if (!SetCommState(hport, &dcbSerialParams))
        error_system_api_exit("failed to set port state.\n");

    COMMTIMEOUTS timeouts;
    memset(&timeouts, 0, sizeof(timeouts));

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hport, &timeouts))
    {
        error_system_api_exit("failed to set port timeout.\n");
    }

    return;
}