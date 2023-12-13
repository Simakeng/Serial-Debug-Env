#include <port_api.h>
#include <timer_api.h>
#include <debug_api.h>

static const uint32_t this_application_is_running_for_ms = 100000;

void uart_rx_cplt(uint32_t byte, uint32_t flag)
{
    debug_putc(byte);
}

void uart_tx_send_string(uart_device_t* device,const char* str)
{
    while(*str)
    {
        uart_tx_send_byte(device,*str++);
    }
}

int app_main(int argc, char *argv[])
{
    uart_init_physical_t init = {0};

    init.port_name = "COM3";
    init.buadrate = 256000;
    init.data_bits = 8;
    init.stop_bits = UART_STOP_BIT_1;
    init.parity = UART_PARITY_NO;

    uart_device_t device;

    uart_init_physical(&device, &init);

    uart_register_rx_handler(&device, uart_rx_cplt);

    uart_tx_send_string(&device, "Hello world!\n");

    while (1)
    {
        sys_delay_ms(10);
    }
}