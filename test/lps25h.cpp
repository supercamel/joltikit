#include "lps25h.h"

#define REF_P_XL        0x08
#define REF_P_L         0x09
#define REF_P_H         0x0A
#define WHO_AM_I        0x0F
#define RES_CONF        0x10
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define INT_CFG         0x24
#define INT_SRC         0x25
#define STATUS_REG      0x27
#define PRESS_OUT_XL  0x28
#define PRESS_OUT_L   0x29
#define PRESS_OUT_H   0x2A
#define TEMP_OUT_L      0x2B
#define TEMP_OUT_H      0x2C
#define FIFO_CTRL       0x2E
#define FIFO_STATUS     0x2F
#define THS_P_L         0x30
#define THS_P_H         0x31
#define RPDS_L          0x39
#define RPDS_H          0x3A

#define LPS25H_WHO_ID   0xBD
#define LPS25H_ADDR_H   0x5D


bool LPS25H::initialise()
{
    int whoami = read_reg(WHO_AM_I);
    if(whoami != LPS25H_WHO_ID)
    {
        healthy = false;
        return false;
    }

    write_reg(CTRL_REG1, 0b11000000);
    write_reg(CTRL_REG2, 0b11100000);
    write_reg(FIFO_CTRL, 0b11011111);
    write_reg(RES_CONF, 0b00001111);

    return true;
}

bool LPS25H::requestPressure()
{
    return read_bytes(PRESS_OUT_XL | (1 << 7), pout, 3);
}

bool LPS25H::requestTemperature()
{
    return read_bytes(TEMP_OUT_L | (1 << 7), tout, 2);
}

float LPS25H::readPressureMillibars()
{
    return (float)readPressureRaw() / 4096.0f;
}

int32 LPS25H::readPressureRaw(void)
{
    return (int32)pout[2] << 16 | (uint16)pout[1] << 8 | pout[0];
}

float LPS25H::readTemperature(void)
{
    return 42.5f + (float)readTemperatureRaw() / 480.0f;
}

int16 LPS25H::readTemperatureRaw(void)
{
    return (int16)(tout[1] << 8 | tout[0]);
}

uint8 LPS25H::read_reg(uint8 reg)
{
	uint8 ret;
	if(!i2c1.transmit(LPS25H_ADDR_H, &reg, 1, &ret, 1))
	{
		i2c1.end();
		i2c1.begin();
	}
	
	return ret;
}

void LPS25H::write_reg(uint8 reg, uint8 value)
{
	uint8 out[2] = {reg, value};
	
	if(!i2c1.transmit(LPS25H_ADDR_H, out, 2, nullptr, 0))
	{
		i2c1.end();
		i2c1.begin();
	}
}

bool LPS25H::read_bytes(uint8 reg, uint8* buf, uint8 num)
{
	if(!i2c1.transmit(LPS25H_ADDR_H, &reg, 1, buf, num))
	{
		i2c1.end();
		i2c1.begin();
		
		healthy = false;
		return false;
	}
	return true;
}



