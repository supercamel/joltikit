#include "joltikit/joltikit.h"
#include "lps25h.h"

using namespace etk;

LPS25H lps;


int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);

	configure_as_output({PC, 5});
	
	pwm_reader.begin();
	pwm_reader.enable_channel(1, PB);
	
	while(true)
	{
		Serial1.print((int)pwm_reader.get_channel(1), "\r\n");
		toggle_pin({PC, 5});
		etk::sleep_ms(200);
	}
}



