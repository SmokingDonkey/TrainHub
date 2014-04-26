/**
 * @file	LPC1114_TargetConfig.h
 * @purpose
 * @brief       specifies target configuration settings for the LPC1114
 * @version 1.0
 * @date        28-apr-2011
 * @author      nxp28536
 * @changes
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

#ifndef __IEC60335_LPC1114_USER_CFG_H
#define __IEC60335_LPC1114_USER_CFG_H
	
/*****************************************************************************/
/* system settings */
/*****************************************************************************/
#define CPU_CLK 		4000000 /* 4 MHz IRC clock */
#define LED_GPIO    	LPC_GPIO0
#define LED_BIT     	7

/* pick Port P0_7 for the fault led */
#define PIOxy_FAULT_DIR     (0x1 << 7)
#define PIOxy_FAULT_DIRADD  (0x50008000)
#define PIOxy_FAULT_SETADD  (0x50000008 | PIOxy_FAULT_DIR)
#define PIOxy_FAULT_SET     (0x1 << 7)
/*****************************************************************************/
/* flash memory settings */
/*****************************************************************************/
/* 32 K on LPC1114/301 */
#define FLASH_SIZE  SIZE32K
#define SELECTED_CRC_TYPE  MISRHW_ALGO


/*
#define BOTTOM_ROM		  .equ IEC60335_BOTTOM_ROM_POST, 0x00000000
#define TOP_ROM		  	  .equ IEC60335_TOP_ROM_POST, 0x00001000
#define MISR_ADDR		  .equ MISR_SIGNATURE_ADDR, 0x2000
#define CRC_ADDR		  .equ CRC_SIGNATURE_ADDR, 0x2000
*/

#if (defined(__GNU_ASM__))
.equ IEC60335_BOTTOM_ROM_POST, 0x00000000
.equ IEC60335_TOP_ROM_POST, 0x00001000
.equ MISR_SIGNATURE_ADDR, 0x2000
.equ CRC_SIGNATURE_ADDR, 0x2000
#endif

// BOTTOM_RAM
#define BOTTOM_RAM .equ IEC60335_BOTTOM_RAM_POST_MARCH, 0x10000000
// TOP_RAM
#define TOP_RAM .equ IEC60335_TOP_RAM_POST_MARCH, 0x10002000

//*****************************************************************************/
/* End of user target configuration                                          */
/*****************************************************************************/

/*****************************************************************************/
/* sanity checks for user chosen configuration                               */
/*****************************************************************************/
#ifdef LPC1227
  #if (SELECTED_CRC_TYPE == MISRHW_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif

  #if ((SELECTED_CRC_TYPE != CCITT_ALGO) && (SELECTED_CRC_TYPE != CRC16_ALGO) && (SELECTED_CRC_TYPE != CRC32_ALGO))
    #error "Invalid CRC calculation type selected"
  #endif
#endif

#if defined (LPC1114)
  #if (SELECTED_CRC_TYPE == CCITT_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE == CRC16_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE == CRC32_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE != MISRHW_ALGO)
    #error "Invalid CRC calculation type selected"
  #endif
#endif


/*****************************************************************************/  
/*	End of user target configuration                                     */
/*****************************************************************************/

#endif
