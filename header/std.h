#ifndef __STD_H
#define __STD_H


#include "sys.h"
#include "config.h"

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
        case ('\b'):
            if(cursorX > 0)
            {
	        cursorX--;
                VideoMemory[((cursorY * sw + cursorX))] = (VideoMemory[((cursorY * sw + cursorX))] & 0xFF00) | '\0';
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

void shutdown()
{                                                     //temp shutdown code
    outportw(0x4004, 0x3400);
}

void reboot()
{                                                      //temp restart code
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inportb(0x64);
    outportb(0x64, 0xFE);
    asm volatile ("hlt");
}

char convscancode(uint8_t scancode)
{
        switch(scancode)
                        {
                        /*case 1:
                            printch('(char)27);           Escape button
                            buffstr[i] = (char)27;
                            i++;
                            break;*/
                    case 2:
                            return '1';
                            break;
                    case 3:
                            return '2';
                            break;
                    case 4:
                            return '3';
                            break;
                    case 5:
                            return '4';
                            break;
                    case 6:
                            return '5';
                            break;
                    case 7:
                            return '6';
                            break;
                    case 8:
                            return '7';
                            break;
                    case 9:
                            return '8';
                            break;
                    case 10:
                            return '9';
                            break;
                    case 11:
                            return '0';
                            break;
                    case 12:
                            return '-';
                            break;
                    case 13:
                            return '=';
                            break;
                    case 14:
                            return '\b';
                            break;
                        /* case 15:
                            printch('\t');          Tab button
                            buffstr[i] = '\t';
                            i++;
                            break;*/
                    case 16:
                            return 'q';
                            break;
                    case 17:
                            return 'w';
                            break;
                    case 18:
                            return 'e';
                            break;
                    case 19:
                            return  'r';
                            break;
                    case 20:
                            return  't';
                            break;
                    case 21:
                            return 'y';
                            break;
                    case 22:
                            return 'u';
                            break;
                    case 23:
                            return 'i';
                            break;
                    case 24:
                            return 'o';
                            break;
                    case 25:
                            return 'p';
                            break;
                    case 26:
                            return '[';
                            break;
                    case 27:
                            return ']';
                            break;
                    case 28:
                            return '\n';
                            break;
                        /*  case 29:
                            printch('q');           Left Control
                            buffstr[i] = 'q';
                            i++;
                            break;*/
                    case 30:
                            return 'a';
                            break;
                    case 31:
                            return 's';
                            break;
                    case 32:
                            return 'd';
                            break;
                    case 33:
                            return 'f';
                            break;
                    case 34:
                            return 'g';
                            break;
                    case 35:
                            return 'h';
                            break;
                    case 36:
                            return 'j';
                            break;
                    case 37:
                            return 'k';
                            break;
                    case 38:
                            return 'l';
                            break;
                    case 39:
                            return ';';
                            break;
                    case 40:
                            return (char)44;               //   Single quote (')
                            break;
                    case 41:
                            return (char)44;               // Back tick (`)
                            break;
                        /* case 42:                                 Left shift
                            printch('q');
                            buffstr[i] = 'q';
                            i++;
                            break;
                    case 43:                                 \ (< for somekeyboards)
                            printch((char)92);
                            buffstr[i] = 'q';
                            i++;
                            break;*/
                    case 44:
                            return 'z';
                            break;
                    case 45:
                            return 'x';
                            break;
                    case 46:
                            return 'c';
                            break;
                    case 47:
                            return 'v';
                            break;
                    case 48:
                            return 'b';
                            break;
                    case 49:
                            return 'n';
                            break;
                    case 50:
                            return 'm';
                            break;
                    case 51:
                            return ',';
                            break;
                    case 52:
                            return '.';
                            break;
                    case 53:
                            return '/';
                            break;
                    case 54:
                            return '.';
                            break;
                    case 55:
                            return '/';
                            break;
                        /*case 56:
                            printch(' ');           Right shift
                            buffstr[i] = ' ';
                            i++;
                            break;*/
                    case 57:
                            return ' ';
                            break;

                        default:
                            {
                            char* foo = "KEYBOARD 0x00 ";
                            char* hex = "0123456789ABCDEF";
                            foo[11] = hex[(scancode >> 4) & 0xF];
                            foo[12] = hex[scancode & 0xF];
                            printf(foo);
                            return '\0';
                            break;
                            }
                        }
}

// char * readstr()
// {
//         char * buffstr;
//         uint8_t i = 0;
//                 void readKeyboard(char InputKeyboard)
//         while (reading == "True")
//         {
//         printch(InputKeyboard);
//                if (mode == "Terminal")
//                {
//                        if (input == '\n')
//                        {
//                                reading = "False";
//                        }

//                }

//         }


// }


#endif

