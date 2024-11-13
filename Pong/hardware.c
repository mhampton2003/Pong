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
int writeLCD(char text[])
{

	void *virtual_base;
	int fd;

	LCD_CANVAS LcdCanvas;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	LcdCanvas.Width = LCD_WIDTH;
	LcdCanvas.Height = LCD_HEIGHT;
	LcdCanvas.BitPerPixel = 1;
	LcdCanvas.FrameSize = LcdCanvas.Width * LcdCanvas.Height / 8;
	LcdCanvas.pFrame = (void *)malloc(LcdCanvas.FrameSize);

	if (LcdCanvas.pFrame == NULL){
			printf("failed to allocate lcd frame buffer\r\n");
	}

	else{

		LCDHW_Init(virtual_base);
		LCDHW_BackLight(true); // turn on LCD backlight

		LCD_Init();

		// clear screen
		DRAW_Clear(&LcdCanvas, LCD_WHITE);

		// x, y, width, height
		DRAW_Rect(&LcdCanvas, 90, 60, 45, 55, LCD_BLACK); // rectangle
		DRAW_Circle(&LcdCanvas, 10, 10, 4, LCD_BLACK); // circle

		DRAW_Refresh(&LcdCanvas);

		DRAW_Clear(&LcdCanvas, LCD_WHITE);

		free(LcdCanvas.pFrame);
	}

	// clean up our memory mapping and exit

	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	printf("Writing to LCD display: %s\n", text);

	return( 0 );
}

// a button was pressed
// logic for which button
void buttonInput()
{
	printf("Button was pressed\n");
}
