#include "joltikit/joltikit.h"

using namespace etk;


int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);
    
	configure_as_input({PB, 7}, INPUT_PULLDOWN);
	while(true)
	{
		if(read_pin({PB, 7}))
			Serial1.print("up\r\n");
		else
			Serial1.print("down\r\n");
	}
}



