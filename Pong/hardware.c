/*
 * ======================================
 * Name: hardware.c
 * Author: Maya Hampton
 * Version: 5.0
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
 * increases the score by 1 using the counter circuit and displays on 7-seg display
 */

int increase7Segment()
{
	void * LW_virtual;
	volatile unsigned int *JP1_ptr; // virtual address pointer to JP1 Expansion Port
	int fd = -1;

	// opens and maps physical addresses to virtual addresses
	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	// pointer to access GPIO pins using JP1
	JP1_ptr = (unsigned int *) (LW_virtual + JP1_BASE);

	// set GPIO[0] as output
	*(JP1_ptr + 1) |= (1 << 0);


	// sends high pulse to GPIO[0]
	*JP1_ptr |= (1 << 0);

	usleep(50);

	// sends low pulse to GPIO[0]
	*JP1_ptr &= ~(1 << 0);

	return 0;
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
 * initialize the LCD screen and draw initial platform and ball
 */
int initLCD()
{

	void *virtual_base;
	int fd;

	// open space for physical address
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	// map physical address to virtual address
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

		// draw initial placement of platform and ball
		DRAW_Rect(&LcdCanvas, 90, 60, 45, 55, LCD_BLACK);
		DRAW_Circle(&LcdCanvas, LcdCanvas.Width / 2, LcdCanvas.Height / 6, 4, LCD_BLACK);
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
	int fd = -1;
	volatile int* KEY_ptr; 			// virtual address for the KEY port

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

/*
 * check to see if the switch has been flipped
 */
int switchFlip()
{
	void * LW_virtual;
	int fd = -1;
	volatile int* SW_ptr;			// virtual address for the SW port

	static int initialized = 0;  // tracks first-time initialization
	static uint32_t prev_state = 0; // stores previous switch state

	// opens and maps physical address to virtual address
	if ((fd = open_physical (fd)) == -1)
	   return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);

	// init virtual address for SW port
	SW_ptr = LW_virtual + SW_BASE;

	// Read the current state of the switches
	uint32_t curr_state = *SW_ptr;

	// If it's the first time running, set prev_state to current state and return 0
	if (!initialized) {
		prev_state = curr_state;
		initialized = 1;  // Mark as initialized

		// unmap memory
		unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
		close_physical(fd);
		return 0;  // No change detected on first run
	}

	// Check if switch state (SW0 (using 0x1 mask)) has changed
	if ((curr_state & 0x1) != (prev_state & 0x1)) {
		prev_state = curr_state;  // Update previous state

		// unmap memory
		unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
		close_physical(fd);
		return 1;  // SW0 changed
	}

	usleep(200000);

	return -1;
}
