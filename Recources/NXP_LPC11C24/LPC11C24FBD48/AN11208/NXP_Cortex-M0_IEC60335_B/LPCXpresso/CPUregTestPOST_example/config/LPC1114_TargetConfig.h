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

/*****************************************************************************/
/* ram settings */
/*****************************************************************************/
/*****************************************************************************/
/* User target configuration                                                 */
/*****************************************************************************/
/* the following includes are target dependant and provide the necessary     */
/* system level definitions. They need to be modified by the end user to     */
/* specify the target device. Default build rules for LPC1227 and LPC114     */
/* are provided in the project setup                                         */

//#if defined (LPC1114)
//	#include "LPC1114_TargetConfig.h"
//#endif
//
//#if defined (LPC1227)
//	#include "LPC1227_TargetConfig.h"
//#endif

/* user config checks */
/*****************************************************************************/
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
