/* Ali Fallah (C) 1397/12/25 */

#include "PCB.h"
#include "..\Kernel\vmm.h"
#include <HAL.h>


#define PAGE_SIZE    0x1000

PROCESS idle;
PROCESS *ready_queue_head;
PROCESS *current_process;

Thread *current_task;
void scheduler_tick() {
	current_task = current_task->t_next;
}

void pm_set_current_task(Thread *t)
{
	current_task = t;
}

__declspec(naked) void _cdecl  scheduler()
{

	_asm {
		;
		; clear interrupts and save context.
			;
		cli
			pushad
			;
		; if no current task, just return.
			;
		;
		; save selectors.
			;
		push ds
			push es
			push fs
			push gs
			;
		; switch to kernel segments.
			;
		mov ax, 0x10
			mov ds, ax
			mov es, ax
			mov fs, ax
			mov gs, ax
			;
		; save esp.
			;
		mov eax, [current_task]
			mov[eax], esp
			;
		; call scheduler.
			;
		call scheduler_tick
			;
		; restore esp.
			;
		mov eax, [current_task]
			mov esp, [eax]
			;
		; send EOI and restore context.
			;
		pop gs
			pop fs
			pop es
			pop ds


			mov al, 0x20
			out 0x20, al

			popad
			iretd
	}
}


void start_scheduler()
{
	setvect(32, scheduler);
}



typedef struct _trapFrame {
	/* pushed by isr. */
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	/* pushed by pushf. */
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t esp;
	uint32_t ebp;
	/* pushed by cpu. */
	uint32_t eip;
	uint32_t cs;
	uint32_t flags;
	/* used only when coming from/to user mode. */
	//	uint32_t user_stack;
	//	uint32_t user_ss;
}trapFrame;



#define KERNEL_DATA 0x10
#define KERNEL_CODE 8

uint32_t init_stack(void(_cdecl *entry)(void), uint32_t esp){

	trapFrame* frame;

	/* adjust stack. We are about to push data on it. */
	esp -= sizeof(trapFrame);

	/* initialize task frame. */
	frame = ((trapFrame*)esp);
	frame->flags = 0x202;
	frame->eip = (uint32_t)entry;
	frame->ebp = 0;
	frame->esp = 0;
	frame->edi = 0;
	frame->esi = 0;
	frame->edx = 0;
	frame->ecx = 0;
	frame->ebx = 0;
	frame->eax = 0;

	/* set up segment selectors. */
	frame->cs = KERNEL_CODE;
	frame->ds = KERNEL_DATA;
	frame->es = KERNEL_DATA;
	frame->fs = KERNEL_DATA;
	frame->gs = KERNEL_DATA;
	/* set stack. */
	return esp;
}

#define PROCESS_KERNEL_STACK_BASE 0xe0000000
int stack_counter = 0;

virtual_addr create_kernel_stack()
{
	physical_addr phys_addr;
	virtual_addr vir_addr;

	if (!(phys_addr = (physical_addr)pmm_alloc_block()))
		return 0;

	vir_addr = PROCESS_KERNEL_STACK_BASE + stack_counter * PAGE_SIZE;
	vmm_mapPhysicalAddress(vmm_get_cur_dir(), vir_addr, phys_addr, 3);
	stack_counter++;
	return (vir_addr + PAGE_SIZE - 1);

}

void create_thread(void(*entry)(void), Thread* t)
{
	/* Alloc a Block of Memory */
	virtual_addr esp;
	esp = (uint32_t)create_kernel_stack();
	t->esp = init_stack(entry, esp);
	t->t_next = 0;
}

void task_exe()
{
	_asm {
		;
		; restore esp.
			;
		mov eax, [current_task]
			mov esp, [eax]
			;
		; send EOI and restore context.
			;
		pop gs
			pop fs
			pop es
			pop ds
			popad
			iretd
	}

}


