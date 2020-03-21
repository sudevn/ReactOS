#ifndef __MOUSE_H
#define __MOUSE_H

    #include "types.h"
    #include "port.h"
    #include "interrupts.h"

    class MouseDriver : public InterruptHandler
    {
        Port8Bit dataport;
        Port8Bit commandport;
        uint8_t buffer[3];
        uint8_t offset;
        uint8_t buttons;

        int8_t x, y;
    public:
        MouseDriver(InterruptManager* manager);
        ~MouseDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
    };

MouseDriver::MouseDriver(InterruptManager* manager)
    : InterruptHandler(manager, 0x2C),  //12 is the isa irq for mouse see https://wiki.osdev.org/Interrupts , so 32+12=44
    dataport(0x60),
    commandport(0x64)
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;     //for cursor part
        offset = 0;
        buttons = 0;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);   //till here

        commandport.Write(0xA8);    //Enable the device see (https://wiki.osdev.org/Mouse_Input)
        commandport.Write(0x20); // command 0x20 = read controller command byte
        uint8_t status = dataport.Read() | 2;
        commandport.Write(0x60); // command 0x60 = set controller command byte
        dataport.Write(status);

        commandport.Write(0xD4);
        dataport.Write(0xF4);
        dataport.Read();
    }

    MouseDriver::~MouseDriver()
    {
    }

    uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
    {
        uint8_t status = commandport.Read();
        if (!(status & 0x20))
            return esp;

        buffer[offset] = dataport.Read();
        offset = (offset + 1) % 3;          //(0+1)%3=1 , 2 , 0 ,0, 1,........(% is for modulas that is the remainder)

        if(offset == 0)
        {
            if(buffer[1] != 0 || buffer[2] != 0)        //buffer1 is x movment and the 2 is y movement here we are checking if there is any movment
            {
                static uint16_t* VideoMemory = (uint16_t*)0xb8000;
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);

                x += buffer[1];     //x = x + buffer(1)
                if(x >= 80) x = 79; //more that video memory bring back to video memory
                if(x < 0) x = 0;    //same in case of negetive
                y -= buffer[2];     //y = y - buffer(2)  here it is subtract because if we go down mouse sent negetive signal so inorder it to be positive we inverse it
                if(y >= 25) y = 24; //same logic as x axis
                if(y < 0) y = 0;

                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);
            }

            /*
            for(uint8_t i = 0; i < 3; i++)
            {
                if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
                {
                    if(buttons & (0x1<<i))
                        handler->OnMouseButtonReleased(i+1);
                    else
                        handler->OnMouseButtonPressed(i+1);
                }
            }
            buttons = buffer[0];
            */
        }
        return esp;
    }

#endif