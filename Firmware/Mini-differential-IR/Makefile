PROG?=	MiniLedSensor
TARGET_CPU?= attiny85

CXX= avr-g++
CC=	avr-gcc
LD= avr-gcc
CPPFLAGS+=	-DOUTPUT_DIGITAL
CPPFLAGS+=	-DOUTPUT_ACTIVE_LOW
CPPFLAGS+=	-DDEBUG_UART
#CPPFLAGS+=	-DDEBUG_PWM
CPPFLAGS+=	-DBAUD_RATE=115200
CPPFLAGS+=	-DF_CPU=8000000uL
CFLAGS= -Wall -g -mmcu=$(TARGET_CPU) -Os -fpack-struct -fshort-enums
CXXFLAGS=
LDFLAGS=
OBJCOPY?=	avr-objcopy
OBJDUMP?=	avr-objdump
SIZE?=	avr-size

AVRDUDE?=	avrdude
AVRDUDE_PROGRAMMER?=	usbasp
AVRDUDE_TARGET?=	attiny85
AVRDUDE_PORT?=
AVRDUDE_BAUD?=
AVRDUDE_BITCLOCK?=	10

FUSE_L=0xe2
FUSE_H=0xdf
FUSE_E=0xff

AVRDUDE_CMD:= $(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_TARGET) $(addprefix -P ,$(AVRDUDE_PORT)) $(addprefix -b ,$(AVRDUDE_BAUD)) $(addprefix -B ,$(AVRDUDE_BITCLOCK))

all: $(PROG).hex $(PROG).lss size

$(PROG).elf: BBUart.o uart_send.o uart_print.o uart_format.o MiniLedSensor.o

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c $<
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
%.elf: %.o
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
%.eep: %.elf
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings $< $@
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

size: $(PROG).elf
	@echo
	@$(SIZE) -C --mcu=$(TARGET_CPU) $<

fuses:
	$(AVRDUDE_CMD) -U lfuse:w:$(FUSE_L):m -U hfuse:w:$(FUSE_H):m -U efuse:w:$(FUSE_E):m 
upload: $(PROG).hex size
	$(AVRDUDE_CMD) -U flash:w:$(PROG).hex:i

clean:
	rm -f $(PROG).o $(PROG).elf $(PROG).hex $(PROG).eep $(PROG).lss *.o
