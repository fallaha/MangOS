/*	Ali Fallah (C) 97/10/12 */
/* Help from www.brokenthorn.com ~ mike :) */

#include "kbrd.h"
#include <HAL.h>
#include <stdint.h>
#include "..\Kernel\DebugDisplay.h"
static volatile uint8_t _kbrd_code_char = 0;
void kbrd_irq_handler();
uint8_t kbrd_controller_read_inbuf();
uint8_t kbrd_controller_read_status();

static uint16_t kbrd_scan_code_list [] = {
	//! key			scancode
	0,						//0
	KBRD_KEY_ESCAPE,		//1
	KBRD_KEY_1,				//2
	KBRD_KEY_2,				//3
	KBRD_KEY_3,				//4
	KBRD_KEY_4,				//5
	KBRD_KEY_5,				//6
	KBRD_KEY_6,				//7
	KBRD_KEY_7,				//8
	KBRD_KEY_8,				//9
	KBRD_KEY_9,				//0xa
	KBRD_KEY_0,				//0xb
	KBRD_KEY_MINUS,			//0xc
	KBRD_KEY_EQUAL,			//0xd
	KBRD_KEY_BACKSPACE,		//0xe
	KBRD_KEY_TAB,			//0xf
	KBRD_KEY_Q,				//0x10
	KBRD_KEY_W,				//0x11
	KBRD_KEY_E,				//0x12
	KBRD_KEY_R,				//0x13
	KBRD_KEY_T,				//0x14
	KBRD_KEY_Y,				//0x15
	KBRD_KEY_U,				//0x16
	KBRD_KEY_I,				//0x17
	KBRD_KEY_O,				//0x18
	KBRD_KEY_P,				//0x19
	KBRD_KEY_LEFTBRACKET,   //0x1a
	KBRD_KEY_RIGHTBRACKET,  //0x1b
	KBRD_KEY_RETURN,		//0x1c
	KBRD_KEY_LCTRL,		    //0x1d
	KBRD_KEY_A,			    //0x1e
	KBRD_KEY_S,			    //0x1f
	KBRD_KEY_D,			    //0x20
	KBRD_KEY_F,			    //0x21
	KBRD_KEY_G,			    //0x22
	KBRD_KEY_H,			    //0x23
	KBRD_KEY_J,			    //0x24
	KBRD_KEY_K,			    //0x25
	KBRD_KEY_L,			    //0x26
	KBRD_KEY_SEMICOLON,	    //0x27
	KBRD_KEY_QUOTE,		    //0x28
	KBRD_KEY_GRAVE,		    //0x29
	KBRD_KEY_LSHIFT,		//0x2a
	KBRD_KEY_BACKSLASH,		//0x2b
	KBRD_KEY_Z,				//0x2c
	KBRD_KEY_X,				//0x2d
	KBRD_KEY_C,				//0x2e
	KBRD_KEY_V,				//0x2f
	KBRD_KEY_B,				//0x30
	KBRD_KEY_N,				//0x31
	KBRD_KEY_M,				//0x32
	KBRD_KEY_COMMA,			//0x33
	KBRD_KEY_DOT,			//0x34
	KBRD_KEY_SLASH,			//0x35
	KBRD_KEY_RSHIFT,		//0x36
	KBRD_KEY_KP_ASTERISK,   //0x37
	KBRD_KEY_RALT,		    //0x38
	KBRD_KEY_SPACE,		    //0x39
	KBRD_KEY_CAPSLOCK,	    //0x3a
	KBRD_KEY_F1,			//0x3b
	KBRD_KEY_F2,			//0x3c
	KBRD_KEY_F3,			//0x3d
	KBRD_KEY_F4,			//0x3e
	KBRD_KEY_F5,			//0x3f
	KBRD_KEY_F6,			//0x40
	KBRD_KEY_F7,			//0x41
	KBRD_KEY_F8,			//0x42
	KBRD_KEY_F9,			//0x43
	KBRD_KEY_F10,			//0x44
	KBRD_KEY_KP_NUMLOCK,	//0x45
	KBRD_KEY_SCROLLLOCK,	//0x46
	KBRD_KEY_HOME,			//0x47
	KBRD_KEY_KP_8,			//0x48	//keypad up arrow
	KBRD_KEY_PAGEUP,		//0x49
	KBRD_KEY_KP_2,			//0x50	//keypad down arrow
	KBRD_KEY_KP_3,			//0x51	//keypad page down
	KBRD_KEY_KP_0,			//0x52	//keypad insert key
	KBRD_KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KBRD_KEY_UNKNOWN,		//0x54
	KBRD_KEY_UNKNOWN,		//0x55
	KBRD_KEY_UNKNOWN,		//0x56
	KBRD_KEY_F11,			//0x57
	KBRD_KEY_F12			//0x58
};

struct ikey
{
	bool shift;
	bool ctrl;
	bool alt;
	bool capslock;
	bool numlock;
	bool scrolllock;
};
static struct ikey ikey;

void kbrd_irq_handler(){
	_asm cli
	uint8_t code = 0;
	if (kbrd_controller_read_status() & KBRD_OUT_BUF_FULL){
		code = kbrd_controller_read_inbuf();

		if (code & 0x80){ /*it is brake scan code*/
			code -= 0x80;
			uint16_t key = kbrd_scan_code_list[code];
			switch (key)
			{
			case KBRD_KEY_LSHIFT:
			case KBRD_KEY_RSHIFT:
				ikey.shift = false;
				break;
			case KBRD_KEY_LALT:
			case KBRD_KEY_RALT:
				ikey.alt = false;
				break;
			case KBRD_KEY_LCTRL:
			case KBRD_KEY_RCTRL:
				ikey.ctrl = false;
				break;
			}
		}
		else {	/*it is make scan code */
			uint16_t key = kbrd_scan_code_list[code];
			switch (key)
			{
			case KBRD_KEY_LSHIFT:
			case KBRD_KEY_RSHIFT:
				ikey.shift = true;
				break;
			case KBRD_KEY_LALT:
			case KBRD_KEY_RALT:
				ikey.alt = true;
				break;
			case KBRD_KEY_LCTRL:
			case KBRD_KEY_RCTRL:
				ikey.ctrl = true;
				break;
			case KBRD_KEY_CAPSLOCK:
				ikey.capslock ^= true;
				break;
			case KBRD_KEY_KP_NUMLOCK:
				ikey.numlock ^= true;
				break;
			case KBRD_KEY_SCROLLLOCK:
				ikey.scrolllock ^= true;
				break;
			default :
				_kbrd_code_char = code;
				break;
			}
			
		}
	}

	intdone(1);
	_asm sti
	_asm iretd
}

uint8_t kbrd_controller_read_inbuf(){
	return inportb(KBRD_OUT_BUF);
}

uint8_t kbrd_controller_read_status(){
	return inportb(KBRD_STATUS_PORT);
}

uint8_t kbrd_get_last_std_char()
{	
	return _kbrd_code_char;
}

void kbrd_destroy_last_char(){
	_kbrd_code_char = 0;
}

uint8_t kbrd_make (uint8_t c){
	if (!c)
		return 0;
	uint8_t key = kbrd_scan_code_list[c];

	if ((ikey.shift && !ikey.capslock) || (!ikey.shift && ikey.capslock))
		if (key >= 'a' && key <= 'z')
			key -= 32;
		else if (key >= '0' && key <= '9')
			switch (key) {

			case '0':
				key = KBRD_KEY_RIGHTPARENTHESIS;
				break;
			case '1':
				key = KBRD_KEY_EXCLAMATION;
				break;
			case '2':
				key = KBRD_KEY_AT;
				break;
			case '3':
				key = KBRD_KEY_EXCLAMATION;
				break;
			case '4':
				key = KBRD_KEY_HASH;
				break;
			case '5':
				key = KBRD_KEY_PERCENT;
				break;
			case '6':
				key = KBRD_KEY_CARRET;
				break;
			case '7':
				key = KBRD_KEY_AMPERSAND;
				break;
			case '8':
				key = KBRD_KEY_ASTERISK;
				break;
			case '9':
				key = KBRD_KEY_LEFTPARENTHESIS;
				break;
		}
		else {

			switch (key) {
			case KBRD_KEY_COMMA:
				key = KBRD_KEY_LESS;
				break;

			case KBRD_KEY_DOT:
				key = KBRD_KEY_GREATER;
				break;

			case KBRD_KEY_SLASH:
				key = KBRD_KEY_QUESTION;
				break;

			case KBRD_KEY_SEMICOLON:
				key = KBRD_KEY_COLON;
				break;

			case KBRD_KEY_QUOTE:
				key = KBRD_KEY_QUOTEDOUBLE;
				break;

			case KBRD_KEY_LEFTBRACKET:
				key = KBRD_KEY_LEFTCURL;
				break;

			case KBRD_KEY_RIGHTBRACKET:
				key = KBRD_KEY_RIGHTCURL;
				break;

			case KBRD_KEY_GRAVE:
				key = KBRD_KEY_TILDE;
				break;

			case KBRD_KEY_MINUS:
				key = KBRD_KEY_UNDERSCORE;
				break;

			case KBRD_KEY_PLUS:
				key = KBRD_KEY_EQUAL;
				break;

			case KBRD_KEY_BACKSLASH:
				key = KBRD_KEY_BAR;
				break;
			}
		}

		//! return the key
		return key;
}

void kbrd_initilize(){
	setvect(33, (void(__cdecl &)(void))kbrd_irq_handler);
}