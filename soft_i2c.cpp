#include "soft_i2c.h"
#include "FreeRTOS.h"
#include "task.h"

void SoftI2C::begin(gpio_pin sda, gpio_pin scl)
{
	SDA = sda;
	SCL = scl;
    configure_as_output(SDA);
    configure_as_output(SCL);

    stop();
}


bool SoftI2C::transmit(uint8 slave, uint8* out, uint32 olen, uint8* in, uint32 ilen)
{
	vTaskSuspendAll();
    if(olen)
    {
        start();
        if(!write_byte(slave<<1))
        {
            stop();
            xTaskResumeAll();
            return false;
        }

        for(uint32 i = 0; i < olen; i++)
        {
            if(!write_byte(out[i]))
            {
                stop();
                xTaskResumeAll();
                return false;
            }
        }
    }
    if(ilen)
    {
        start();
        write_byte((slave << 1) | 1);

        uint32 i = 0;
        for(i = 0; i < ilen-1; )
        {
            in[i++] = read_byte();
            send_ack();
        }

        in[i] = read_byte();
        send_nack();
    }

    stop();
	xTaskResumeAll();
    return true;
}

void SoftI2C::start()
{
    configure_as_output(SDA);
    configure_as_output(SCL);

    set_pin(SDA, HIGH);
    small_delay();
    set_pin(SCL, HIGH);
    small_delay();
    set_pin(SDA, LOW);
    small_delay();
    set_pin(SCL, LOW);
    small_delay();
}

void SoftI2C::stop()
{
    configure_as_output(SDA);

    set_pin(SDA, LOW);
    small_delay();

    set_pin(SCL, HIGH);
    small_delay();
    set_pin(SDA, HIGH);
    small_delay();
}

bool SoftI2C::write_byte(uint8 b)
{
    configure_as_output(SDA);

    for(int i = 8; i; i--)
    {
        set_pin(SDA, (b&0x80));

        b <<= 1;

        clock_pulse();
    }

    configure_as_input(SDA);
    small_delay();
    set_pin(SCL, HIGH);
    small_delay();

    bool ack = !read_pin(SDA);
    small_delay();
    set_pin(SCL, LOW);
	
    return ack;
}

uint8 SoftI2C::read_byte()
{
    configure_as_input(SDA);
    configure_as_input(SCL);

    auto start = etk::now();
    while(!read_pin(SCL))
    {
        if(etk::now().diff_time_ms(start) > timeout)
            return 0;
    }

    etk::Bits<uint8> bits(0);

    small_delay();
    bits.set_bit(7, read_pin(SDA));
    small_delay();
    configure_as_output(SCL);
    set_pin(SCL, LOW);
    small_delay();

    for(uint8 i = 1; i < 8; i++)
    {
        set_pin(SCL, HIGH);
        small_delay();
        bits.set_bit(7-i, read_pin(SDA));
        small_delay();
        set_pin(SCL, LOW);
        small_delay();
        small_delay();
    }

    return bits.get();
}

void SoftI2C::send_ack()
{
    configure_as_output(SDA);
    set_pin(SDA, LOW);
    clock_pulse();
}

void SoftI2C::send_nack()
{
    configure_as_output(SDA);
    set_pin(SDA, HIGH);

    clock_pulse();
}

void SoftI2C::clock_pulse()
{
    small_delay();
    set_pin(SCL, HIGH);
    small_delay();
    small_delay();
    set_pin(SCL, LOW);
    small_delay();
}

void SoftI2C::small_delay()
{
	for(int i = 0; i < 10; i++)
		asm("nop");
}


