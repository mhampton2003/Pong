/*
 * ======================================
 * Name: hardware.h
 * Author: Maya Hampton
 * Version: 3.0
 * Description: method signatures for
 * all hardware functions
 * ======================================
 */

// lcd includes
#include "LCD_Includes/terasic_os_includes.h"
#include "LCD_Includes/LCD_Lib.h"
#include "LCD_Includes/lcd_graphic.h"
#include "LCD_Includes/font.h"

// 7 segment includes
#include <linux/kernel.h>
#include "address_map_arm.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// LCD
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

// input output logic
int increase7Segment(int count);
int writeLCD();
int buttonPress();
int switchFlip();

// mapping
int open_physical (int fd);
void* map_physical(int fd, unsigned int base, unsigned int span);
void close_physical (int fd);
int unmap_physical(void * virtual_base, unsigned int span);

// drawing
int drawString();
int eraseBall(float x, float y);
int drawBall(float x, float y);
int drawPlatform(int x1, int y1, int x2, int y2);
int erasePlatform(int x1, int y1, int x2, int y2);
