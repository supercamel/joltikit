#include "joltikit/joltikit.h"
#include "LSM9DS0.h"

SoftI2C si2c({PB, 9}, {PB, 8});
LSM9DS0_G lsmg(&si2c);
using namespace etk;



int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);

	
	while(true)
	{
		Serial1.print("Hello world!\r\n");
		etk::sleep_ms(100);
	}
}



