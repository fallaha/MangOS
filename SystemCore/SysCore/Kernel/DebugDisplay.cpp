#include <stdint.h>
#include "DebugDisplay.h"
#include <stdarg.h>
#include <string.h>

uint8_t _curX = 0;	/*Current X Posation*/
uint8_t _curY = 0;	/*Current Y posation*/
uint16_t *video_memory = (uint16_t *)0xB8000; /*Adress of Video Memory*/
uint16_t _color = 0x1f;	/*Current Color*/

/*	Display a character */
void DebugPutc(unsigned char ch){
	if (!ch)
		return;
	uint16_t attribute = _color << 8;
	if (ch == 0x0A){ /*next Line*/
		_curX = 0;
		_curY++;
	}
	else if (ch == '\b' && _curX) /*BackSpace*/
		_curX--;
	else if (ch == '\r') /*return*/
		_curX = 0;
	else if (ch == 0x09)
		_curX += 8;
	else if (ch >= ' '){ /*ASSCI : 32 */
		uint16_t * location = (uint16_t *)(video_memory + (_curX + _curY * 80));
		*location = ch | attribute;
		_curX++;
	}

	if (_curX >= 80){
		_curX = 0;
		_curY++;
	}

}

void DebugSetColor(uint16_t color){
	_color = color;
}

void DebugClrScreen(unsigned char bgcolor){
	DebugGotoXY(0, 0);
	uint16_t *loc;
	loc = video_memory;
	for (int i = 0; i < 25 * 80; i++)
		loc[i] = ' ' | (bgcolor << 8);
}


void DebugGotoXY(uint8_t x, uint8_t y){
	_curX = x;
	_curY = y;

}

void DebugPuts(char * s){
	while (*s)
		DebugPutc(*s++);
}

/* New thing in 97/9/30 */

char tbuf[32];
char bchars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void itoa(unsigned i, unsigned base, char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top = pos--;
	for (opos = 0; opos<top; pos--, opos++) {
		buf[opos] = tbuf[pos];
	}
	buf[opos] = 0;
}

void itoa_s(int i, unsigned base, char* buf) {
	if (base > 16) return;
	if (i < 0) {
		*buf++ = '-';
		i *= -1;
	}
	itoa(i, base, buf);
}

static char Debugstr[32] = { 0 };
//! Displays a formatted string
int DebugPrintf(const char* str, ...) {

	if (!str)
		return 0;

	va_list		args;
	va_start(args, str);
	size_t i;
	for (i = 0; i<strlen(str); i++) {

		switch (str[i]) {

		case '%':

			switch (str[i + 1]) {

				/*** characters ***/
			case 'c': {
				char c = va_arg(args, char);
				DebugPutc(c);
				i++;		// go to next character
				break;
			}

					  /*** address of ***/
			case 's': {
				int c = (int&)va_arg(args, char);
				strcpy(Debugstr, (const char*)c);
				DebugPuts(Debugstr);
				i++;		// go to next character
				break;
			}

					  /*** integers ***/
			case 'd':
			case 'i': {
				int c = va_arg(args, int);

				itoa_s(c, 10, Debugstr);
				DebugPuts(Debugstr);
				i++;		// go to next character
				break;
			}

					  /*** display in hex ***/
			case 'X':
			case 'x': {
				int c = va_arg(args, int);
				itoa_s(c, 16, Debugstr);
				DebugPuts(Debugstr);
				i++;		// go to next character
				break;
			}

			default:
				va_end(args);
				return 1;
			}

			break;

		default:
			DebugPutc(str[i]);
			break;
		}

	}

	va_end(args);
	return i;
}