
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../address_map_arm.h"


/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

volatile unsigned int *JP1_ptr;    	// virtual address pointer to JP1 Expansion Port

/* 
	This program writes to HEX0 Display via GPIO to the BCD Decoder in the FPGA 
*/
int main(void)
{
	int fd = -1;               
	void *LW_virtual;
	int i;	
    
	// Create virtual memory access to the FPGA light-weight bridge and setup virtual address pointers to Expansion port
	if ((fd = open_physical (fd)) == -1)
		return (-1);
	if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
		return (-1);
	JP1_ptr = (unsigned int *) (LW_virtual + JP1_BASE);

	// Configure Expansion JP1 Parallel Port (lower 4 bits to be outputs)
	printf("BCD 7 Segment Decoder circuit test\r\n");
	*(JP1_ptr + 1) = 0x000000F;
	
	// Write decimal 0 thru 9 to BCD 7 Segment Decoder circuit using GPIO[0]-GPIO[3] pins
	printf("Writing 0 through 9 to the BCD 7 Segment Decoder circuit using GPIO[0]-GPIO[3] pins\r\n");
	for (i = 0; i < 10; i++)
	{
		*(JP1_ptr + 0) = i;
		printf("Wrote value of %d\r\n", i);
		usleep(2000 * 1000);
		
	}
	
	printf("Done with test\r\n");

	// Cleanup and return
 	unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   
	close_physical (fd); 
	return 0;
}

// Open /dev/mem, if not already done, to give access to physical addresses
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

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
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
 * Close the previously-opened virtual address mapping
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


