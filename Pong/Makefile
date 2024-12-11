#
TARGET = Pong2
ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= $(SOCEDS_DEST_ROOT)
HWLIBS_ROOT = $(SOCEDS_ROOT)/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -static -g -Wall   -D$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY)   -I$(HWLIBS_ROOT)/include/ 
LDFLAGS =  -g -Wall  
CC = $(CROSS_COMPILE)gcc
ARCH= arm


build: $(TARGET)


$(TARGET): main.o hardware.o app.o LCD_Includes/LCD_Lib.o LCD_Includes/terasic_lib.o LCD_Includes/LCD_Driver.o LCD_Includes/LCD_Hw.o LCD_Includes/lcd_graphic.o LCD_Includes/font.o
	$(CC) $(LDFLAGS)   $^ -o $@   -lrt -lm
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~ *.bmp
.PHONY: all
all: $(TARGET)
