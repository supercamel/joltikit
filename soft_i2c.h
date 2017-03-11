#ifndef SOFT_I2C_H
#define SOFT_I2C_H


#include "gpio.h"


/**
 * This is a software I2C implementation. I can't figure out how to use the flaming hardware with opencm3.
 * Personally, I don't intend to use DMA or interrupts so there's no great need for me to use hardware I2C.
 * Polling an I2C register takes just as long as reading/writing digital IOs.
 */


class SoftI2C
{
public:
    SoftI2C()
    { }

    void set_timeout(uint32 t)
    {
        timeout = t;
    }

    void begin(gpio_pin sda, gpio_pin scl);
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
	void small_delay();

    gpio_pin SDA;
    gpio_pin SCL;

    uint32 timeout = 10;
};


#endif


