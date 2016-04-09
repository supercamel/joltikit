#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <etk/etk.h>

enum : uint8 {
    PA,
    PB,
    PC,
    PD
} typedef GPIO_BANK;


struct gpio_pin
{
    gpio_pin() { }

    gpio_pin(GPIO_BANK bank, int pin) : bank(bank), pin(pin)
    {

    }

    GPIO_BANK bank;
    int pin;
};


enum : uint8 {
    INPUT_FLOAT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_ANALOG
} typedef PIN_INPUT_TYPE;

const bool HIGH = true;
const bool LOW = false;

void set_pin(gpio_pin pin, bool high);
void toggle_pin(gpio_pin pin);
bool read_pin(gpio_pin pin);

uint32 pulse_in(gpio_pin pin);

void configure_as_input(gpio_pin pin, PIN_INPUT_TYPE pull = INPUT_FLOAT);
void configure_as_output(gpio_pin pin);


#endif


