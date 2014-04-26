/************************************************************************/
/* 	Quick Start-up Guide for the NXP Cortex-M0 IEC60335 Class B Library */
/*----------------------------------------------------------------------*/
/*	RAM test example (BIST)											*/
/************************************************************************/

---- WELCOME ------------------------------------------------------------
This example code will show how to implement the IEC60335 Class B 
RAM BIST test.
This quick start guide explains how this example project is setup and 
how it can be configured and used best.


---- EXAMPLE DIRECTORY STRUCTURE ----------------------------------------
 RAMTestBIST_example						
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
		->	LPC1227 Flash
   	This will set some project definitions and select the corresponding 
   	source files.
2. 	Configuration files:
2.1	IEC60335_B_UserData.c/.h:
	This source file contains IEC60335 specific configurations
2.2	LPC1xxx_TargetConfig.c/.h:
	These source files contains target specific configurations 

---- ADDITIONAL INFORMATION ---------------------------------------------

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! NOTE this test is destructive for the RAM Content tested	       !!
!! Please take precaution in backing up RAM to be tested	       !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
																		 
The user is able to handle a failing test by creating a so called 
hook function. An example how to use this hook function is shown in 
ramBistFailure_userHookFunction_ARM.s.

	
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