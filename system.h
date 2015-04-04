#ifndef __SYSTEM_H
#define __SYSTEM_H

/* MAIN.C */
extern void *memcpy(void *destination, const void *source, int counter);
extern void *memset(void *destination, char value, int counter);
extern unsigned short *memsetw(unsigned short *destination, unsigned short value, int counter);
extern int strlen(const char *str);
extern unsigned char inport(unsigned short _port);
extern void outport(unsigned short _port, unsigned char _data);
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();
extern void isrs_install();
/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, error_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();
extern void kb_install();
int inst_length;
unsigned char instruction[20];
char arr[100][15];
int curr_dir;
int direct_no[10];
int size;
int shift_flag;
#endif
