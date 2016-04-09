#include "clock.h"
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/flash.h>
#include <etk/etk.h>



void sys_tick_handler(void)
{
    etk::tick();
}

void clock_setup()
{
	/*
		.pllm = 16,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_4,
		.ppre2 = RCC_CFGR_PPRE_DIV_2,
		.flash_config = FLASH_ACR_ICE | FLASH_ACR_DCE |
				FLASH_ACR_LATENCY_5WS,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	*/
	
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);
	
	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_HSI);
	
	pwr_set_vos_scale(PWR_SCALE1);
	
    rcc_set_main_pll_hsi(16, 336, 2, 8);
    
    /* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);

	/* Configure flash settings. */
	flash_set_ws(FLASH_ACR_ICE | FLASH_ACR_DCE |
				FLASH_ACR_LATENCY_5WS);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
	
	rcc_set_hpre(RCC_CFGR_HPRE_DIV_NONE);
	rcc_set_ppre1(RCC_CFGR_PPRE_DIV_4);
	rcc_set_ppre2(RCC_CFGR_PPRE_DIV_2);

	/* Wait for PLL clock to be selected. */
	rcc_wait_for_sysclk_status(RCC_PLL);

	rcc_ahb_frequency = 168000000;
	rcc_apb1_frequency = 42000000;
	rcc_apb2_frequency = 84000000;
	
	/* Disable internal high-speed oscillator. */
	rcc_osc_off(RCC_HSI);

	// clock rate is 1680 to get 10uS interrupt rate 
	systick_set_reload(1680);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();

	systick_interrupt_enable();
	
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    etk::set_tick_rate(10);
}



