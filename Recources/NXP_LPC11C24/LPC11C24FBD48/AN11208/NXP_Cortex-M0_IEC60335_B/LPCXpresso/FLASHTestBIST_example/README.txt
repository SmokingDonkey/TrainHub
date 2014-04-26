/************************************************************************/
/* 	Quick Start-up Guide for the NXP Cortex-M0 IEC60335 Class B Library */
/*----------------------------------------------------------------------*/
/*	Flash test example (BIST)											*/
/************************************************************************/

---- WELCOME ------------------------------------------------------------
This example code will show how to implement the IEC60335 Class B 
Flash BIST test.
This quick start guide explains how this example project is setup and 
how it can be configured and used best.


---- EXAMPLE DIRECTORY STRUCTURE ----------------------------------------
 FLASHTestBIST_example						
	+ src					-> 	Folder containing this README file 
	|
	+ config				->	Folder with configuration source files
	|							(See description below)
	+ Debug					->	Output folder of the compiler while 
								in Debug mode

---- CONFIGURATION ------------------------------------------------------
A number of configurations is possible for this examples, please find 
here some more explanation. 
1. 	Select your target with the Target Select drop down box
		-> 	LPC1114 Flash (default)

   	This will set some project definitions and select the corresponding 
   	source files.
2. 	Configuration files:
2.1	IEC60335_B_UserData.c/.h:
	This source file contains IEC60335 specific configurations
2.2	LPC1xxx_TargetConfig.c/.h:
	These source files contains target specific configurations 

---- ADDITIONAL INFORMATION ---------------------------------------------
The user is able to handle a failing test by creating a so called 
hook function. An example how to use this hook function is shown in 
flashPostFailure_userHookFunction_ARM.s.

In LPC1xxx_TargetConfig.c the reference signatures can be found to 
which the calculated signature (or CRC) will be compared.

Note: there are the following possible definitions for the flash signature options:
LPC111x: MISR_FLASH_CRC

All of them are referenced within the LPC1xxx_TargetConfig.c file.

The values of the signatures depend on the compiler and linker version number thus 
might need to be updated from the default values provided within the example application. 
The computed values will also be different whenever any modifications are made inside the application code.

To determine the signature, either:

- Run the test application once, to find out the actual signature, then substitute the 
actual value into the appropriate #define directive. For convenience, place a breakpoint at 
the routine check loop labels (_misr_hw_sigcheck, _crc16_verify, _crc32_verify) within the 
file IEC60335_B_FlashTestxxx.s. Then copy the computed values back in the signature definition


- calculate the signature automatically, and patch it in the desired location, by using the build tools

- use a custom script to patch the image before it gets burned in flash
	
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