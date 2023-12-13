#include <port_api.h>


int main(int argc, char *argv[])
{
    uart_init_physical_t init = {0};

    init.port_name = "COM3";
    init.buadrate = 115200;
    init.data_bits = 8;
    init.stop_bits = UART_STOP_BIT_1;
    init.parity = UART_PARITY_NO;
    
    uart_device_t device;

    uart_init_physical(&device, &init);

    return 0;
}
