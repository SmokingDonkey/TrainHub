/***********************************************************************
 * $Id:: Retarget.c 1604 2012-04-24 11:34:47Z nxp31103     $
 *
 * Project: CANopen Application Example for LPC11Cxx
 *
 * Description:
 *   Retarget source file for IAR
 *
 * Copyright(C) 2012, NXP Semiconductor
 * All rights reserved.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

#include "uart.h"

int (__write)(int Handle, const unsigned char *Buf, size_t BufSize) 
{ 
  int i;
  
  for(i=0;i<BufSize;i++)
  {
    UART_PutChar(Buf[i]);
  }
  
  return BufSize;
}

size_t __read(int handle,unsigned char *buf,size_t bufSize)
{
  size_t i;
  for (i=0x0; i<bufSize;i++)
  {
    buf[i] = UART_GetChar();
    UART_PutChar(buf[i]);
    if(buf[i] == '\r')
    {
      UART_PutChar('\n');
      buf[i] = '\n';
    }
  }
  return i;
}

void __lseek(void)
{
}

void __close(void)
{
}

int remove(char const* tmp)
{
  return 0;
}