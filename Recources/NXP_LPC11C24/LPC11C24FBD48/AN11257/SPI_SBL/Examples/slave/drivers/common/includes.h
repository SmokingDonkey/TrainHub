/**********************************************************************
* $Id$		includes.h			2012-05-04
*//**
* @file		includes.h
* @brief		Collect include header files
* @version	1.0
* @date		04. May. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include "lpc_types.h"
#include "sbl.h"
#include "iap.h"
#ifdef __MCU_LPC17xx
#include "LPC17xx.h"
#include "lpc17xx_spi.h"
#elif defined (__MCU_LPC11xx)
#include "LPC11xx.h"
#include "LPC11xx_ssp.h"
#endif

#endif /*__INCLUDES_H__*/
