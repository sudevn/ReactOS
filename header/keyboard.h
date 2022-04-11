#ifndef __KEYBOARD_H
#define __KEYBOARD_H

        #include "types.h"
        #include "interrupts.h"
        #include "port.h"
        #include "std.h"
        #include "config.h"

    class KeyboardDriver : public InterruptHandler
    {
        Port8Bit dataport;
        Port8Bit commandport;
    public:
        KeyboardDriver(InterruptManager* manager);
        ~KeyboardDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
    };

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(manager, 0x21),      //1 is the isa irq for keyboard see https://wiki.osdev.org/Interrupts , so 32+1=33
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xae); // activate interrupts see https://wiki.osdev.org/%228042%22_PS/2_Controller
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;     //if success 1 so 1 & 11101111 = 1 if not success (0 | 1)&11101111=1 so whats the point
    commandport.Write(0x60); // command 0x60 = set controller command byte see above link
    dataport.Write(status);
    dataport.Write(0xf4);       //see https://wiki.osdev.org/PS2_Keyboard#Commands
}

KeyboardDriver::~KeyboardDriver()
{
}


uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
        uint8_t key = dataport.Read();
        if(key < 0x80)
        {
                printch(convscancode(key));
        }
        return esp;
}


#endif