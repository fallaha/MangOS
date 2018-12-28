#ifndef _VMM_H
#define _VMM_H
#include <stdint.h>
#include "pmm.h"

/* page table represents 4mb address space */
#define VMM_PT_ADDRESS_SPACE_SIZE 0x400000
/* directory table represents 4gb address space */
#define VMM_PD_ADDRESS_SPACE_SIZE 0x100000000
/* page sizes are 4k */
#define VMM_PAGE_SIZE 4096
#define VMM_GET_PT_ENTRY(addr) (addr>>12)&0x3ff
#define VMM_GET_PD_ENTRY(addr) (addr>>22)&0x3ff


#define VMM_PT_ENTRY_PRESENT 1
#define VMM_PD_ENTRY_PRESENT 1

#define VMM_PT_ENTRY_WRITABLE 2 /*10b*/
#define VMM_PD_ENTRY_WRITABLE 2 /*10b*/

#define VMM_PT_ENTRY_FRAME 0x7FFFF000 	//1111111111111111111000000000000
#define VMM_PD_ENTRY_FRAME 0x7FFFF000 	//1111111111111111111000000000000

#define VMM_PD_ENTRY_USER 4			//0000000000000000000000000000100
#define VMM_PD_ENTRY_4MB 0x80		//0000000000000000000000010000000
typedef uint32_t pt_entry;
typedef uint32_t pd_entry;
typedef uint32_t virtual_addr;


void vmm_load_pdbr(struct pdir *dir);
struct pdir * vmm_get_cur_dir();
bool vmm_alloc_frame(pt_entry * pe);
bool vmm_free_frame(pt_entry * pe);
void vmm_map_page(void* phys, void* virt);
void vmm_initialize();

//! sets a flag in the page table entry
extern void vmm_pd_entry_add_attrib(pd_entry* e, uint32_t attrib);

//! clears a flag in the page table entry
extern void vmm_pd_entry_del_attrib(pd_entry* e, uint32_t attrib);

//! sets a frame to page table entry
extern void vmm_pd_entry_set_frame(pd_entry*, physical_addr);

//! test if page is present
extern bool vmm_pd_entry_is_present(pd_entry e);

//! test if directory is user mode
extern bool vmm_pd_entry_is_user(pd_entry);

//! test if directory contains 4mb pages
extern bool vmm_pd_entry_is_4mb(pd_entry);

//! test if page is writable
extern bool vmm_pd_entry_is_writable(pd_entry e);

//! get page table entry frame address
extern physical_addr pd_entry_pfn(pd_entry e);

//! enable global pages
extern void vmm_pd_entry_enable_global(pd_entry e);

//! sets a flag in the page table entry
extern void vmm_pt_entry_add_attrib(pt_entry* e, uint32_t attrib);

//! clears a flag in the page table entry
extern void vmm_pt_entry_del_attrib(pt_entry* e, uint32_t attrib);

//! sets a frame to page table entry
extern void vmm_pt_entry_set_frame(pt_entry*, physical_addr);

//! test if page is present
extern bool vmm_pt_entry_is_present(pt_entry e);

//! test if page is writable
extern bool vmm_pt_entry_is_writable(pt_entry e);
void vmm_paging_enable(bool b);



#endif