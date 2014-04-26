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

/* structures used for RAM Bist tests */
const ADDR_TYPE RamBistMarchTestTable[NUM_RAM_MARCH_TEST_BLOCKS][2] = {
   { (ADDR_TYPE) IEC60335_MARCH_SEG1_START, (ADDR_TYPE) IEC60335_MARCH_SEG1_END },  
   { (ADDR_TYPE) IEC60335_MARCH_SEG2_START, (ADDR_TYPE) IEC60335_MARCH_SEG2_END }  
 };


/* IEC60335 POST Flash test signature */
/* special section aligned to 16 bytes for usage with MISR */
#pragma section="flashSignatureSection" 16 

#define CCITT_FLASH_CRC { (CcittSig_t) 0xB3D7 }
#define CRC16_FLASH_CRC { (Crc16Sig_t) 0x0AB0 }
#define CRC32_FLASH_CRC { (Crc32Sig_t) 0x87C79BAD }
 
  
#if (SELECTED_CRC_TYPE == CCITT_ALGO)
  const CcittSig_t  ccittFlashSignature @ "flashSignatureSection" = CCITT_FLASH_CRC;
#endif
  
#if (SELECTED_CRC_TYPE == CRC16_ALGO)
  const Crc16Sig_t  crc16FlashSignature @ "flashSignatureSection" = CRC16_FLASH_CRC;
#endif

#if (SELECTED_CRC_TYPE == CRC32_ALGO)
  const Crc32Sig_t  crc32FlashSignature @ "flashSignatureSection" = CRC32_FLASH_CRC;
#endif
