// kheap.c -- Kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
#include "kheap.h"

// end is defined in the linker script.
extern uint end;
uint place_addr = (uint)&end;

uint kmalloc_int(uint size, int alignment, uint *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at place_addr
    // and increment it by size. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if ((alignment == 1) && (place_addr & 0xFFFFF000) )
    {
        // align the placement address;
        place_addr &= 0xFFFFF000;
        place_addr += 0x1000;
    }
    if (phys)
    {
        *phys = place_addr;
    }
    uint temp = place_addr;
    place_addr += size;
    return temp;
}

uint kmalloc_align(uint size)
{
    return kmalloc_int(size, 1, 0);
}

uint kmalloc_physloc(uint size, uint *phys)
{
    return kmalloc_int(size, 0, phys);
}

uint kmalloc_align_physloc(uint size, uint *phys)
{
    return kmalloc_int(size, 1, phys);
}

uint kmalloc(uint size)
{
    return kmalloc_int(size, 0, 0);
}
