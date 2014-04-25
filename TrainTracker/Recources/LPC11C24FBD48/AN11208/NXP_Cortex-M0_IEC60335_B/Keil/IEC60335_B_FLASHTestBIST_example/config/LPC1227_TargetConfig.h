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
  #define LED_GPIO    LPC_GPIO0
  #define LED_BIT     7

  /* pick Port P0_7 for the fault led */
  #define PIOxy_FAULT_DIR     (0x1 << 7)    
  #define PIOxy_FAULT_DIRADD  (0x50008000)    
  #define PIOxy_FAULT_SETADD  (0x50000008 | PIOxy_FAULT_DIR) 
  #define PIOxy_FAULT_SET     (0x1 << 7) 
    

/*****************************************************************************/
/* flash memory settings */
/*****************************************************************************/
	/* 128 K on LPC1227 */
	#define FLASH_SIZE  SIZE128K 
 
	#define SELECTED_CRC_TYPE  CCITT_ALGO
	
	/* Start address*/
	#define IEC60335_BOTTOM_ROM_POST  		(0x000)
	/* Stop address*/
	#define IEC60335_TOP_ROM_POST 			(0x700)
	
	#define MISR_SIGNATURE_ADDR 		(0x1000)
	#define CRC_SIGNATURE_ADDR 			(0x1000)
	
	#define FMSSTART_ADDR  TARGET_FMSSTART   EQU 0x4003C020
	#define FMSSTOP_ADDR   TARGET_FMSSTOP    EQU 0x4003C024
	#define FMSW0_ADDR     TARGET_FMSW0      EQU 0x4003C02C
	#define FMSW1_ADDR     TARGET_FMSW1      EQU 0x4003C030
	#define FMSW2_ADDR     TARGET_FMSW2      EQU 0x4003C034
	#define FMSW3_ADDR     TARGET_FMSW3      EQU 0x4003C038
	#define FMSTAT_ADDR    TARGET_FMSTAT     EQU 0x4003CFE0
	#define FMSTATCLR_ADDR TARGET_FMSTATCLR  EQU 0x4003CFE8
	
	/* Crc generation engine on LPC12x */
	#define CRC_MODE_ADDR     TARGET_CRC_MODE_ADDR EQU 0x50070000
	#define CRC_SEED_ADDR     TARGET_CRC_SEED_ADDR EQU 0x50070004
	#define CRC_SUM_ADDR      TARGET_CRC_SUM_ADDR  EQU 0x50070008
	#define CRC_WR_DATA_ADDR  TARGET_WD_DATA_ADDR  EQU 0x50070008
  
///*****************************************************************************/
///* ram settings */
///*****************************************************************************/
//
//  /* These define the maximum ranges testable for POST */
//  /* at POST the whole ram gets tested */
//  extern int IEC60335_TOP_RAM_POST;
//  extern int IEC60335_BOTTOM_RAM_POST;
//
//  /* march POST test is destructive and covers all ram */
//  #define IEC60335_TOP_RAM_POST_MARCH    &IEC60335_TOP_RAM_POST
//  #define IEC60335_BOTTOM_RAM_POST_MARCH &IEC60335_BOTTOM_RAM_POST
//  
//  /* These define the maximum ranges testable for POST */
//  extern int IEC60335_TOP_RAM_BIST;
//  extern int CSTACK$$Limit;
//
//  /* march BIST test is destructive and covers all unused RAM segments */
//  /* defined in RamBistMarchTestTable */
//  #define IEC60335_TOP_RAM_BIST_MARCH    &IEC60335_TOP_RAM_BIST
//  #define IEC60335_BOTTOM_RAM_BIST_MARCH (&CSTACK$$Limit)
//  #define IEC60335_RAM_SIZE_BIST_MARCH   (IEC60335_TOP_RAM_BIST_MARCH - IEC60335_BOTTOM_RAM_BIST_MARCH + 1)
//
//  /* these define ranges of memory to be tested in smaller chunks for march bist */  
//  /* for test these are the same as the whole ram */
//  #define NUM_RAM_MARCH_TEST_BLOCKS 2
//
//  #define IEC60335_MARCH_SEG1_START IEC60335_BOTTOM_RAM_BIST_MARCH
//  #define IEC60335_MARCH_SEG1_END   IEC60335_TOP_RAM_BIST_MARCH
//  
//  #define IEC60335_MARCH_SEG2_START IEC60335_BOTTOM_RAM_BIST_MARCH
//  #define IEC60335_MARCH_SEG2_END   IEC60335_TOP_RAM_BIST_MARCH  
//
//#if defined(__ICCARM__)
//  extern const ADDR_TYPE RamBistMarchTestTable[NUM_RAM_MARCH_TEST_BLOCKS][2];
//#endif


/*****************************************************************************/  
/*	End of user target configuration                                     */
/*****************************************************************************/

#endif
