#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "system.h"

typedef struct page
{
    uint present    : 1;   // Page present in memory
    uint rw         : 1;   // Read-only if clear, readwrite if set
    uint user       : 1;   // Supervisor level only if clear
    uint accessed   : 1;   // Has the page been accessed since last refresh?
    uint dirty      : 1;   // Has the page been written to since last refresh?
    uint unused     : 7;   // Amalgamation of unused and reserved bits
    uint frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    /**
       Array of pointers to pagetables.
    **/
    page_table_t *tables[1024];
    /**
       Array of pointers to the pagetables above, but gives their *physical*
       location, for loading into the CR3 register.
    **/
    uint tablesPhysical[1024];

    /**
       The physical address of tablesPhysical. This comes into play
       when we get our kernel heap allocated and the directory
       may be in a different location in virtual memory.
    **/
    uint physicalAddr;
} page_directory_t;

/**
   Sets up the environment, page directories etc and
   enables paging.
**/
void initialise_paging();

/**
   Causes the specified page directory to be loaded into the
   CR3 register.
**/
void switch_page_directory(page_directory_t *new);

/**
   Retrieves a pointer to the page required.
   If make == 1, if the page-table in which this page should
   reside isn't created, create it!
**/
page_t *get_page(uint address, int make, page_directory_t *dir);

/**
   Handler for page faults.
**/
void page_fault(struct regs *r);

#endif
