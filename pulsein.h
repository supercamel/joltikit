#ifndef PULSE_IN_H
#define PULSE_IN_H

#include "gpio.h"
#include <etk/etk.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>


class PulseIn
{
public:
    void begin();

    void enable_channel(int chan, GPIO_BANK bank);

    uint32 get_channel(int chan)
    {
        return chan_time[chan];
    }

    bool available();

private:
    void process_isr(int channel);

    friend void exti0_isr(void);
    friend void exti1_isr(void);
    friend void exti2_isr(void);
    friend void exti3_isr(void);
    friend void exti4_isr(void);
    friend void exti5_isr(void);
    friend void exti9_5_isr(void);
    friend void exti15_10_isr(void);

    volatile GPIO_BANK channel_banks[16];
    volatile etk::Time chan_start[16];
    volatile uint32 chan_time[16];
    volatile etk::Time last_pulse_time;
    volatile bool last_chan_state[16];
};

extern PulseIn pwm_reader;




#endif


