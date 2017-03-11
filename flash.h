#ifndef FLASH_H_INCLUDED
#define FLASH_H_INCLUDED

#include <stdint.h>
#include <etk/etk.h>

constexpr uint32_t FLASH_OFFSET = 0x080F0000;

void flash_read_data(uint32_t start, uint32_t len, unsigned char* b);
uint32_t flash_program_data(uint32_t start, unsigned char* b, uint32_t len);


template <uint32 N> uint32 flash_program_data(uint32_t start_address, etk::StaticString<N>& ss)
{
    return flash_program_data(start_address + FLASH_OFFSET, (uint8*)ss.c_str(), ss.length()+1);
}


template <uint32 N> void flash_read_data(uint32_t start_address, uint16 num_elements, etk::StaticString<N>& ss)
{
    int8 *memory_ptr= (int8*)start_address + FLASH_OFFSET;

    int i = 0;
    for(i = 0; i < num_elements; i++)
        ss[i] = *(memory_ptr + i);
    ss[i] = '\0';
}


#endif
