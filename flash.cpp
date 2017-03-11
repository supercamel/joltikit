#include "flash.h"
#include <libopencm3/stm32/flash.h>

const int FLASH_PAGE_SIZE = 0x800;
const int FLASH_PAGE_NUM_MAX = 812;

uint32_t flash_program_data(uint32_t start_address, unsigned char* input_data, uint32_t num_elements)
{
    uint32_t iter;
    start_address += FLASH_OFFSET;
    uint32_t current_address = start_address;

    flash_unlock();
    flash_wait_for_last_operation();
    flash_clear_status_flags();
    flash_erase_sector(11, 2);

    for(iter=0; iter<num_elements; iter += 4)
    {
        flash_program_word(current_address+iter, *((uint32_t*)(input_data + iter)));
        flash_wait_for_last_operation();

        if(*((uint32_t*)(current_address+iter)) != *((uint32_t*)(input_data + iter)))
            return 0x80;
    }

    flash_lock();

    return 0;
}


void flash_read_data(uint32_t start_address, uint32_t num_elements, unsigned char *output_data)
{
    uint32_t iter;
    uint32_t *memory_ptr= (uint32_t*)(start_address+FLASH_OFFSET);

    for(iter=0; iter<num_elements/4; iter++)
    {
        *(uint32_t*)output_data = *(memory_ptr + iter);
        output_data += 4;
    }
}
