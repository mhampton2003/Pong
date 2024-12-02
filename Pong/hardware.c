/*
 * ======================================
 * Name: hardware.c
 * Author: Maya Hampton
 * Version: 2.0
 * Description: Functionality of all
 * methods required for hardware access
 * ======================================
 */

#include "hardware.h"

int platformWidth = 40;
int platformHeight = 55;
int platformX = 90;
int platformY = 60;

LCD_CANVAS LcdCanvas;

/*
 * opens memory space for register access
 */
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}

/*
 * maps physical addresses to virtual addresses
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

/*
 * closes the physical space that registers accessed
 */
void close_physical (int fd)
{
   close (fd);
}

/*
 * unmaps the virtual addresses the physical addresses were mapped to
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}

/*
 * display value on the 7-segment display based on input
 */
int increase7Segment(int count)
{
	void * LW_virtual;
	volatile int *HEX_ptr;
	int fd = -1;

	// opens and maps physical addresses to virtual addresses
	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	// initializes HEX pointer to turn on far right display
	HEX_ptr = (unsigned int *) (LW_virtual + HEX3_HEX0_BASE);


	// uses BCD decoder to display value on the far right display
	// if count is bigger than 9, multiple displays are needed
	// calculates each value separately
	if (count >= 10)
	{
		int x = count % 10;
		int y = count / 10;
		*HEX_ptr = decimal_bcd(x) | (decimal_bcd(y) << 8);
	}

	// only use one display
	else{

		*HEX_ptr = decimal_bcd(count);
	}

	//*HEX_ptr = decimal_bcd(count);
	// unmaps and closes memory
	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
	close_physical (fd);

	return 0;

}

/*
 * decimal to BCD decoder
 * returns BCD value based on decimal input
 */
int decimal_bcd(int decimal)
{
	switch (decimal)
	{
	case 0:
		return 0x3f;
	case 1:
		return 0x06;
	case 2:
		return 0x5b;
	case 3:
		return 0x4f;
	case 4:
		return 0x66;
	case 5:
		return 0x6d;
	case 6:
		return 0x7d;
	case 7:
		return 0x07;
	case 8:
		return 0x7f;
	case 9:
		return 0x67;
	default:
		return 0xff;
	}

}

/*
 * draws a rectangle at input position to move platform
 */
int drawPlatform(int x1, int y1, int x2, int y2)
{
	// 90 60 45 55
	DRAW_Rect(&LcdCanvas, x1, y1, x2, y2, LCD_BLACK);
	DRAW_Refresh(&LcdCanvas);

	return 0;
}

/*
 * draws a rectangle in white at input position to remove platform at previous position
 */
int erasePlatform(int x1, int y1, int x2, int y2)
{
	// 90 60 45 55
	DRAW_Rect(&LcdCanvas, x1, y1, x2, y2, LCD_WHITE);
	DRAW_Refresh(&LcdCanvas);

	return 0;
}

/*
 * draws a circle at input position to move ball
 */
int drawBall(float x, float y)
{
	DRAW_Circle(&LcdCanvas, x, y, 4, LCD_BLACK);
	DRAW_Refresh(&LcdCanvas);

	return 0;
}

/*
 * draws a circle in white at input position to remove ball at previous position
 */
int eraseBall(float x, float y)
{
	DRAW_Circle(&LcdCanvas, x, y, 4, LCD_WHITE);
	DRAW_Refresh(&LcdCanvas);

	return 0;
}

/*
 * prints text to the LCD screen
 */
int drawString(int count)
{
	DRAW_PrintString(&LcdCanvas, 40, 5, "GAME", LCD_BLACK, &font_16x16);
	DRAW_PrintString(&LcdCanvas, 40, 5+16, "OVER", LCD_BLACK, &font_16x16);
	DRAW_Refresh(&LcdCanvas);

	return 0;
}

/*
 * initilize the LCD screen and draw initial platform and ball
 */
int writeLCD()
{

	void *virtual_base;
	int fd;

	// open space for physical address
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	// map physcial address to virtual address
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	// initialize canvas attributes
	LcdCanvas.Width = LCD_WIDTH;
	LcdCanvas.Height = LCD_HEIGHT;
	LcdCanvas.BitPerPixel = 1;
	LcdCanvas.FrameSize = LcdCanvas.Width * LcdCanvas.Height / 8;
	LcdCanvas.pFrame = (void *)malloc(LcdCanvas.FrameSize);

	if (LcdCanvas.pFrame == NULL){
			printf("failed to allocate lcd frame buffer\r\n");
	}

	else{

		// initialize and turn on LCD
		LCDHW_Init(virtual_base);
		LCDHW_BackLight(true); // turn on LCD backlight

		LCD_Init();

		DRAW_Clear(&LcdCanvas, LCD_WHITE);

		// turn on 7-segment display and set to 0
		increase7Segment(0);

		// draw initial placement of platform and ball
		DRAW_Rect(&LcdCanvas, 90, 60, 45, 55, LCD_BLACK);
		DRAW_Circle(&LcdCanvas, LcdCanvas.Width / 2, LcdCanvas.Height / 4, 4, LCD_BLACK);
		DRAW_Refresh(&LcdCanvas);
	}

	return(0);
}

/*
 * checks to see which button was pressed
 */
int buttonPress()
{

	void * LW_virtual;
	// virtual address for the KEY port
	volatile int* KEY_ptr;

	int fd = -1;

	// opens and maps physical address to virtual address
	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	// init virtual address for KEY port
	KEY_ptr = LW_virtual + KEY_BASE;

	// returns the button number based on which one was pressed
	if (*KEY_ptr & 0x1) {
		return 0;
	}
	if (*KEY_ptr & 0x2) {
		return 1;
	}
	if (*KEY_ptr & 0x4) {
		return 2;
	}
	if (*KEY_ptr & 0x8) {
		return 3;
	}

	usleep(200000);

	// release the physical-memory mapping
	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
	close_physical (fd);

	return -1;
}
