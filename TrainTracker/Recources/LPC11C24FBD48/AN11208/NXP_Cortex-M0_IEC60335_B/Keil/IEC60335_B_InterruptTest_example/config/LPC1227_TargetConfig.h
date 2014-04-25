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

#if defined(__ICCARM__) | (__CC_ARM__)
  #include "LPC12xx.h"
  #include "lpc12xx_led.h"
#endif
	
/***********************************************/
/* system settings */
/***********************************************/ 
  #define CPU_CLK 4000000 /* 4 MHz IRC clock */
  #define LED_GPIO    LPC_GPIO0
  #define LED_BIT     7
  

/***********************************************/
/* flash memory settings */
/***********************************************/
  #define FLASH_SIZE 		SIZE128K /* 128 K on LPC1227 */
  #define FLASH_CRC_ADDR	(FLASH_SIZE - 0xF)
  #define ENTRY_FLASH_CRC	{0x63B29505,0x4584156C,0x6C35D996,0x59921E02}
  #define SELECTED_CRC_TYPE	CCITT_ALGO
/***********************************************/
/* ram settings */
/***********************************************/


/*	End of user configuration */
#endif
