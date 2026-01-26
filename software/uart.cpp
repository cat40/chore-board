#include "uart.hpp"
#include "config.h"

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include <stdint.h>
#include <stdio.h>

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

uint32_t characters_recieved = 0;

void on_uart_rx(void)
{
    printf("uart isr called");
    while (uart_is_readable(UART_ID))
    {
        char c = uart_getc(UART_ID);
        characters_recieved += 1;
        if (uart_is_writable(UART_ID))
        {
            uart_putc(UART_ID, c);
        }
    }
}

void setup_uart(void)
{
    uart_init(UART_ID, 2400);

    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

    uart_set_baudrate(UART_ID, UART_BAUD_RATE);

    // turn off CTS/RTS
    uart_set_hw_flow(UART_ID, false, false);

    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, true);

    // int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    // irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    // irq_set_enabled(UART_IRQ, true);

    // // set up rx only irq
    // uart_set_irq_enables(UART_ID, true, false);
}