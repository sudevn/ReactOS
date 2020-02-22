#ifndef __STD_H
#define __STD_H


#include "sys.h"
unsigned char cursorX = 0, cursorY = 0;
const unsigned char sw = 80,sh = 25,sd = 2;       



void update_coursor()
{
    unsigned temp;
    temp = cursorY * 80 + cursorX;                                                      //current value of cursor
    outportb(0x3D4, 14);                                                                // CRT Control Register: Select Cursor Location
    outportb(0x3D5, temp >> 8);                                                         // Send the high byte across the bus
    outportb(0x3D4, 15);                                                                // CRT Control Register: Select Send Low byte
    outportb(0x3D5, temp);                                                              // Send the Low byte of the cursor location

}


void cls()
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int i = 0; i != (80*2*25); ++i) //80 coloumns 2 bytes for every column and 25 hight of vga
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | '\0';
        cursorX = 0;
        cursorY = 0;
        update_coursor();
}
void cls(unsigned char from, unsigned char to )
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(unsigned char i = 80*from*2; i != 80*(to+1)*25; ++i)                                     //80 coloumns 2 bytes for every column and 25 hight of vga
        VideoMemory[i] = 0x0;
}

void scrollUp(unsigned char lineNumber)
{
        static unsigned short* VideoMemory = (unsigned short*)0xb8000;
        unsigned short i = 0;
        for (i;i<2*(25-1)*80;i++)
        {
                VideoMemory[i] = VideoMemory[i+80*2*lineNumber];
        }
        cls(25-1-lineNumber,25-1);
        if((cursorY - lineNumber) < 0 ) 
        {
                cursorY = 0;
                cursorX = 0;
        } 
        else 
        {
                cursorY -= lineNumber;
        }
        update_coursor();
}

void newLineCheck()
{
        if(cursorY >=25-1)
        {
                scrollUp(1);
        }
}


void printch(char str)
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;

    switch(str)
    {
        case (0x08):
            if(cursorX > 0) 
            {
	             cursorX--;									
                VideoMemory[(cursorY * sw + cursorX)*sd]=0x00;	                              
	        }
	        break;
     
                
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        case (0x20):
                cursorX++;
                break;
        default:
                VideoMemory[((cursorY * sw + cursorX))] = (VideoMemory[((cursorY * sw + cursorX))] & 0xFF00) | str;
                cursorX++; 
                break;
    }
    if(cursorX >= sw)                                                                   
    {
        cursorX = 0;                                                                
        cursorY++;                                                                    
    }
    newLineCheck();
    update_coursor();
}

unsigned short strlength(char * ch)
{
        unsigned short i = 1;
        while(ch[i++]);  
        return --i;
}

void printf(char * ch)
{
        unsigned short str = strlength(ch);
        unsigned short i = 0;
        for(i;i < str;i++)
        {
                printch(ch[i]);
        }
}

uint8_t strEql(char * ch1,char * ch2)                     
{
        uint8_t result = 1;
        uint8_t size = strlength(ch1);
        if(size != strlength(ch2)) result =0;
        else 
        {
        uint8_t i = 0;
        for(i;i<=size;i++)
        {
                if(ch1[i] != ch2[i]) result = 0;
        }
        }
        return result;
}

#endif

