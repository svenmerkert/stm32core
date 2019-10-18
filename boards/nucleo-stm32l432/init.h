#ifndef __init__
#define __init__

#define LED_USER_PORT GPIOB
#define LED_USER_PIN GPIO3

void clock_setup(void);
void usart_setup(void);
void gpio_setup(void);
void systick_setup(int freq);


#endif

