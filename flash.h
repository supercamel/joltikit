#ifndef FLASH_H_INCLUDED
#define FLASH_H_INCLUDED

#include <etk/etk.h>


void flash_read_data(int start, unsigned int len, unsigned char* b);
uint32 flash_program_data(int start, unsigned char* b, unsigned int len);




template <uint32 N> uint32 flash_program_data(uint32 start_address, etk::StaticString<N>& ss)
{
    return flash_program_data(start_address + 0x080E0000, (uint8*)ss.c_str(), ss.length()+1);
}


template <uint32 N> void flash_read_data(uint32 start_address, uint16 num_elements, etk::StaticString<N>& ss)
{
    int8 *memory_ptr= (int8*)start_address + 0x080E0000;

    int i = 0;
    for(i = 0; i < num_elements; i++)
        ss[i] = *(memory_ptr + i);
    ss[i] = '\0';
}


#endif


