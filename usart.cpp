#include "usart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

UsartDriver::UsartDriver(uint32 usartn)
{
    usart = usartn;
}

void UsartDriver::begin(uint32 baud, uint32 bits, uint32 stopbits, uint32 parity)
{
    /* Setup GPIO pins */
    switch(usart)
    {
    case USART1:
    {
        rcc_periph_clock_enable(RCC_USART1);
        nvic_enable_irq(NVIC_USART1_IRQ);
        

		/* Setup GPIO pins for USART1 transmit. */
		gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
		
		/* Setup for receive */
		gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
		gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO10);

		/* Setup USART2 pins as alternate function. */
		gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
		gpio_set_af(GPIOA, GPIO_AF7, GPIO10);
    }
    break;
    case USART2:
    {
        rcc_periph_clock_enable(RCC_USART2);
        nvic_enable_irq(NVIC_USART2_IRQ);
        
        /* Setup GPIO pins for USART2 transmit. */
		gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
		
		/* Setup for receive */
		gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
		gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);

		/* Setup USART2 pins as alternate function. */
		gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
		gpio_set_af(GPIOA, GPIO_AF7, GPIO3);
    }
    break;
    case USART6:
    {
        rcc_periph_clock_enable(RCC_USART6);
        nvic_enable_irq(NVIC_USART6_IRQ);
        
        /* Setup GPIO pins for USART1 transmit. */
		gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
		
		/* Setup for receive */
		gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7);
		gpio_set_output_options(GPIOC, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO7);

		/* Setup USART6 pins as alternate function. */
		gpio_set_af(GPIOC, GPIO_AF8, GPIO6);
		gpio_set_af(GPIOC, GPIO_AF8, GPIO7);
    }
    break;
    }

    /* Setup UART parameters. */
    usart_set_baudrate(usart, baud);
    usart_set_databits(usart, bits);
    usart_set_stopbits(usart, stopbits);
    usart_set_parity(usart, parity);
    usart_set_flow_control(usart, USART_FLOWCONTROL_NONE);
    usart_set_mode(usart, USART_MODE_TX_RX);

    USART_CR1(usart) |= USART_CR1_RXNEIE;

    /* Finally enable the USART. */
    usart_enable(usart);
}

void UsartDriver::disable()
{
    usart_disable(usart);
}


void usart1_isr(void)
{
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
            ((USART_SR(USART1) & USART_SR_RXNE) != 0))
    {
        Serial1.push(usart_recv(USART1));
    }
}

void usart2_isr(void)
{
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
            ((USART_SR(USART2) & USART_SR_RXNE) != 0))
    {
        char c = usart_recv(USART2);
        Serial2.push(c);
    }
}

void usart3_isr(void)
{
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
            ((USART_SR(USART3) & USART_SR_RXNE) != 0))
    {
        char c = usart_recv(USART3);
        Serial3.push(c);
    }
}


UsartDriver Serial1(USART1);
UsartDriver Serial2(USART2);
UsartDriver Serial3(USART3);


