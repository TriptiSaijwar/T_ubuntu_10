#ifndef __MAIN_H
#define __MAIN_H

/* MAIN.C */
extern void* memcpy(void* destination, const void* source, int counter);
extern void* memset(void* destination, unsigned char value, int counter);
extern unsigned short* memsetw(unsigned short* destination, unsigned short value, int counter);
extern unsigned char inport(unsigned short _port);
extern void outport(unsigned short _port, unsigned char _data);

#endif
