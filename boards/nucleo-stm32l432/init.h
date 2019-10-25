#ifndef __init__
#define __init__

#define LED_USER_PORT GPIOB
#define LED_USER_PIN GPIO3

void clock_setup(void);
void usart_setup(void);
void gpio_setup(void);
void systick_setup(int freq);

/**
 * USART2 GPIO Configuration
 * PA2     ------> USART2_TX
 * PA15    ------> USART2_RX
 * Used as debug output console
 */
#define USART_CONSOLE USART2  /* PD5/6 , af7 */

/**
 * USART1 GPIO Configuration
 * PA9     ------> USART1_TX
 * PA10    ------> USART1_RX
 * Used for KNX TPUART
 */
#define TPUSART       USART1


#endif
