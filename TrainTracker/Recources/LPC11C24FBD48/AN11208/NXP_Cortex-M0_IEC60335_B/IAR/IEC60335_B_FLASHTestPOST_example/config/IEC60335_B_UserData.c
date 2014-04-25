/**
 * @file	IEC600335_B_UserData.c
 * @purpose
 * @brief
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

/* used for secure variables */
#pragma object_attribute=__root
type_secured_UINT32 SensVar;

#pragma object_attribute=__root
type_secured_UINT32 SensVarCopy;

/* variable used for CPU reg tests (POST) */
#pragma object_attribute=__no_init __root
type_testResult CPUregTestPOSTStatus;

/* variable used for CPU reg tests (BIST) */
#pragma object_attribute=__root
IEC60335_CPUreg_struct CPUregTestBIST_struct;

/* variable used for RAM test (POST) */
#pragma object_attribute=__no_init __root
type_testResult  RamPostTestStatus;

/* variable used for RAM test (BIST) */
#pragma object_attribute=__root
type_testResult  RamBistTestStatus;

/* variable used for flash test (POST) */
#pragma object_attribute=__no_init __root
type_testResult  FlashPostTestStatus;

/* variable used for flash test (BIST) */
__root
type_testResult  FlashBistTestStatus;

