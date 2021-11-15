TARGET = $(notdir $(CURDIR))
SOURCES_S = $(wildcard *.s)
SOURCES_S_O = $(SOURCES_S:.s=.o)
SOURCES_C = $(wildcard *.c)
SOURCES_C_O = $(SOURCES_C:.c=.o)
SOURCES_CPP = $(wildcard *.cpp)
SOURCES_CPP_O = $(SOURCES_CPP:.cpp=.o)
OBJECTS = $(SOURCES_S:.s=.o) $(SOURCES_C:.c=.o) $(SOURCES_CPP:.cpp=.o)

extern_ard_o = $(wildcard _libs/arduino/*.o)

INC_DIR = -I_libs/arduino 


MCU = atmega2560
USBDEV = /dev/ttyACM0


#ifeq ($(shell test -e $(USBDEV)),yes)
#    USBDEV=$(USBDEV)
#else
#    USBDEV=$(FILE1) /dev/ttyACM1
#endif

BAUD = 19200

AS = avr-as
CC = avr-gcc
CPP = avr-g++
LD = avr-ld
OBJDUMP = avr-objdump


CFLAGS = -g -Os -mmcu=$(MCU) -DF_CPU=16000000UL  $(INC_DIR) 
CPPFLAGS = -g -Os -mmcu=$(MCU) -DF_CPU=16000000UL  $(INC_DIR) 
ASFLAGS = -g -Os -mmcu=$(MCU) -DF_CPU=16000000UL $(INC_DIR) 


AVRDUDE = avrdude
AVRDUDE_CONF = /etc/avrdude.conf

all: $(TARGET).hex


$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(SOURCES_S_O) $(SOURCES_C_O) $(SOURCES_CPP_O) 
	$(CC) $(CFLAGS) $(OBJECTS) $(extern_ard_o) -o $@ 


$(SOURCES_S_O):
	$(AS)  $(ASFLAGS)   $*.s      -o $@

$(SOURCES_C_O):
	$(CC)  $(CFLAGS)    $*.c   -c -o $@

$(SOURCES_CPP_O):
	$(CPP) $(CPPFLAGS)  $*.cpp -c -o $@


upload: $(TARGET).hex
	$(AVRDUDE) -C $(AVRDUDE_CONF)   -p $(MCU) -c stk500v1 -P $(USBDEV) -b $(BAUD) -D -U flash:w:$(TARGET).hex:i
	

dump: $(TARGET).elf
	$(OBJDUMP) -d $(TARGET).elf

	
upload_eeprom:
	$(AVRDUDE) -C $(AVRDUDE_CONF) -p $(MCU) -c stk500v1 -P $(USBDEV) -b $(BAUD) -D -U eeprom:w:$(TARGET).eep:h

read_eeprom:	
	$(AVRDUDE) -C $(AVRDUDE_CONF) -p $(MCU) -c stk500v1 -P $(USBDEV) -b $(BAUD) -D -U eeprom:r:readfile_eep.eep:h

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(OBJECTS)

.PHONY: clean upload dump

