#include "console.h"
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


/**
 * USART2 GPIO Configuration
 * PA2     ------> USART2_TX
 * RX Not configured
 * Used as debug output console
 */
#define USART_CONSOLE USART2

void console_setup(void)
{
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Setup GPIO pins for USART1 TX. PA2 -> TX */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

    usart_set_baudrate(USART_CONSOLE, 115200);
    usart_set_databits(USART_CONSOLE, 8);
    usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
    usart_set_mode(USART_CONSOLE, USART_MODE_TX);
    usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
    usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART_CONSOLE);
}

void console_putchar(char c)
{
    usart_send_blocking(USART_CONSOLE, c);
}

/**
 * Use USART_CONSOLE as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len)
{
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        for (i = 0; i < len; i++) {
            if (ptr[i] == '\n') {
                console_putchar('\r');
            }
            console_putchar(ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}
