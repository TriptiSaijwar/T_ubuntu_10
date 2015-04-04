#include "common.h"
#include "system.h" 

// Write a byte out to the specified port.
void outb(ushort port, uchar value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}

uchar inb(ushort port)
{
    uchar ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

ushort inw(ushort port)
{
    ushort ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from source to destination.
/*
void memcpy(uchar *destination, const uchar *source, uint len)
{
    const uchar *sp = (const uchar *)source;
    uchar *dp = (uchar *)destination;
    for(; len != 0; len--) *dp++ = *sp++;
}
*/
// Write len copies of val into destination.
/*
void memset(uchar *destination, uchar val, uint len)
{
    uchar *temp = (uchar *)destination;
    for ( ; len != 0; len--) *temp++ = val;
}
*/
// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string source into destination, and
// return destination.
char *strcpy(char *destination, const char *source)
{
    do
    {
      *destination++ = *source++;
    }
    while (*source != 0);
}

// Concatenate the NULL-terminated string source onto
// the end of destination, and return destination.
char *strcat(char *destination, const char *source)
{
    while (*destination != 0)
    {
        *destination = *destination++;
    }

    do
    {
        *destination++ = *source++;
    }
    while (*source != 0);
    return destination;
}

void panic(const char *message, const char *file, uint line)
{
    // We encountered a massive problem and have to stop.
    //asm volatile("cli"); // Disable interrupts.
    __asm__ __volatile__ ("cli");
    puts("PANIC(");
    puts(message);
    puts(") at ");
    puts(file);
    puts(":");
    puts(line);
    puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, uint line, const char *description)
{
    // An assertion failed, and we have to panic.
    //asm volatile("cli"); // Disable interrupts.
    __asm__ __volatile__ ("cli");
    puts("ASSERTION-FAILED(");
    puts(description);
    puts(") at ");
    puts(file);
    puts(":");
   // puts_dec(line);
    puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
