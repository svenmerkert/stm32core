#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "init.h"


uint32_t SystemCoreClock;


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
				usart_send_blocking(USART_CONSOLE, '\r');
			}
			usart_send_blocking(USART_CONSOLE, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}


void clock_setup(void)
{
	rcc_set_msi_range(RCC_CR_MSIRANGE_4MHZ);
	rcc_osc_on(RCC_MSI);

	rcc_osc_on(RCC_LSE);

	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_MSI, 1, 16, RCC_PLLCFGR_PLLP_DIV7, RCC_PLLCFGR_PLLQ_DIV2, RCC_PLLCFGR_PLLR_DIV2);
	rcc_osc_on(RCC_PLL);
	//rcc_wait_for_osc_ready(RCC_PLL);

	rcc_periph_clock_enable(RCC_GPIOA); // FOR CONSOLE UART
	rcc_periph_clock_enable(RCC_GPIOB); // FOR USER LED
	rcc_periph_clock_enable(RCC_USART1); // FOR TPUART UART
	rcc_periph_clock_enable(RCC_USART2); // FOR CONSOLE UART

	rcc_set_ppre2(RCC_CFGR_PPRE2_NODIV);
	rcc_set_ppre1(RCC_CFGR_PPRE1_NODIV);
	rcc_set_hpre(RCC_CFGR_HPRE_NODIV);
	rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
	rcc_wait_for_sysclk_status(RCC_PLL);

	rcc_ahb_frequency  = 32000000;
	rcc_apb1_frequency = 32000000;
	rcc_apb2_frequency = 32000000;
	SystemCoreClock    = 32000000;
}


void console_usart_setup(void)
{
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

void usart_setup(void)
{
	console_usart_setup();
	//tpuart_usart_setup();
}

void gpio_setup(void)
{
	/* red led for buttons */
	gpio_mode_setup(LED_USER_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_USER_PIN);
	gpio_toggle(LED_USER_PORT, LED_USER_PIN);
}


void systick_setup(int freq)
{
	systick_set_frequency(freq, rcc_ahb_frequency);
	systick_counter_enable();
	systick_interrupt_enable();
}

