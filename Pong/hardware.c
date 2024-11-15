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

void close_physical (int fd)
{
   close (fd);
}

int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}

// write to 7 segment display
int increase7Segment(int count)
{
	void * LW_virtual;         // Lightweight bridge base address
	volatile int *HEX_ptr;
	int fd = -1;

	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	HEX_ptr = (unsigned int *) (LW_virtual + HEX3_HEX0_BASE);

	*HEX_ptr = decimal_bcd(count);

	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
	close_physical (fd);   // close /dev/mem

	return 0;

}

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

int initLCD() {

	return 0;
}

// write to the LCD display based on input
int writeLCD(char text[])
{
	int count = 0;
	int platformWidth = 40;
	int platformHeight = 55;
	int platformX = 90;
	int platformY = 60;

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

		DRAW_Rect(&LcdCanvas, 90, 60, 45, 55, LCD_BLACK); // rectangle
		//DRAW_Circle(&LcdCanvas, 10, 10, 4, LCD_BLACK); // circle+
		DRAW_Refresh(&LcdCanvas);

		int radius = 4;
		float x = LcdCanvas.Width / 2;
		float y = LcdCanvas.Height / 2;
		float vx = 100.0;
		float vy = 50.0;

		clock_t lastTime = clock();
		int running = 1;

		increase7Segment(0);
		while(running)
		{

			clock_t currentTime = clock();
			double elapsedTime = (double)(currentTime - lastTime) / CLOCKS_PER_SEC; // Convert to seconds
			lastTime = currentTime;

			DRAW_Circle(&LcdCanvas, x, y, radius, LCD_WHITE);
			DRAW_Refresh(&LcdCanvas);

			x += vx * elapsedTime * 10;
			y += vy * elapsedTime * 10;

			// Ensure the ball stays within bounds
			if (x - radius < 0)
			{
				x = radius; // Correct position if it hits left wall
				vx = -vx;
			}
			if (x + radius > LcdCanvas.Width)
			{
				x = LcdCanvas.Width - radius; // Correct position if it hits right wall
				vx = -vx;
			}
			if (y - radius < 0)
			{
				y = radius; // Correct position if it hits top wall
				vy = -vy;
			}
			if (y + radius > LcdCanvas.Height)
			{
				y = LcdCanvas.Height - radius; // Correct position if it hits bottom wall
				vy = -vy;

				//DRAW_Clear(&LcdCanvas, LCD_WHITE);
				//DRAW_Refresh(&LcdCanvas);

				DRAW_PrintString(&LcdCanvas, 40, 5, "GAME", LCD_BLACK, &font_16x16);
				DRAW_PrintString(&LcdCanvas, 40, 5+16, "OVER", LCD_BLACK, &font_16x16);

				DRAW_Refresh(&LcdCanvas);

				free(LcdCanvas.pFrame);

				return 1;
			}

			if ((y + radius > platformHeight) && (x + radius < platformX) && (x - radius > platformWidth))
			{
				y = platformHeight - radius;// Correct position if it hits top of platform
				vy = -vy;
				count = count + 1;
				increase7Segment(count);
			}

			DRAW_Circle(&LcdCanvas, x, y, radius, LCD_BLACK); // circle+
			DRAW_Refresh(&LcdCanvas);
			usleep(100000);

			if ((buttonPress() == 2) || (buttonPress() == 3))
			{
				DRAW_Rect(&LcdCanvas, platformX, platformY, platformWidth, platformHeight, LCD_WHITE);
				platformX = platformX - 10;
				platformWidth = platformWidth - 10;
				DRAW_Rect(&LcdCanvas, platformX, platformY, platformWidth, platformHeight, LCD_BLACK);
				DRAW_Refresh(&LcdCanvas);
				usleep(500000);
			}
			if ((buttonPress() == 0) || (buttonPress() == 1))
			{
				DRAW_Rect(&LcdCanvas, platformX, platformY, platformWidth, platformHeight, LCD_WHITE);
				platformX = platformX + 10;
				platformWidth = platformWidth + 10;
				DRAW_Rect(&LcdCanvas, platformX, platformY, platformWidth, platformHeight, LCD_BLACK);
				DRAW_Refresh(&LcdCanvas);
				usleep(500000);
			}
		}
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
int buttonPress()
{

	void * LW_virtual;
	volatile int* KEY_ptr;     // virtual address for the KEY port

	int fd = -1;

	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	KEY_ptr = LW_virtual + KEY_BASE;    // init virtual address for KEY port


		// You can add additional logic here to handle key presses
		// For example, check if a specific key is pressed
		if (*KEY_ptr & 0x1) {
			//printf("Key 0 pressed\n");
			return 0;
		}
		if (*KEY_ptr & 0x2) {
			//printf("Key 1 pressed\n");
			return 1;
		}
		if (*KEY_ptr & 0x4) {
			//printf("Key 2 pressed\n");
			return 2;
		}
		if (*KEY_ptr & 0x8) {
			//printf("Key 3 pressed\n");
			return 3;
		}

		usleep(200000);


	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
	close_physical (fd);

	return -1;
}
