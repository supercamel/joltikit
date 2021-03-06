GCC_BIN =
PROJECT = joltikit


CPP_SOURCES=$(wildcard ../EmbeddedToolKit/src/*.cpp) $(wildcard ../*.cpp) $(wildcard *.cpp)
C_SOURCES=$(wildcard ../FreeRTOS/*.c) $(wildcard ../FreeRTOS/portable/GCC/ARM_CM4F/*.c)
OBJECTS=$(foreach x, $(basename $(C_SOURCES)), $(x).o)
OBJECTS+=$(foreach x, $(basename $(CPP_SOURCES)), $(x).o)

SYS_OBJECTS =
INCLUDE_PATHS = -I. -I../ -I../../ -I../EmbeddedToolKit/inc/ -I../FreeRTOS/include \
				-I../FreeRTOS/portable/GCC/ARM_CM4F
LIBRARY_PATHS =
LIBRARIES =
LINKER_SCRIPT = stm32f415.ld

###############################################################################
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE 	= $(GCC_BIN)arm-none-eabi-size

CPU = -mthumb -mcpu=cortex-m4 -mfloat-abi=$(FLOAT_ABI) -mfpu=fpv4-sp-d16
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -ffunction-sections -fwrapv -Os
CC_FLAGS += -MMD -MP
CC_SYMBOLS = -DSTM32F4
CPP_FLAGS = -fno-exceptions -felide-constructors -fno-rtti -std=c++14 -Wall -ffunction-sections -fwrapv

LD_FLAGS = $(CPU) -Wl,--gc-sections -u _printf_float -u _scanf_float
LD_FLAGS += -Wl,-Map=$(PROJECT).map,--cref -L../libopencm3/ -L../
LD_SYS_LIBS = -lm -lopencm3_stm32f4 -lnosys -lgcc -lc


FLOAT_ABI = hard


ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

all: $(PROJECT).bin $(PROJECT).hex
	@echo "The rosy roses of success burgers!"

clean:
	@rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(OBJECTS) $(DEPS)
	@echo "Project is now squeeky clean :-)"

.s.o:
	$(AS) $(CPU) -o $@ $<

.c.o:
	@$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	@$(CPP) $(CC_FLAGS) $(CPP_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	@$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES) $(LD_SYS_LIBS) $(LIBRARIES) $(LD_SYS_LIBS)
	$(SIZE) $@

$(PROJECT).bin: $(PROJECT).elf
	@$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size:
	$(SIZE) $(PROJECT).elf

monitor:
	./terminal
	#screen /dev/ttyUSB0 57600

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)

upload:
	stm32flash -e 1 -b 115200 -w $(PROJECT).bin -v -g 0x0 /dev/ttyUSB0
