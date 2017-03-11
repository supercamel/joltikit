#include "joltikit/joltikit.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>

using namespace etk;

const uint32_t dma_read_length = 120;
volatile char buf[dma_read_length];
volatile char buf1[dma_read_length];


static void dma_init()
{
	rcc_periph_clock_enable(RCC_DMA2);
	nvic_enable_irq(NVIC_DMA2_STREAM1_IRQ);
	usart_enable_rx_dma(USART6);
}

static void dma_read()
{
	dma_stream_reset(DMA2, DMA_STREAM1);

	dma_set_peripheral_address(DMA2, DMA_STREAM1, (uint32_t)&USART6_DR);
	
	dma_set_memory_address(DMA2, DMA_STREAM1, (uint32_t)buf);
	dma_set_memory_address_1(DMA2, DMA_STREAM1, (uint32_t)buf1);
	dma_set_number_of_data(DMA2, DMA_STREAM1, dma_read_length);
	dma_channel_select(DMA2, DMA_STREAM1, DMA_SxCR_CHSEL_5);
	dma_set_priority(DMA2, DMA_STREAM1, DMA_SxCR_PL_HIGH);
	dma_enable_double_buffer_mode(DMA2, DMA_STREAM1);
	
	dma_set_dma_flow_control(DMA2, DMA_STREAM1);
	dma_set_transfer_mode(DMA2, DMA_STREAM1, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
	dma_enable_memory_increment_mode(DMA2, DMA_STREAM1);
	dma_set_peripheral_size(DMA2, DMA_STREAM1, DMA_SxCR_PSIZE_8BIT);
	dma_set_memory_size(DMA2, DMA_STREAM1, DMA_SxCR_PSIZE_8BIT);
	
	dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM1);
	dma_enable_stream(DMA2, DMA_STREAM1);
}


void dma2_stream1_isr(void)
{
	if(dma_get_interrupt_flag(DMA2, DMA_STREAM1, DMA_TCIF))
	{
		dma_clear_interrupt_flags(DMA2, DMA_STREAM1, DMA_TCIF);
		
		if(dma_get_target(DMA2, DMA_STREAM1) == 0)
		{
			for(uint32_t i = 0; i < dma_read_length; i++)
			{
				//Serial3.push(buf1[i]);
				Serial1.put(buf1[i]);
				buf1[i] = 'A';
				
			}
		}
		else
		{
			for(uint32_t i = 0; i < dma_read_length; i++)
			{
				//Serial3.push(buf[i]);
				Serial1.put(buf[i]);
				buf[i] = 'B';
			}
		}
	}
	
	if(dma_get_interrupt_flag(DMA2, DMA_STREAM1, DMA_HTIF))
	{
		dma_clear_interrupt_flags(DMA2, DMA_STREAM1, DMA_HTIF);
	}
}


int main(void)
{
    clock_setup();
    
    Serial1.begin(115200);
    Serial3.begin(2'000'000);
    
    configure_as_output({PA, 8});
    set_pin({PA, 8}, LOW);
    
    for(uint32_t i = 0; i < dma_read_length; i++)
		buf[i] = '\0';
			
    dma_init();
    
    dma_read();
    
	while(true)
	{
		while(Serial3.available())
			Serial1.put(Serial3.read());
	}
}



