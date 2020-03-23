#include "header/types.h"
#include "header/std.h"
#include "header/gdt.h"
#include "header/keyboard.h"
#include "header/mouse.h"
#include "header/interrupts.h"




typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, unsigned int /*multiboot_magic*/)
{

    cls();

    // printf(     "  _____ _    _          _____ _  __     \n");
    // printf(     "/ ____| |  | |   /\\   / ____| |/ /      \n");
    // printf(     " (___ | |__| |  /  \\ | |    | ' /       \n");
    // printf(     " \\___ \\|  __  | / /\\ \\| |    |  <       \n");  
    // printf(     "___) | |  | |/ ____ \\ |____| . \\        \n");
    // printf(     "_____/|_|  |_/_/    \\_\\_____|_|\\_\\      \n");
    //    printf("Welcome to Sudev operating system\nPlease enter a command\n");
    //    while (1)
    //    {
    //             printf("\nSUDEV> ");
                
    //             char * ch = readStr();
    //             if (strEql(ch , "sudev") == 1)
    //             {
    //                 printf("\nyou are in god mode");
    //             }
    //             else if (strEql(ch , "shutdown") == 1)
    //             {
    //                 printf("\nGoodbye");
    //                 shutdown();
    //             }
    //             else if (strEql(ch , "restart") == 1)
    //             {
    //                 printf("\nSee you soon");
    //                 reboot();
    //             }
    //             else if (strEql(ch , "help") == 1)
    //             {
    //                 printf("\nFollowing Commands are available");
    //                 printf("\nshutdown  - what does it sound like");
    //                 printf("\nrestart   - give me an another chance to grow up once again");
    //                 printf("\ncls     - clean slate");
    //             }
    //             else if (strEql(ch , "cls") == 1)
    //             {
    //                 cls();
    //             }
    //             else 
    //             {
    //             printf("\nType help");
    //             printf("\nyou typed ");printf(ch);
    //             }
                
    //    }
    printf("Hello World with idt and gdt");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    printf("\n Started GDT ...");
    KeyboardDriver keyboard(&interrupts);
    printf("\n Started keyboard ...");
    MouseDriver mouse(&interrupts);
    printf("\n Started mouse ...");
    interrupts.Activate();
    while(1);
    

}
