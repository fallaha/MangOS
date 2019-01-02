#ifndef _KBRD_H
#define _KBRD_H
#include <stdint.h>
void kbrd_initilize();
uint8_t kbrd_get_last_std_char();
void kbrd_destroy_last_char();
uint8_t kbrd_make(uint8_t c);

#define KBRD_STATUS_PORT     0x64
#define KBRD_OUT_BUF     0x60
#define KBRD_INPT_BUF    0x60
#define KBRD_CMD_BUF     0x64
#define KBRD_DATA_BUF    0x60
#define KBRD_PORT_A          0x60
#define KBRD_PORT_B          0x61
#define KBRD_OUT_BUF_FULL    0x01
#define KBRD_INPT_BUF_FULL   0x02
#define KBRD_SYS_FLAG        0x04
#define KBRD_CMD_DATA        0x08
#define KBRD_INH       0x10
#define KBRD_TRANS_TMOUT     0x20
#define KBRD_RCV_TMOUT       0x40
#define KBRD_PARITY_EVEN     0x80
#define KBRD_INH_KEYBOARD    0x10
#define KBRD_ENA         0xAE
#define KBRD_DIS         0xAD
// Keyboard Commands	
#define KBRD_MENU         0x0F1
#define KBRD_ENABLE       0x0F4
#define KBRD_MAKEBREAK    0x0F7
#define KBRD_ECHO         0x0FE
#define KBRD_RESET        0x0FF
#define KBRD_LED_CMD      0x0ED
//Keyboard responses	
#define KBRD_OK           0x0AA
#define KBRD_ACK          0x0FA
#define KBRD_OVERRUN      0x0FF
#define KBRD_RESEND       0x0FE
#define KBRD_BREAK        0x0F0
#define KBRD_FA           0x010
#define KBRD_FE           0x020
#define KBRD_PR_LED       0x040





/*Keyboard Key scan Code */
/* help with www.brokenthorn.com */
// Alphanumeric keys ////////////////

#define KBRD_KEY_SPACE  ' '
#define KBRD_KEY_0  '0'
#define KBRD_KEY_1  '1'
#define KBRD_KEY_2  '2'
#define KBRD_KEY_3  '3'
#define KBRD_KEY_4  '4'
#define KBRD_KEY_5  '5'
#define KBRD_KEY_6  '6'
#define KBRD_KEY_7  '7'
#define KBRD_KEY_8  '8'
#define KBRD_KEY_9  '9'

#define KBRD_KEY_A  'a'
#define KBRD_KEY_B  'b'
#define KBRD_KEY_C  'c'
#define KBRD_KEY_D  'd'
#define KBRD_KEY_E  'e'
#define KBRD_KEY_F  'f'
#define KBRD_KEY_G  'g'
#define KBRD_KEY_H  'h'
#define KBRD_KEY_I  'i'
#define KBRD_KEY_J  'j'
#define KBRD_KEY_K  'k'
#define KBRD_KEY_L  'l'
#define KBRD_KEY_M  'm'
#define KBRD_KEY_N  'n'
#define KBRD_KEY_O  'o'
#define KBRD_KEY_P  'p'
#define KBRD_KEY_Q  'q'
#define KBRD_KEY_R  'r'
#define KBRD_KEY_S  's'
#define KBRD_KEY_T  't'
#define KBRD_KEY_U  'u'
#define KBRD_KEY_V  'v'
#define KBRD_KEY_W  'w'
#define KBRD_KEY_X  'x'
#define KBRD_KEY_Y  'y'
#define KBRD_KEY_Z  'z'

#define KBRD_KEY_RETURN  '\r'
#define KBRD_KEY_ESCAPE  0x1001
#define KBRD_KEY_BACKSPACE  '\b'

// ArrowKBRD_ keys ////////////////////////

#define KBRD_KEY_UP  0x1100
#define KBRD_KEY_DOWN  0x1101
#define KBRD_KEY_LEFT 0x1102
#define KBRD_KEY_RIGHT  0x1103

 // FuncKBRD_tion keys /////////////////////

#define KBRD_KEY_F1  0x1201
#define KBRD_KEY_F2  0x1202
#define KBRD_KEY_F3  0x1203
#define KBRD_KEY_F4  0x1204
#define KBRD_KEY_F5  0x1205
#define KBRD_KEY_F6  0x1206
#define KBRD_KEY_F7  0x1207
#define KBRD_KEY_F8  0x1208
#define KBRD_KEY_F9  0x1209
#define KBRD_KEY_F10  0x120a
#define KBRD_KEY_F11  0x120b
#define KBRD_KEY_F12  0x120b
#define KBRD_KEY_F13  0x120c
#define KBRD_KEY_F14  0x120d
#define KBRD_KEY_F15  0x120e

#define KBRD_KEY_DOT  '.' 
#define KBRD_KEY_COMMA  ',' 
#define KBRD_KEY_COLON  ':' 
#define KBRD_KEY_SEMICOLON  ';' 
#define KBRD_KEY_SLASH  '/' 
#define KBRD_KEY_BACKSLASH  '\\' 
#define KBRD_KEY_PLUS '+' 
#define KBRD_KEY_MINUS  '-' 
#define KBRD_KEY_ASTERISK  '*' 
#define KBRD_KEY_EXCLAMATION  '!' 
#define KBRD_KEY_QUESTION  '?' 
#define KBRD_KEY_QUOTEDOUBLE  '\"' 
#define KBRD_KEY_QUOTE  '\'' 
#define KBRD_KEY_EQUAL  ' ' 
#define KBRD_KEY_HASH  '#' 
#define KBRD_KEY_PERCENT   '%' 
#define KBRD_KEY_AMPERSAND   '&' 
#define KBRD_KEY_UNDERSCORE   '_' 
#define KBRD_KEY_LEFTPARENTHESIS   '(' 
#define KBRD_KEY_RIGHTPARENTHESIS   ')' 
#define KBRD_KEY_LEFTBRACKET   '[' 
#define KBRD_KEY_RIGHTBRACKET   ']' 
#define KBRD_KEY_LEFTCURL   '{' 
#define KBRD_KEY_RIGHTCURL   '}' 
#define KBRD_KEY_DOLLAR   '$' 
#define KBRD_KEY_POUND   '£' 
#define KBRD_KEY_EURO   '$' 
#define KBRD_KEY_LESS   '<' 
#define KBRD_KEY_GREATER   '>' 
#define KBRD_KEY_BAR   '|' 
#define KBRD_KEY_GRAVE   '`' 
#define KBRD_KEY_TILDE   '~' 
#define KBRD_KEY_AT   '@' 
#define KBRD_KEY_CARRET   '^' 
#define KBRD_
// Numeric keypad //////////////////////
#define KBRD_
#define KBRD_KEY_KP_0  '0' 
#define KBRD_KEY_KP_1  '1' 
#define KBRD_KEY_KP_2  '2' 
#define KBRD_KEY_KP_3  '3' 
#define KBRD_KEY_KP_4  '4' 
#define KBRD_KEY_KP_5  '5' 
#define KBRD_KEY_KP_6  '6' 
#define KBRD_KEY_KP_7  '7' 
#define KBRD_KEY_KP_8  '8' 
#define KBRD_KEY_KP_9  '9' 
#define KBRD_KEY_KP_PLUS   '+' 
#define KBRD_KEY_KP_MINUS   '-' 
#define KBRD_KEY_KP_DECIMAL   '.' 
#define KBRD_KEY_KP_DIVIDE   '/' 
#define KBRD_KEY_KP_ASTERISK   '*' 
#define KBRD_KEY_KP_NUMLOCK   0x300f 
#define KBRD_KEY_KP_ENTER   0x3010 

#define KBRD_KEY_TAB   0x4000 
#define KBRD_KEY_CAPSLOCK   0x4001 

// Modify keys ////////////////////////////

#define KBRD_KEY_LSHIFT   0x4002 
#define KBRD_KEY_LCTRL   0x4003 
#define KBRD_KEY_LALT   0x4004 
#define KBRD_KEY_LWIN   0x4005 
#define KBRD_KEY_RSHIFT   0x4006 
#define KBRD_KEY_RCTRL   0x4007 
#define KBRD_KEY_RALT   0x4008 
#define KBRD_KEY_RWIN   0x4009 

#define KBRD_KEY_INSERT   0x400a 
#define KBRD_KEY_DELETE   0x400b 
#define KBRD_KEY_HOME   0x400c 
#define KBRD_KEY_END   0x400d 
#define KBRD_KEY_PAGEUP   0x400e 
#define KBRD_KEY_PAGEDOWN   0x400f 
#define KBRD_KEY_SCROLLLOCK   0x4010 
#define KBRD_KEY_PAUSE   0x4011 

#define KBRD_KEY_UNKNOWN 0
#define KBRD_KEY_NUMKEYCODES 0

#endif