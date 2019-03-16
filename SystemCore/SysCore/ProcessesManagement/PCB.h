/* Ali Fallah (c) 1397/12/17 */

#ifndef PCB_H
#define PCB_H

#include <stdint.h>

#define PROCESS_MAX_CHILD 16
#define PROCESS_MAX_THREAD 16
#define READY 1
#define RUNNING 2
#define SUSPAND 3




struct TSS {
	uint16_t previous_task_link;
	uint16_t reserved;

	uint32_t esp0;

	uint16_t ss0;
	uint16_t reserved2;
	uint32_t esp1;

	uint16_t ss1;
	uint16_t reserved3;

	uint32_t esp2;

	uint16_t ss2;
	uint16_t reserved4;

	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	uint16_t es;
	uint16_t reserved5;

	uint16_t cs;
	uint16_t reserved6;

	uint16_t ss;
	uint16_t reserved7;

	uint16_t ds;
	uint16_t reserved8;

	uint16_t fs;
	uint16_t reserved9;

	uint16_t gs;
	uint16_t reserved10;

	uint16_t ldt; /* LDT Segment Selector */
	uint16_t reserved11;

	uint16_t reserved12;
	uint16_t iomap; /* I/O Map Base Address */

};

struct Thread
{
	uint32_t esp;
	Thread* t_next;
};

struct PROCESS
{
	uint32_t esp;
	/* identify */
	int pid;
	int gid;
	int uid;
	int parent_id;

	/* Status */
	

	/* Control */
	int scheduler_status;
	int priority;
	int qrun;  /* Quantum Runned */
	int eventno; /* Number of Event Waiting */

	int child_id[PROCESS_MAX_CHILD];
	Thread thread[PROCESS_MAX_THREAD];

	PROCESS *next;
};

/*
struct TSSDESCRIPTOR 
{
	uint16_t limit0;
	uint16_t base0;
	uint8_t base16;
	unsigned type : 4;
	unsigned zero : 1;
	unsigned DPL  : 2;
	unsigned P    : 1;
	unsigned limit16 : 4;
	unsigned avl : 1;
	unsigned zeros : 2;
	unsigned G : 1;
	uint8_t base24 ;
};
*/
#endif