#include "system.h"

/* define textpointer,background and foreground colors (attributes), and x and y cursor coordinates */
unsigned short *text_mem_ptr;
int attribute = 0x0F;
int cursor_x = 0, cursor_y = 0;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined */
    blank = 0x20 | (attribute << 8);

    /* Row 25 is the end */
    if(cursor_y >= 25)
    {
        temp = cursor_y - 25 + 1;
        memcpy (text_mem_ptr, text_mem_ptr + temp * 80, (25 - temp) * 80 * 2);

        memsetw (text_mem_ptr + (25 - temp) * 80, blank, 80);
        cursor_y = 25 - 1;
    }
}

void move_cursor(void)
{
    unsigned temp;

    /* equation for finding the index*/
    temp = cursor_y * 80 + cursor_x;
    /** to make cursor blink ***/
    outport(0x3D4, 14);
    outport(0x3D5, temp >> 8);
    outport(0x3D4, 15);
    outport(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /*  represent a space with color */
    blank = 0x20 | (attribute << 8);

    /* Set the entire screen with current color */
    for(i = 0; i < 25; i++)
        memsetw (text_mem_ptr + i * 80, blank, 80);

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attribute << 8;

    /*  backspace */
    if(c == 0x08)
    {
        if(cursor_x != 0) cursor_x--;
    }
    /* tab */
    else if(c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    }
    else if(c == '\r')
    {
        cursor_x = 0;
    }
    /*newline */
    else if(c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }

    // character
    else if(c >= ' ')
    {
        where = text_mem_ptr + (cursor_y * 80 + cursor_x);
        *where = c | att;	/* Character AND attributes: color */
        cursor_x++;
    }

    /* cursor has reached the edge of the screen's width */
    if(cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}

/** prints a string **/
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Set the forecolor and backcolor*/
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attribute = (backcolor << 4) | (forecolor & 0x0F);
}

void init_video(void)
{
    text_mem_ptr = (unsigned short *)0xB8000;
    cls();
}
		
