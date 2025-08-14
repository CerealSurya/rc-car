# Compiler and flags
CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p
OBJCOPY = avr-objcopy
MCU = atmega328p

# File lists
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = Main

# Default target
all: $(TARGET).hex

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Link all object files
$(TARGET).bin: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Generate HEX file
%.hex: %.bin
	$(OBJCOPY) -O ihex -j .text -j .data -R .eeprom $< $@

# Upload
.PHONY: deploy
deploy:
	avrdude -F -V -c arduino -p m328p -P /dev/tty.usbserial-B0012B8B -b 115200 -U flash:w:$(TARGET).hex

# Clean build files
.PHONY: clean
clean:
	rm -f *.o *.bin *.hex