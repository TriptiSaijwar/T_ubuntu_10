#include "system.h"
#include "common.h"

void *memcpy(void *destination,const void *source,int counter)
{
    const char *src_ptr = (const char *)source;
    char *dest_ptr = (char *)destination;
    for(; counter != 0; counter--)
    {
	*dest_ptr++ = *src_ptr++;
    }
    return destination;
}

void *memset ( void *destination, char value, int counter)
{

    char *temp = (char *)destination;
    for( ; counter != 0; counter--) 
    {
	*temp++ = value ;
    }
    return destination;
}

unsigned short *memsetw(unsigned short *destination,unsigned short value,int counter)
{

    unsigned short *temp = (unsigned short *)destination;
    for( ; counter != 0; counter--) 
    {
	*temp++ = value;
    }
    return destination;
}

int strlen(const char *str)
{
    int number;
    for(number = 0; *str != '\0'; str++) 
    {
	number++;
    }
    return number;
}

unsigned char inport (unsigned short _port)
{

   unsigned char ch ;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (ch) : "dN" (_port));
  
  return ch;
}

void outport (unsigned short _port, unsigned char _data)
{

    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));

}

void main()
{
     gdt_install();
     idt_install();
     isrs_install();
     irq_install();
     kb_install();

     __asm__ __volatile__ ("sti");
     init_video();
     settextcolor(9,0);
     puts("Welcome to T_Ubuntu\n");
/*
int i;
    i = 10/0;
   putch(i/0+'0');
*/
    initialise_paging();
    settextcolor(2,0);
    puts("Hello, paging world!\n");
    char *x = kmalloc(100);
    x[0] = 'c';
    //putch(x[0]);

    for (;;);

}
		
