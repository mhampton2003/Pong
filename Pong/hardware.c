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



	//printf("Can you see LCD?(CTRL+C to terminate this program)\r\n");
	printf("Graphic LCD Demo\r\n");

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

			// demo grphic api
		DRAW_Rect(&LcdCanvas, 0,0, LcdCanvas.Width-1, LcdCanvas.Height-1, LCD_BLACK); // retangle
		DRAW_Circle(&LcdCanvas, 10, 10, 6, LCD_BLACK);
		DRAW_Circle(&LcdCanvas, LcdCanvas.Width-10, 10, 6, LCD_BLACK);
		DRAW_Circle(&LcdCanvas, LcdCanvas.Width-10, LcdCanvas.Height-10, 6, LCD_BLACK);
		DRAW_Circle(&LcdCanvas, 10, LcdCanvas.Height-10, 6, LCD_BLACK);

		// demo font
		DRAW_PrintString(&LcdCanvas, 40, 5, "I", LCD_BLACK, &font_16x16);
		DRAW_PrintString(&LcdCanvas, 40, 5+16, "LOVE", LCD_BLACK, &font_16x16);
		DRAW_PrintString(&LcdCanvas, 40, 5+32, "Cookies", LCD_BLACK, &font_16x16);

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
