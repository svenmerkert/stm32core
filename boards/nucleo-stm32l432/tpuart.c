#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <init.h>
#include <ring_buffer.h>

#define RX_BUFFER_LEN 64
#define TX_BUFFER_LEN 64

struct ring_buffer rx_buffer;
struct ring_buffer tx_buffer;
static int tx_it_enable = 0;

void tpuart_usart_setup(void)
{
	init_buffer(&rx_buffer, RX_BUFFER_LEN);
	init_buffer(&tx_buffer, TX_BUFFER_LEN);

	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Setup GPIO pins for USART1 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);

	/* Setup GPIO pins for USART1 receive. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO10);

	/* Setup USART1 TX and RX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO10);

	/* Setup USART1 parameters. */
	usart_set_baudrate(TPUSART, 19200);
	usart_set_databits(TPUSART, 9);
	usart_set_stopbits(TPUSART, USART_STOPBITS_1);
	usart_set_mode(TPUSART, USART_MODE_TX_RX);
	usart_set_parity(TPUSART, USART_PARITY_EVEN);
	usart_set_flow_control(TPUSART, USART_FLOWCONTROL_NONE);

	/* Enable USART1 Receive interrupt. */
	usart_enable_rx_interrupt(TPUSART);

	/* Finally enable the USART. */
	usart_enable(TPUSART);
}

void usart1_isr (void)
{
	static uint8_t data = 'A';

	/* Check if we were called because of RXNE. */
	if ((USART_CR1(TPUSART) & USART_CR1_RXNEIE) != 0) {

		/* Indicate that we got data. */
		gpio_toggle(LED_USER_PORT, LED_USER_PIN);

		/* Retrieve the data from the peripheral. */
		data = usart_recv(TPUSART);
		fifo_push(&rx_buffer, data);
	}

	/* Check if we were called because of TXE. */
	if ((USART_CR1(TPUSART) & USART_CR1_TXEIE) != 0) {
		if(fifo_pop(&tx_buffer, &data) > 0)
		{
			/* Put data into the transmit register. */
			usart_send(TPUSART, data);
		}
		if(buffer_length(&tx_buffer) == 0)
		{
			/* Disable the TXE interrupt as we don't need it anymore. */
			usart_disable_tx_interrupt(TPUSART);
			tx_it_enable = 0;
		}
	}
}

void tpuart_send(uint8_t data)
{
	fifo_push(&tx_buffer, data);
	if(tx_it_enable == 0)
	{
		usart_enable_tx_interrupt(TPUSART);
		tx_it_enable = 1;
	}
}

uint32_t tpuart_rx_size()
{
	return buffer_length(&rx_buffer);
}

uint32_t tpuart_tx_size()
{
	return buffer_length(&rx_buffer);
}

uint8_t tpuart_get()
{
	uint8_t data = 0;
	fifo_pop(&rx_buffer, &data);
	return data;
}
