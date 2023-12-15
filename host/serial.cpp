#include <Windows.h>
#include <stdio.h>
#include <port_api.h>
#include "host_error.hpp"
#include <mutex>
#include <deque>

#include <thread>

namespace Serial
{

    enum class Type
    {
        Unknown = 0,
        Console,
        Physical,
    };

    /***************************************************************
     * TODO: This 'init - exit' mechanism is not implemented yet.
     * this part for tracking the resource usage of the host
     * environment, and release the resource when the application
     * exit. this can make sure all the resource aquired by the
     * host is released.
     *
     * This will be helpful when user want to debug memory leak of
     * Application using tools like valgrind.
     ***************************************************************/

    /**
     * @brief prepare the host environment
     */
    void HostInit()
    {
        // see above
    }

    /**
     * @brief clean up
     */
    void HostExit()
    {
        // see above
    }
}

struct serial_t
{
    Serial::Type type;
    HANDLE hport;
    HANDLE hthread;
    bool thread_exit;
    std::thread rx_thread;

    uart_rx_handler_t handler;
};

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

        auto serial = new serial_t;
        memset(serial, 0, sizeof(serial_t));
        device->dummy = serial;

        return;
    }

    void uart_register_rx_handler(uart_device_t *device, uart_rx_handler_t handler)
    {
        if (device == nullptr)
            error_exit("device is nullptr\n");

        auto serial = reinterpret_cast<serial_t *>(device->dummy);
        serial->handler = handler;
    }

    void uart_init_physical(uart_device_t *device, uart_init_physical_t *init)
    {
        if (device == nullptr)
            error_exit("device is nullptr\n");

        if (init == nullptr)
            error_exit("init is nullptr\n");

        // Print the attributes in init to stderr
        fprintf(stderr, "Creating physical UART port with args:\n");
        fprintf(stderr, COLOR_BEIGE);
        fprintf(stderr, "Port Name: %s\n", init->port_name);
        fprintf(stderr, "Baud Rate: %d\n", init->buadrate);
        fprintf(stderr, "Data Bits: %d\n", init->data_bits);
        fprintf(stderr, "Stop Bits: %s\n", strf_uart_stop_bits_e(init->stop_bits));
        fprintf(stderr, "Parity: %s\n", strf_uart_parity_e(init->parity));
        fprintf(stderr, COLOR_RESET);

        auto serial = new serial_t;
        device->dummy = serial;
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

        // set port attributes
        DCB dcbSerialParams = {0};
        memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
        dcbSerialParams.BaudRate = init->buadrate;
        dcbSerialParams.ByteSize = init->data_bits;
        dcbSerialParams.StopBits = init->stop_bits;
        dcbSerialParams.Parity = init->parity;
        if (!SetCommState(hport, &dcbSerialParams))
            error_system_api_exit("failed to set port state.\n");

        // set port timeout
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

        // create rx thread
        serial->thread_exit = false;
        serial->rx_thread = std::thread([serial]()
                                        {
            while (!serial->thread_exit)
            {
                DWORD dwRead;
                uint8_t ch;
                if (ReadFile(serial->hport, &ch, 1, &dwRead, NULL))
                {
                    if (dwRead > 0)
                    {
                        if (serial->handler != nullptr)
                        {
                            serial->handler(ch, 0);
                        }
                    }
                }
                else
                {
                    error_system_api_exit("failed to read from port.\n");
                }
            } });

        fprintf(stderr, COLOR_GREEN "[SUCCESS]\n" COLOR_RESET);

        return;
    }
    void uart_tx_send_byte(uart_device_t *device, uint8_t byte)
    {
        if (device == nullptr)
            error_exit("device is nullptr\n");

        auto serial = reinterpret_cast<serial_t *>(device->dummy);

        if (serial->type == Serial::Type::Physical)
        {
            DWORD dwWrite;
            if (!WriteFile(serial->hport, &byte, 1, &dwWrite, NULL))
                error_system_api_exit("failed to write to port.\n");
        }
        else if (serial->type == Serial::Type::Console)
        {
            fprintf(stderr, "%c", byte);
        }
        else
        {
            error_exit("unknown serial type %d.\n", (int)serial->type);
        }

        return;
    }
}
