/*
 * ======================================
 * Name: hardware.h
 * Author: Maya Hampton
 * Version: 1.0
 * Description: method signatures for
 * all hardware functions
 * ======================================
 */

// lcd
#include "LCD_Includes/terasic_os_includes.h"
#include "LCD_Includes/LCD_Lib.h"
#include "LCD_Includes/lcd_graphic.h"
#include "LCD_Includes/font.h"

// 7 segment
#include <linux/kernel.h>
#include "address_map_arm.h"

// push button


// LCD
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )


int increase7Segment(int count);
int writeLCD(char text[]);
int buttonPress();
int decimal_bcd(int decimal);

int open_physical (int fd);
void* map_physical(int fd, unsigned int base, unsigned int span);
void close_physical (int fd);
int unmap_physical(void * virtual_base, unsigned int span);

