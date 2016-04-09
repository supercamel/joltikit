#ifndef LPS25H_INCLUDED
#define LPS25H_INCLUDED

#include "joltikit/joltikit.h"


class LPS25H
{
public:
    bool initialise();

    bool requestPressure();
    bool requestTemperature();
    float readPressureMillibars(void);
    int32 readPressureRaw(void);
    float readTemperature(void);
    int16 readTemperatureRaw(void);

    bool get_healthy() { return healthy; }

private:
	uint8 read_reg(uint8 reg);
	void write_reg(uint8 reg, uint8 value);
	bool read_bytes(uint8 reg, uint8* buf, uint8 num);
	
    bool healthy = true;
    uint8 pout[3];
    uint8 tout[2];
};

#endif


