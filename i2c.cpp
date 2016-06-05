#include "i2c.h"

#include "usart.h"

void SoftI2C::begin()
{
    configure_as_output(SDA);
    configure_as_output(SCL);

    stop();
}


bool SoftI2C::transmit(uint8 slave, uint8* out, uint32 olen, uint8* in, uint32 ilen)
{
    if(olen)
    {
        start();
        if(!write_byte(slave<<1))
        {
            stop();
            return false;
        }

        for(uint32 i = 0; i < olen; i++)
        {
            if(!write_byte(out[i]))
            {
                stop();
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

    return true;
}

void SoftI2C::start()
{
    configure_as_output(SDA);
    configure_as_output(SCL);

    set_pin(SDA, HIGH);
    etk::sleep_us(2);
    set_pin(SCL, HIGH);
    etk::sleep_us(2);
    set_pin(SDA, LOW);
    etk::sleep_us(2);
    set_pin(SCL, LOW);
    etk::sleep_us(2);
}

void SoftI2C::stop()
{
    configure_as_output(SDA);

    set_pin(SDA, LOW);
    etk::sleep_us(2);

    set_pin(SCL, HIGH);
    etk::sleep_us(2);
    set_pin(SDA, HIGH);
    etk::sleep_us(2);
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
    etk::sleep_us(2);
    set_pin(SCL, HIGH);
    etk::sleep_us(2);

    bool ack = !read_pin(SDA);
    etk::sleep_us(2);
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

    etk::sleep_us(2);
    bits.set_bit(7, read_pin(SDA));
    etk::sleep_us(2);
    configure_as_output(SCL);
    set_pin(SCL, LOW);
    etk::sleep_us(4);

    for(uint8 i = 1; i < 8; i++)
    {
        set_pin(SCL, HIGH);
        etk::sleep_us(2);
        bits.set_bit(7-i, read_pin(SDA));
        etk::sleep_us(2);
        set_pin(SCL, LOW);
        etk::sleep_us(4);
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
    etk::sleep_us(2);
    set_pin(SCL, HIGH);
    etk::sleep_us(4);
    set_pin(SCL, LOW);
    etk::sleep_us(2);
}

SoftI2C i2c1({PB, 9}, {PB, 8});

