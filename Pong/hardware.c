/*
 * ======================================
 * Name: hardware.c
 * Author: Maya Hampton
 * Version: 1.0
 * Description: Functionality of all
 * methods required for hardware access
 * ======================================
 */

#include "hardware.h"

// write to 7 segment display
void write7Segment()
{
	printf("Writing to 7 Segment display\n");
}

// write to the LCD display based on input
void writeLCD(char text[])
{
	printf("Writing to LCD display: %s\n", text);
}

// a button was pressed
// logic for which button
void buttonInput()
{
	printf("Button was pressed\n");
}
