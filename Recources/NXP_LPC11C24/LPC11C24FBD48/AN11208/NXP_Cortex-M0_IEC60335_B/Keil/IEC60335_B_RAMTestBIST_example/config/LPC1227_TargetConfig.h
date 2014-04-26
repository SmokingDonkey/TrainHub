/**
 * @file	LPC1227_TargetConfig.h
 * @purpose
 * @brief       specifies target configuration settings for the LPC1227
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

#ifndef __IEC60335_LPC1227_USER_CFG_H
#define __IEC60335_LPC1227_USER_CFG_H

#if !defined(ASM_ARM)
  #include "LPC12xx.h"
  #include "lpc12xx_led.h"
#endif
	
/***********************************************/
/* system settings */
/***********************************************/
  #define CPU_CLK 4000000 /* 4 MHz IRC clock */

  /* pick Port P0_7 for the fault led */
  #define PIOxy_FAULT_DIR     (0x1 << 7)    
  #define PIOxy_FAULT_DIRADD  (0x50000020)   
  #define PIOxy_FAULT_SET     (0x1 << 7)      
  #define PIOxy_FAULT_SETADD  (0x50000008) 
    
  #define LED_GPIO    LPC_GPIO0
  #define LED_BIT     7

/*****************************************************************************/
/* flash memory settings */
/*****************************************************************************/
  /* 128 K on LPC1227 */
  #define FLASH_SIZE  SIZE128K 

  #define SELECTED_CRC_TYPE  CCITT_ALGO
  
  /* these are predefined for LPC1227 target in IEC60335_B_Config.h */
  /* could override them by #undef and #define */
  /*
  #define CRC_MODE_ADDR     TARGET_CRC_MODE_ADDR EQU 0x50070000
  #define CRC_SEED_ADDR     TARGET_CRC_SEED_ADDR EQU 0x50070004
  #define CRC_SUM_ADDR      TARGET_CRC_SUM_ADDR  EQU 0x50070008
  #define CRC_WR_DATA_ADDR  TARGET_WD_DATA_ADDR  EQU 0x50070008
  */
  
/*****************************************************************************/
/* ram settings */
/*****************************************************************************/
  
  /* These define the maximum ranges testable for POST */
  /* at POST the whole ram gets tested */
  #if (defined(ASM_ARM))

    EXTERN |IEC60335_BOTTOM_RAM_POST_MARCH|
    EXTERN |IEC60335_BOTTOM_RAM_POST_MARCH|  

  #else

    extern int IEC60335_TOP_RAM_POST;
    extern int IEC60335_BOTTOM_RAM_POST;

  #endif


  /* march POST test is destructive and covers all ram */
  #define IEC60335_BOTTOM_RAM_POST_MARCH    (0x10000000)
  #define IEC60335_TOP_RAM_POST_MARCH       (0x10002000)


/*****************************************************************************/  
/*	End of user target configuration                                     */
/*****************************************************************************/

#endif
