/**
 * @file	IEC60335_B_UserData.h
 * @purpose
 * @brief       used to hold ram segment not subject to test for 
 *              backup variables and status information
 * @version	1.0
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

#ifndef IEC60335_B_USER_DATA_H_
#define IEC60335_B_USER_DATA_H_

/* used for secure variables */
extern type_secured_UINT32 SensVar;
extern type_secured_UINT32 SensVarCopy;

/***********************************************************/
/* Used for register and ram tests */
/***********************************************************/
/* structures holding result of tests */
extern type_testResult CPUregTestPOST_struct;
extern IEC60335_CPUreg_struct CPUregTestBIST_struct;

/* memory location for status after POST */
extern type_testResult  RamPostTestStatus;
extern type_testResult  RamBistTestStatus;


#endif

