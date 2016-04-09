#include "joltikit/joltikit.h"
#include "lps25h.h"

using namespace etk;

LPS25H lps;


int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);
    Serial2.begin(9600);
    
	etk::sleep_ms(2000);
	
	etk::StaticString<128> ss = "Hello world!";
	flash_program_data(0, ss);
	
	ss = "";
	flash_read_data(0, 128, ss);
	
	while(true)
	{
		Serial1.print(ss, "\r\n");
	}
}



