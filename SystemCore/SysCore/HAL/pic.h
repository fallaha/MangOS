#ifndef _PIC_H_INCLUDE
#define _PIC_H_INCLUDE

#include <stdint.h>

#define IA32_PIC_ICW1_IC4  0X1 /*00000001*/
#define IA32_PIC_ICW1_SNGL 0X2 /*00000010*/
#define IA32_PIC_ICW1_INIT 0X10 /*00010000*/
#define IA32_PIC_ICW4_x86 0X01  /*00000000*/
#define IA32_PIC0_CMD_REG 0X20 /*Primary PIC Command and Status Register*/
#define IA32_PIC1_CMD_REG 0XA0 /*Secondary (Slave) PIC Command and Status Register*/
#define IA32_PIC0_DATA_REG 0X21 /*Primary PIC Interrupt Mask Register and Data Register*/
#define IA32_PIC1_DATA_REG 0XA1 /*Secondary PIC Interrupt Mask Register and Data Register*/


void IA32_pic_initialize(uint8_t offset1, uint8_t offset2);
void IA32_pic_eoi(uint8_t intno);
#endif