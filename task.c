

#include "task.h"
#include "paging.h"
volatile task_t *current_task;

volatile task_t *ready_queue;
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern u32int initial_esp;
extern u32int read_eip();

u32int next_pid = 1;

void initialise_tasking()
{
    asm volatile("cli");

    // Relocate the stack so we know where it is.
    move_stack((void*)0xE0000000, 0x2000);

    // Initialise the first task (kernel task)
    current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    // Reenable interrupts.
    asm volatile("sti");
}

void move_stack(void *new_stack_start, u32int size)
{
  u32int i;
  // Allocate some space for the new stack.
  for( i = (u32int)new_stack_start;
       i >= ((u32int)new_stack_start-size);
       i -= 0x1000)
  {
    alloc_frame( get_page(i, 1, current_directory), 0 /* User mode */, 1 /* Is writable */ );
  }
  
  // Flush the TLB by reading and writing the page directory address again.
  u32int pd_addr;
  asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
  asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

  // Old ESP and EBP, read from registers.
  u32int old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  u32int old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  u32int offset            = (u32int)new_stack_start - initial_esp;

  u32int new_stack_pointer = old_stack_pointer + offset;
  u32int new_base_pointer  = old_base_pointer  + offset;

  // Copy the stack.
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  // Backtrace through the original stack, copying new values into the new stack.  
  for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  {
    u32int tmp = * (u32int*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer and remap it. This will unfortunately remap ANY value in //this range, whether they are base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      u32int *tmp2 = (u32int*)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void switch_task()
{
    // If we haven't initialised tasking yet, just return.
    if (!current_task)
        return;

    // Read esp, ebp now for saving later on.
    u32int esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    /*  (a) We called the function and it returned the EIP as requested.
       (b) We have just switched tasks, and because the saved EIP is essentially the instruction after read_eip(), it will seem as if read_eip has just
           returned.
     In the second case we need to return immediately. To detect it we put a dummy
    value in EAX further down at the end of this function. As C returns values in EAX, it will look like the return value is this dummy value! (0x12345).*/
    eip = read_eip();

  
    if (eip == 0x12345)
        return;
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;
    
    // Get the next task to run.
    current_task = current_task->next;
    if (!current_task) current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    current_directory = current_task->page_directory;
     /* Stop interrupts
      Temporarily puts the new EIP location in ECX.
      Loads the stack and base pointers from the new task struct.
      Changes page directory to the physical address (physicalAddr) of the new directory.
      Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just switched task.
      Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after the next instruction.
      Jumps to the location in ECX (remember we put the new EIP in there).*/
    asm volatile("         \
      cli;                 \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      sti;                 \
      jmp *%%ecx           "
                 : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

int fork()
{
    asm volatile("cli");

    // Taking a pointer to this process' task struct
    task_t *parent_task = (task_t*)current_task;

    // Cloning the address space.
    page_directory_t *directory = clone_directory(current_directory);

    // Creating new process.
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    // Adding to the end of the ready queue.
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    //entry point for the new process.
    u32int eip = read_eip();

    if (current_task == parent_task)
    {
        // parent process, so set up the esp/ebp/eip for our child.
        u32int esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        u32int ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        asm volatile("sti");

        return new_task->id;
    }
    else
    {
        //child.
        return 0;
    }

}

int getpid()
{
    return current_task->id;
}