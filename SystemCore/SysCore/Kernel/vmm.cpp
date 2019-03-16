/* Ali Fallah 97/10/07  */
#include <stdint.h>
#include <string.h>
#include "vmm.h"
#include "pmm.h"

static pdir * _vmm_cur_dir;


struct pdir{ pt_entry entry[1024]; };

struct ptable{ pt_entry entry[1024]; };
typedef struct pdir pdir;
typedef struct ptable ptable;


inline pt_entry * vmm_pt_get_entry(ptable *pe, virtual_addr addr){
	return &pe->entry[VMM_GET_PT_ENTRY(addr)];
}

inline pd_entry * vmm_pd_get_entry(pdir *pe, virtual_addr addr){
	return &pe->entry[VMM_GET_PT_ENTRY(addr)];
}

inline bool vmm_pd_switch(pdir *pd){
	if (!pd)
		return false;

	_vmm_cur_dir = pd;
	vmm_load_pdbr((pdir*)&_vmm_cur_dir->entry);
	return true;
}

void vmm_flush_tlb_entry(virtual_addr addr){
	_asm{
		cli
			invlpg addr
			sti
	}
}


struct pdir * vmm_get_cur_dir(){
	return _vmm_cur_dir;
}

bool vmm_alloc_frame(pt_entry * pe){
	void *p = pmm_alloc_block();
	if (!p)
		return false;
	vmm_pt_entry_set_frame(pe, (physical_addr)p);
	vmm_pt_entry_add_attrib(pe, VMM_PT_ENTRY_PRESENT);
	return true;
}

bool vmm_free_frame(pt_entry * pe){
	pmm_free_block(pe); /*Maybe this wrong because we send Address of entry ?*/
	vmm_pt_entry_del_attrib(pe, VMM_PT_ENTRY_PRESENT);
	return true;
}

void vmm_map_page(void* phys, void* virt){
	pdir *dir = _vmm_cur_dir;
	pd_entry *pde = (pd_entry *)&dir->entry[VMM_GET_PD_ENTRY((pd_entry)virt)];
	if (!(*pde&VMM_PT_ENTRY_PRESENT)){
		//! page table not present, allocate it
		ptable* table = (ptable*)pmm_alloc_block();
		if (!table)
			return;

		//! clear page table
		memset(table, 0, sizeof(ptable));

		//! create a new entry
		pd_entry* entry = &dir->entry[VMM_GET_PD_ENTRY((uint32_t)virt)];

		//! map in the table (Can also just do *entry |= 3) to enable these bits
		vmm_pd_entry_add_attrib(entry, VMM_PT_ENTRY_PRESENT);
		vmm_pd_entry_add_attrib(entry, VMM_PT_ENTRY_WRITABLE);
		vmm_pd_entry_set_frame(entry, (physical_addr)table);

	}
	ptable *table = (ptable *)(*pde& ~0xfff);
	pt_entry *page = &table->entry[VMM_GET_PT_ENTRY((pt_entry)virt)];
	vmm_pt_entry_set_frame(page, (physical_addr)phys);
	vmm_pt_entry_add_attrib(page, VMM_PT_ENTRY_PRESENT);
}


void vmm_initialize(){
	ptable * pt_first4mb = (ptable *)pmm_alloc_block();
	if (!pt_first4mb)
		return;
	ptable * pt_1mb_to_3gb = (ptable *)pmm_alloc_block();
	if (!pt_1mb_to_3gb)
		return;
	/* Map 1st 4Mb (phys) to 1st 4Mb (virt) - identity map */
	for (uint32_t i = 0, frame = 0; i < 1024; i++, frame += 4096)
		pt_first4mb->entry[i] = frame | VMM_PT_ENTRY_PRESENT;

	for (uint32_t i = 0, frame = 0x100000; i < 1024; i++, frame += 4096)
		pt_1mb_to_3gb->entry[i] = frame | VMM_PT_ENTRY_PRESENT;

	/*set Page table to page directory*/
	pdir *dir = (pdir*)pmm_alloc_block();
	if (!dir)
		return;

	memset(dir, 0, sizeof(pdir));
	dir->entry[0] = (pd_entry)pt_first4mb | VMM_PT_ENTRY_PRESENT | VMM_PT_ENTRY_WRITABLE;
	dir->entry[VMM_GET_PD_ENTRY(0xC0000000)] = (pt_entry)pt_1mb_to_3gb | VMM_PT_ENTRY_PRESENT | VMM_PT_ENTRY_WRITABLE;

	/*Set Page Directory Base Address*/
	vmm_pd_switch(dir);

	/* paging Enable */
	vmm_paging_enable(true);

}


void vmm_paging_enable(bool b){
	_asm {
		mov eax, cr0
			cmp byte ptr[b], 1
			jnz fal
			or eax, 0x80000000; Bit 31 (PG) : Enables Memory Paging.
			jmp set
		fal :
		and eax, 0x7FFFFFFF; clear Bit 31 (PG) : Enables Memory Paging.
		set :
			mov cr0, eax
	}
}


void vmm_load_pdbr(pdir *dir){
	_asm {
		mov eax, dword ptr[dir]
			mov cr3, eax
	}
}

/* All of under Funcuin written by mike - brokenthorn.com (Thanks!) */
inline void vmm_pt_entry_add_attrib(pt_entry* e, uint32_t attrib) {
	*e |= attrib;
}

inline void vmm_pt_entry_del_attrib(pt_entry* e, uint32_t attrib) {
	*e &= ~attrib;
}

inline void vmm_pt_entry_set_frame(pt_entry* e, physical_addr addr) {
	*e = (*e & ~VMM_PT_ENTRY_FRAME) | addr;
}

inline bool vmm_pt_entry_is_present(pt_entry e) {
	return e & VMM_PT_ENTRY_PRESENT;
}

inline bool vmm_pt_entry_is_writable(pt_entry e) {
	return e & VMM_PT_ENTRY_WRITABLE;
}

inline void vmm_pd_entry_add_attrib(pd_entry* e, uint32_t attrib) {
	*e |= attrib;
}

inline void vmm_pd_entry_del_attrib(pd_entry* e, uint32_t attrib) {
	*e &= ~attrib;
}

inline void vmm_pd_entry_set_frame(pd_entry* e, physical_addr addr) {
	*e = (*e & ~VMM_PD_ENTRY_FRAME) | addr;
}

inline bool vmm_pd_entry_is_present(pd_entry e) {
	return e & VMM_PD_ENTRY_PRESENT;
}

inline bool pd_entry_is_writable(pd_entry e) {
	return e & VMM_PT_ENTRY_WRITABLE;
}

inline bool vmm_pd_entry_is_user(pd_entry e) {
	return e & VMM_PD_ENTRY_USER;
}

inline bool vmm_pd_entry_is_4mb(pd_entry e) {
	return e & VMM_PD_ENTRY_4MB;
}

inline void pd_entry_enable_global(pd_entry e) {

}


/**
* Allocates new page table
* \param dir Page directory
* \param virt Virtual address
* \param flags Page flags
* \ret Status code
*/
int vmmngr_createPageTable(pdir* dir, uint32_t virt, uint32_t flags) {

	pd_entry* pagedir = dir->entry;
	if (pagedir[virt >> 22] == 0) {
		void* block = pmm_alloc_block();
		if (!block)
			return 0; /* Should call debugger */
		pagedir[virt >> 22] = ((uint32_t)block) | flags;
		memset((uint32_t*)pagedir[virt >> 22], 0, 4096);

		/* map page table into directory */
		vmm_mapPhysicalAddress(dir, (uint32_t)block, (uint32_t)block, flags);
	}
	return 1; /* success */
}

/**
* Map physical address to virtual
* \param dir Page directory
* \param virt Virtual address
* \param phys Physical address
* \param flags Page flags
*/
void vmm_mapPhysicalAddress(pdir* dir, uint32_t virt, uint32_t phys, uint32_t flags) {

	pd_entry* pagedir = dir->entry;
	if (pagedir[virt >> 22] == 0)
		vmmngr_createPageTable(dir, virt, flags);
	((uint32_t*)(pagedir[virt >> 22] & ~0xfff))[virt << 10 >> 10 >> 12] = phys | flags;
}

