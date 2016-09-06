#ifndef JOLTIKIT_I2C_INCLUDED
#define JOLTIKIT_I2C_INCLUDED


#include "gpio.h"


class I2C
{
public:
    I2C(gpio_pin sda, gpio_pin scl) : SDA(sda), SCL(scl)
    { }

    void set_timeout(uint32 t)
    {
        timeout = t;
    }

    void begin();
    void end() { }

    bool transmit(uint8 slave, uint8* out, uint32 olen, uint8* in, uint32 ilen);

    void start();
    void stop();

    bool write_byte(uint8 b);
    uint8 read_byte();

    void clock_pulse();

    void send_ack();
    void send_nack();

private:


    gpio_pin SDA;
    gpio_pin SCL;

    uint32 timeout = 10;
};


extern I2C i2c1;


#endif
