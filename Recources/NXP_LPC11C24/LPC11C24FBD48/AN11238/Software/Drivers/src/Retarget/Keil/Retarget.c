/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#pragma import(__use_no_semihosting_swi)

extern void UART_PutChar (uint8_t ch);
extern uint8_t UART_GetChar (void);

struct __FILE
{
  int handle;

  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

/* FILE is typedef’d in stdio.h. */

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
  uint8_t tempch = ch; 		/* temp var avoids endianness issue */
  UART_PutChar(tempch);
  return ch;
}

int fgetc(FILE *f) {
	uint8_t tempch = UART_GetChar();

	UART_PutChar(tempch);
	if(tempch == '\r')
	{
		UART_PutChar('\n');
		return '\n';
	}
	else
		return tempch;
}

int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

void _ttywrch(int ch) {
  uint8_t tempch = ch; 		/* temp var avoids endianness issue */
  UART_PutChar(tempch);
}

void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
