#ifndef _PM_H
#define _PM_H

#include "PCB.h"

/* Definition of Function */
void start_scheduler();
void create_thread(void(*entry)(void), Thread* t);
void pm_set_current_task(Thread *t); 
void task_exe();

#endif