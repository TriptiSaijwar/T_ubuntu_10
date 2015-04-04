#include "system.h"


unsigned char keyboard[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',  '9', '0', '-', '=', '\b', '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',  'm', ',', '.', '/',   0,  '*',   0,  ' ', 0, 0, 0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0,	'+', 0,	 0,	 0, 0,	 0,	0,   0,   0, 0,	0, 0, };


unsigned char keyboard_shift[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',  '(', ')', '_', '+', '\b', '\t','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	0,'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',   0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',  'M', '<', '>', '?',   0,  '*',   0,  ' ', 0, 0, 0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0,	'+', 0,	 0,	 0, 0,	 0,	0,   0,   0, 0,	0, 0, };


/* keyboard interrupt */
void kb_handle(struct regs *r)
{
    unsigned char scancode;

    /*reads from the keyboard */
    scancode = inport(0x60);
    /* check whther key is released */
    if (scancode & 0x80)
    {
	/*** shift key is released ***/
        if(scancode == 170)     
		{
			shift_flag = 0;
		}
    }
    else
    {
		if(keyboard[scancode] != '\b' && keyboard[scancode] != 0)
		{
			if(shift_flag == 1)
				instruction[inst_length] = keyboard_shift[scancode];
			else
        		instruction[inst_length] = keyboard[scancode];
			inst_length += 1;
		}
		/*** shift key is pressed ***/
		if(scancode == 42)
		{
			shift_flag = 1;
		}
		if(shift_flag == 1)
			putch(keyboard_shift[scancode]);
		else
        	putch(keyboard[scancode]);
		if(keyboard[scancode] == '\n')
		{
			if(inst_length >= 7 && instruction[0] == 'm' && instruction[1] == 'k' && instruction[2] == 'd')
			{
				mkdir(instruction,inst_length);
			}
			else if(inst_length >= 7 && instruction[0] == 'm' && instruction[1] == 'k' && instruction[2] == 'f')
			{
				mkfil(instruction,inst_length);
			}
			else if(inst_length >=4 && instruction[0] == 'c' && instruction[1] == 'd')
			{
				cd(instruction,inst_length);
			}
			else if(inst_length >=2 && instruction[0] == 'l' && instruction[1] == 's')
			{
				ls();
			}
			else if(inst_length >=4 && instruction[0] == 'r' && instruction[1] == 'm')
			{
				rm(instruction,inst_length);
			}
			inst_length = 0;
		}
		else if(keyboard[scancode] == '\b')
		{
			putch(' ');
			putch('\b');
			instruction[inst_length-1] = ' ';
			inst_length -= 1;
		}
    }
}

/* Installs the keyboard handler */
void kb_install()
{
    irq_install_handler(1, kb_handle);
}
