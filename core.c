#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#ifdef WITH_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#endif
#include "init.h"

static volatile uint64_t _millis = 0;

void fatal_error(int error)
{
    for(;;)
    {
        for(int i = 0; i < error; i++)
        {
            for (int i = 0; i < 3000000; i++) { /* Wait a bit. */
                __asm__("NOP");
            }
            gpio_toggle(LED_USER_PORT, LED_USER_PIN);
            for (int i = 0; i < 1000000; i++) { /* Wait a bit. */
                __asm__("NOP");
            }
            gpio_toggle(LED_USER_PORT, LED_USER_PIN);
        }
        for (int i = 0; i < 9000000; i++) { /* Wait a bit. */
            __asm__("NOP");
        }
    }
}

#ifdef WITH_FREERTOS
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
    fatal_error(10);
}

void vApplicationMallocFailedHook( void )
{
    fatal_error(4);
}

extern void vPortSVCHandler(void) __attribute__ ((naked));
extern void xPortPendSVHandler(void) __attribute__ ((naked));
extern void xPortSysTickHandler(void);


__attribute__((used)) void sv_call_handler(void) {
    vPortSVCHandler();
}

__attribute__((used)) void pend_sv_handler(void) {
    xPortPendSVHandler();
}
#endif

// Get the current value of the millis counter
uint64_t millis() {
    return _millis;
}

void delay(uint32_t delay)
{
	uint32_t wake = millis() + delay;
	while (wake > millis());
}


void sys_tick_handler(void) {
	_millis++;
#ifdef WITH_FREERTOS
    xPortSysTickHandler();
#endif
}
