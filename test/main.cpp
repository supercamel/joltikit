#include "joltikit/joltikit.h"

using namespace etk;


int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);
    

    while(1)
    {
    	Serial1.print("Hello world!\r\n");
    }
}



