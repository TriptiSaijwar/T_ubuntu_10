// kheap.h -- Interface for kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.

#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"

/**
   Allocate a chunk of memory, size in size. If align == 1,
   the chunk must be page-aligned. If phys != 0, the physical
   location of the allocated chunk will be stored into phys.

   This is the internal version of kmalloc. More user-friendly
   parameter representations are available in kmalloc, kmalloc_a,
   kmalloc_ap, kmalloc_p.
**/
uint kmalloc_int(uint size, int alignment, uint *phys);

/**
   Allocate a chunk of memory, size in size. The chunk must be
   page aligned.
**/
uint kmalloc_align(uint size);

/**
   Allocate a chunk of memory, size in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to uint!
**/
uint kmalloc_physloc(uint size, uint *phys);

/**
   Allocate a chunk of memory, size in size. The physical address 2
   is returned in phys. It must be page-aligned.
**/
uint kmalloc_align_physloc(uint size, uint *phys);

/**
   General allocation function.
**/
uint kmalloc(uint size);

#endif // KHEAP_H
