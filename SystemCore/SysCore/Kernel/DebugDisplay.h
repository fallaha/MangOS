#ifndef _DEBUGDISPLAY_H
#define _DEBUGDISPLAY_H
#include <stdint.h>
#include <string.h>

void DebugPutc(unsigned char ch);
void DebugClrScreen();
void DebugGotoXY(uint8_t x, uint8_t y);
void DebugClrScreen(unsigned char bgcolor);
void DebugSetColor(uint16_t color);
void DebugPuts(char * s);
int DebugPrintf(const char* str, ...);
#endif /*_DEBUGDISPLAY_H*/