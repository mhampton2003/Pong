/*
 * ======================================
 * Name: hardware.h
 * Author: Maya Hampton
 * Version: 1.0
 * Description: method signatures for
 * all hardware functions
 * ======================================
 */

#include "terasic_os_includes.h"
#include "LCD_Lib.h"
#include "lcd_graphic.h"
#include "font.h"

// LCD
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )


void write7Segment();
int writeLCD(char text[]);
void buttonInput();
