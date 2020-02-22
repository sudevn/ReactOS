#include "header/types.h"
#include "header/std.h"
#include "header/gdt.h"
#include "header/keyboard.h"
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

    printf(     "  _____ _    _          _____ _  __     \n");
    printf(     "/ ____| |  | |   /\\   / ____| |/ /      \n");
    printf(     " (___ | |__| |  /  \\ | |    | ' /       \n");
    printf(     " \\___ \\|  __  | / /\\ \\| |    |  <       \n");  
    printf(     "___) | |  | |/ ____ \\ |____| . \\        \n");
    printf(     "_____/|_|  |_/_/    \\_\\_____|_|\\_\\      \n");
       printf("Welcome to NIDOS operating system\nPlease enter a command\n");
       while (1)
       {
                printf("\nNIDOS> ");
                
                char * ch = readStr();
                printf(ch);
       }
     
        GlobalDescriptorTable gdt;
        InterruptManager interrupts(0x20, &gdt);
        interrupts.Activate();
    

}
