#ifndef COMMON_H
#define COMMON_H

// variable types

typedef unsigned int  uint ;
typedef unsigned short ushort;
typedef unsigned char  uchar;

void outb(ushort port, uchar value);
uchar inb(ushort port);
ushort inw(ushort port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint line);
extern void panic_assert(const char *file, uint line, const char *description);

#endif // COMMON_H
