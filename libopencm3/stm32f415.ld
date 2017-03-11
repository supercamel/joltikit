/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of RAM */

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
	ccmram (xrw) : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Include the common ld script. */
INCLUDE libopencm3_stm32f4.ld
