#include "paging.h"
#include "kheap.h"
#include "common.h"
#include "system.h"

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
uint *frames;
uint nframes;

// Defined in kheap.c
extern uint place_addr;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(uint frame_addr)
{

    uint frame = frame_addr / 0x1000;
    uint index = INDEX_FROM_BIT(frame);
    uint offset = OFFSET_FROM_BIT(frame);
    frames[index] |= (0x1 << offset);
}

// Static function to clear a bit in the frames bitset
static void clear_frame ( uint frame_addr)
{
    uint frame = frame_addr / 0x1000;
    uint index = INDEX_FROM_BIT(frame);
    uint offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~(0x1 << offset);
}

// Static function to test if a bit is set.
static uint test_frame ( uint frame_addr)
{
    uint frame = frame_addr / 0x1000;
    uint index = INDEX_FROM_BIT(frame);
    uint offset = OFFSET_FROM_BIT(frame);
    return (frames[index] & (0x1 << offset));
}

// Static function to find the first free frame.
static uint first_frame()
{
    uint i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {
            // at least one bit is free here.
            for (j = 0; j < 32; j++)
            {
                uint toTest = 0x1 << j;
                if ( !(frames[i]&toTest) )
                {
                    return i*4*8+j;
                }
            }
        }
    }
}

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        uint index = first_frame();
        if (index == (uint)-1)
        {
            // PANIC! no free frames!!
        }
        set_frame(index*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = index;
    }
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    uint frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void initialise_paging()
{
    // The size of physical memory. For the moment we 
    // assume it is 16MB big.
    uint mem_end_page = 0x1000000;
    
    nframes = mem_end_page / 0x100;
    frames = (uint*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_align(sizeof(page_directory_t));
    current_directory = kernel_directory;

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change place_addr
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    int i = 0;
    while (i < place_addr + 0x1000000)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Before we enable paging, we must register our page fault handler.
    irq_install_handler(14, page_fault);
    // Now, enable paging!
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    __asm__ __volatile__("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint cr0;
    __asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    __asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else 
    if(make)
    {
        uint temp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_align_physloc(sizeof(page_table_t), &temp);
        dir->tablesPhysical[table_idx] = temp | 0x7; // PRESENT, RW, US.
        return (&dir->tables[table_idx]->pages[address%1024]);
    }
    else
    {
        return 0;
    }
}


void page_fault(struct regs *r)
{
	puts("ghus gaya\n");
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint faulting_address;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(r->error_code & 0x1); // Page not present
    int rw = r->error_code & 0x2;           // Write operation?
    int us = r->error_code & 0x4;           // Processor was in user-mode?
    int reserved = r->error_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = r->error_code & 0x10;          // Caused by an instruction fetch?
 
    // Output an error message.
    puts("Page fault! ( ");
    if (present) 
    {
	puts("present ");
    }
    if (rw) 
    {
	puts("read-only ");
    }
    if (us)
    {
     	puts("user-mode ");
    }
    if (reserved) 
    {
	 puts("reserved ");
    }
    puts(") at 0x");
  //puts_hex(faulting_address); 
    puts("\n");
    PANIC("Page fault");
}
