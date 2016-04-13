#include "pulsein.h"


uint32 gpio_n_from_bank(GPIO_BANK bank);

PulseIn pwm_reader;


void PulseIn::begin()
{
	rcc_periph_clock_enable(RCC_SYSCFG);
	nvic_enable_irq(NVIC_EXTI0_IRQ);
	nvic_enable_irq(NVIC_EXTI1_IRQ);
	nvic_enable_irq(NVIC_EXTI2_IRQ);
	nvic_enable_irq(NVIC_EXTI3_IRQ);
	nvic_enable_irq(NVIC_EXTI4_IRQ);
    nvic_enable_irq(NVIC_EXTI9_5_IRQ);
    nvic_enable_irq(NVIC_EXTI15_10_IRQ);
}

void PulseIn::enable_channel(int chan, GPIO_BANK bank)
{
	configure_as_input({bank, chan});
	
	channel_banks[chan] = bank;
	
	//http://www2.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf
	//refer page 294
	
	if(chan >= 12)
	{
		auto c = chan-12;
		SYSCFG_EXTICR4 |= static_cast<int>(bank)+(c*4);
	}
	else if(chan >= 8)
	{
		auto c = chan-8;
		SYSCFG_EXTICR3 |= static_cast<int>(bank)+(c*4);
	}
	else if(chan >= 4)
	{
		auto c = chan-4;
		SYSCFG_EXTICR2 |= static_cast<int>(bank)+(c*4);
	}
	else
	{
		SYSCFG_EXTICR1 |= static_cast<int>(bank)+(chan*4);
	}
	
	last_chan_state[chan] = read_pin({bank, chan});
	exti_select_source((1 << chan), gpio_n_from_bank(bank));
    exti_set_trigger((1 << chan), EXTI_TRIGGER_BOTH);
    exti_enable_request((1 << chan));
}


void PulseIn::process_isr(int chan)
{
	bool chan_state = read_pin({channel_banks[chan], chan});
    if(chan_state != last_chan_state[chan])
    {
        last_chan_state[chan] = chan_state;
        exti_reset_request((1 << chan));
        if(chan_state)
        {
            chan_start[chan] = etk::now();
        }
        else
        {
            chan_time[chan] = etk::now().diff_time(chan_start[chan])*1000000;
            if((chan_time[chan] > 500) && (chan_time[chan] < 2500))
                last_pulse_time = etk::now();
        }
    }
}

void exti0_isr(void)
{
	pwm_reader.process_isr(0);
}

void exti1_isr(void)
{
	pwm_reader.process_isr(1);
}

void exti2_isr(void)
{
	pwm_reader.process_isr(2);
}

void exti3_isr(void)
{
	pwm_reader.process_isr(3);
}

void exti4_isr(void)
{
	pwm_reader.process_isr(4);
}

void exti9_5_isr(void)
{
	for(int i = 5; i <= 9; i++)
	{
		bool chan_state = read_pin({pwm_reader.channel_banks[i], i});
		if(chan_state != pwm_reader.last_chan_state[i])
			pwm_reader.process_isr(i);
    }
}

void exti15_10_isr(void)
{
	for(int i = 10; i <= 15; i++)
	{
		bool chan_state = read_pin({pwm_reader.channel_banks[i], i});
		if(chan_state != pwm_reader.last_chan_state[i])
			pwm_reader.process_isr(i);
    }
}


