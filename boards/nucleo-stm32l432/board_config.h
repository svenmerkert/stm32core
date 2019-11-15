#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#define USER_MEMORY_ADDR 0x0803F800
#define USER_MEMORY_SIZE 0x800
#define USER_MEMORY_PAGE 127

#define LED_USER_PORT GPIOB
#define LED_USER_PIN GPIO3

#define KNX_PROG_LED_PORT LED_USER_PORT
#define KNX_PROG_LED_PIN LED_USER_PIN

#define KNX_PROG_BTN_PORT GPIOB
#define KNX_PROG_BTN_PIN GPIO4

/**
 * USART1 GPIO Configuration
 * PA9     ------> USART1_TX
 * PA10    ------> USART1_RX
 * Used for KNX TPUART
 */
#define KNX_TPUSART       USART1

void board_setup();
void console_putchar(char c);

#ifdef __cplusplus
}
#endif
