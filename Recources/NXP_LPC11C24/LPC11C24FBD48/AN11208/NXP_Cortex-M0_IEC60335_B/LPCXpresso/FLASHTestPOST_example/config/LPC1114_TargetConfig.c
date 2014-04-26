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
#define MISR_FLASH_CRC	{ 0x2650ef4b, 0x824d1584, 0x33dd2754, 0x3d118108 }

//#if (SELECTED_CRC_TYPE == MISRHW_ALGO)
//  const FlashSign_t flashSignature __attribute__ ((section ("flashSignatureSection"))) = MISR_FLASH_CRC;
//#endif
#if (SELECTED_CRC_TYPE == MISRHW_ALGO)
__attribute__ ((section(".flashSignatureSection")))
const FlashSign_t flashSignature = MISR_FLASH_CRC;
#endif

