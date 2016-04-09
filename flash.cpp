#include "flash.h"
#include <libopencm3/stm32/flash.h>

const int FLASH_PAGE_SIZE = 0x800;
const int FLASH_PAGE_NUM_MAX = 812;


uint32 flash_program_data(uint32 start_address, uint8 *input_data, uint16 num_elements)
{
	uint16 iter;
    uint32 current_address = start_address;

    start_address += 0x080E0000;
    
    flash_unlock();
	flash_wait_for_last_operation();
	
    for(iter=0; iter<num_elements; iter += 4)
    {
        flash_program_word(current_address+iter, *((uint32_t*)(input_data + iter)));
        flash_wait_for_last_operation();

        if(*((uint32_t*)(current_address+iter)) != *((uint32_t*)(input_data + iter)))
            return 0x80;
    }

    return 0;
}


void flash_read_data(uint32 start_address, uint16 num_elements, uint8 *output_data)
{
    uint16 iter;
    uint32 *memory_ptr= (uint32*)(start_address+0x080E0000);

    for(iter=0; iter<num_elements/4; iter++)
    {
        *(uint32*)output_data = *(memory_ptr + iter);
        output_data += 4;
    }
}

