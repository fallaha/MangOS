//****************************************************************************
//**
//**    panic.cpp
//**
//****************************************************************************

#include <hal.h>
#include <stdarg.h>
#include "DebugDisplay.h"

//! something is wrong--bail out
void _cdecl kernel_panic(const char* fmt, ...) {

	interrupt_disable();

	va_list		args;
	static char buf[1024];

	va_start(args, fmt);

	// We will need a vsprintf() here. I will see if I can write
	// one before the tutorial release

	va_end(args);

	char* disclamer = "We apologize, MOS has encountered a problem and has been shut down\n\
					  to prevent damage to your computer. Any unsaved work might be lost.\n\
					  We are sorry for the inconvenience this might have caused.\n\n\
					  Please report the following information and restart your computer.\n\
					  The system has been halted.\n\n";

	DebugClrScreen(0x1f);
	DebugGotoXY(0, 0);
	DebugSetColor(0x1f);
	DebugPuts(disclamer);

	DebugPrintf("*** STOP: %s", fmt);

	for (;;);
}

