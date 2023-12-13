/**
 * @file port_api.h
 * @author simakeng (simakeng@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-12-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>

#ifndef __PORT_API_H__
#define __PORT_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief UART parity mode
     */
    typedef enum
    {
        UART_PARITY_NO = 0,    // No parity
        UART_PARITY_ODD = 1,   // Odd parity
        UART_PARITY_EVEN = 2,  // Even parity
        UART_PARITY_MARK = 3,  // Mark parity
        UART_PARITY_SPACE = 4, // Space parity
    } uart_parity_e;

    /**
     * @brief UART stop bits
     */
    typedef enum
    {
        UART_STOP_BIT_1 = 0,   // 1 stop bit
        UART_STOP_BIT_1_5 = 1, // 1.5 stop bits
        UART_STOP_BIT_2 = 2,   // 2 stop bits
    } uart_stop_bits_e;

    /**
     * @brief Initialization parameters of UART port
     */
    typedef struct
    {
        const char *port_name;
        uint32_t buadrate;
        uart_parity_e parity;
        uart_stop_bits_e stop_bits;
        uint8_t data_bits;
    } uart_init_physical_t;

    /**
     * @brief UART device handle
     */
    typedef struct
    {
        uint8_t dummy[64];
    } uart_device_t;

    /**
     * @brief Initialize UART port and connect it to a physical port
     *
     * @warning if failed, this function will exit the program
     * and print error message to stderr
     *
     * @param device pointer to UART device handle
     * @param init pointer to UART initialization parameters
     */
    void uart_init_physical(uart_device_t *device, uart_init_physical_t *init);

    /**
     * @brief Initialize UART port and connect it to STDIO
     *
     * @warning only one UART port can be connected to STDIO
     *
     * @param device
     */
    void uart_init_stdio(uart_device_t *device);

    typedef void (*uart_rx_handler_t)(uint32_t byte, uint32_t flag);

    /**
     * @brief Register UART RX handler
     *
     * @note handler is called in another thread, when handler is going to be
     * called, the main thread will be suspended to simulate the interrupt
     * in embedded systems. But the timing is not correct because Windows is not
     * a RTOS.
     *
     * @param device pointer to UART device handle
     * @param handler handler function
     */
    void register_uart_rx_handler(uart_device_t *device, uart_rx_handler_t handler);

    /**
     * @brief Send a byte to UART port
     *
     * @param device pointer to UART device handle
     * @param byte byte to send
     */
    void uart_tx_send_byte(uart_device_t *device, uint8_t byte);

    /**
     * @brief main entry of the application
     * 
     * @param argc 
     * @param argv 
     * @return int 
     */
    int app_main(int argc, char *argv[]);
#ifdef __cplusplus
}
#endif

#endif // ! #ifndef __PORT_API_H__