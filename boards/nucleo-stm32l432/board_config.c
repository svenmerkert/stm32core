#include "board_config.h"
#include "console.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>


uint32_t SystemCoreClock;


void clock_setup(void)
{
    rcc_set_msi_range(RCC_CR_MSIRANGE_4MHZ);
    rcc_osc_on(RCC_MSI);
    rcc_osc_on(RCC_LSE);

    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_MSI, 1, 16, (0x7 << 27), RCC_PLLCFGR_PLLQ_DIV2, RCC_PLLCFGR_PLLR_DIV2);
    rcc_osc_on(RCC_PLL);
    //rcc_wait_for_osc_ready(RCC_PLL);

    RCC_CR |= RCC_CR_MSIPLLEN;

    rcc_set_ppre2(RCC_CFGR_PPRE2_NODIV);
    rcc_set_ppre1(RCC_CFGR_PPRE1_NODIV);
    rcc_set_hpre(RCC_CFGR_HPRE_NODIV);
    rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
    rcc_wait_for_sysclk_status(RCC_PLL);

    rcc_ahb_frequency  = 32000000;
    rcc_apb1_frequency = 32000000;
    rcc_apb2_frequency = 32000000;
    SystemCoreClock    = 32000000;

    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_DMA1);
}

void systick_setup(int freq)
{
    systick_set_frequency(freq, rcc_ahb_frequency);
    systick_counter_enable();
    systick_interrupt_enable();
}

void board_setup()
{
    clock_setup();
    systick_setup(1000); /* systick 1 ms */
    console_setup();
}

