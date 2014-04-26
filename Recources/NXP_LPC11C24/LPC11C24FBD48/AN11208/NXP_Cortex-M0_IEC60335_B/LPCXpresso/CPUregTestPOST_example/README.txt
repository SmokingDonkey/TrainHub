/************************************************************************/
/* 	Quick Start-up Guide for the NXP Cortex-M0 IEC60335 Class B Library */
/*----------------------------------------------------------------------*/
/*	CPU Register (POST) test example									*/
/************************************************************************/

---- WELCOME ------------------------------------------------------------
This example code will show how to implement the IEC60335 Class B 
CPU register test in POST mode.
This quick start guide explains how this example project is setup and 
how it can be configured and used best.


---- EXAMPLE DIRECTORY STRUCTURE ----------------------------------------
 CPUregTestBIST_example						
	+ src					-> 	Folder containing this README file 
	|
	+ config				->	Folder with configuration source files
	|							(See description below)
	+ Debug					->	Output folder of the compiler while 
								in Debug mode

---- CONFIGURATION ------------------------------------------------------
A number of configurations is possible for this examples, please find 
here some more explanation. 
1. 	Configuration files:
1.1	IEC60335_B_UserData.c/.h:
	This source file contains IEC60335 specific configurations
1.2	LPC1xxx_TargetConfig.c/.h:
	These source files contains target specific configurations 

---- ADDITIONAL INFORMATION ---------------------------------------------
Since POST testing should be done prior to any initialization, the 
test-function is called prior to the system init and therefore the branch 
to main.
The test-function, _CPUregTestPOST(), is called from the ResetISR function
in the cr_startup_lpc11.c file 
The _CPUregTestPOST test function is a library function and will not be 
shown during step through in debug mode.
There are two ways of determining whether the test passes:
1. If the test fails, it will branch to the _CPUpostTestFailureHook in the 
cpuPostFailure_userHookFunction_GNU.asm file.
2. Looking to the CPUregTestPOSTStatus variable in the Variable viewer, 
one will see the status changes to IEC60335_testPasses.
	
---- INFORMATION --------------------------------------------------------
- 	Keep the NXP Cortex-M0 IEC60335 Class B application note
	nearby while developing your IEC60335 Class B application	

---- DISCLAIMER ---------------------------------------------------------	
/***********************************************************************
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