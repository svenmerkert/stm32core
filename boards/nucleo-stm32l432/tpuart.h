#ifndef _TPUART_
#define _TPUART_

#ifdef __cplusplus
extern "C" {
#endif
void tpuart_usart_setup(void);
void tpuart_send(uint8_t data);
uint8_t tpuart_get();
uint32_t tpuart_rx_size();
uint32_t tpuart_tx_size();

#ifdef __cplusplus
}
#endif

#endif
