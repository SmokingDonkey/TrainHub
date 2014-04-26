/**
 * @file	LPC1227_TargetConfig.c
 * @purpose
 * @brief       Includes definition of address ranges to test for 
 *              the program
 * @version 1.0
 * @date	29-apr-2011
 * @author	nxp28536
*/
/*----------------------------------------------------------------------------
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
#include "IEC60335.h"


/* IEC60335 POST Flash test signature */
/* special section aligned to 16 bytes for usage with MISR */
#define MISR_FLASH_CRC	{ 0x392FC970, 0xB9EADDB7, 0xE015C2C6, 0x58AAFEFA }

#if (SELECTED_CRC_TYPE == MISRHW_ALGO)
  const FlashSign_t flashSignature __attribute__ ((section ("flashSignatureSection"))) = MISR_FLASH_CRC;
#endif 

// unsigned long  IEC60335_BOTTOM_RAM_POST_MARCH = 0x10000000;
// unsigned long  IEC60335_TOP_RAM_POST_MARCH 	  = 0x10002000;

