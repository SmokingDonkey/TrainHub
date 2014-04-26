/**********************************************************************
* @file		main.c
* @brief	reference project file for testing of AES crypt api
* @version	1.0
* @date		03.07.2012
* @author	nxp28536
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/******************************************************************************
 * Include files
 *****************************************************************************/


#ifdef TARGET_LPC11A

/* LPC11Axx definitions */
#include "lpc11axx.h"
	
/* Specific for IAR LPC11A-SK */
#include "iar_lpc11a_sk.h"
	
#endif

#ifdef TARGET_LPC1769

/* LPC1769 definitions */
#include "lpc17xx.h"
	
/* Specific for Keil MCB 1769 */
#include "keil_lpc1769_mcb.h"
	
#endif




/* include the crypt lib */
#include "cypher.h"


#ifdef TEST_MODE

#ifdef AES128_SUPPORT
extern CypherConfigStr testConfigAes128_test1;
extern CypherConfigStr testConfigAes128_test2;
extern CypherConfigStr testConfigAes128_test3; 
extern plainData_t plainText_AES128_test1[PLAIN_BSIZE];
extern plainData_t plainText_AES128_test2[PLAIN_BSIZE];
extern cryptData_t cryptResult_AES128[CRYPT_BSIZE];
extern plainData_t deCryptResult_AES128[PLAIN_BSIZE];
#endif

#ifdef AES192_SUPPORT
extern CypherConfigStr testConfigAes192_test1;
extern CypherConfigStr testConfigAes192_test2;
extern CypherConfigStr testConfigAes192_test3; 
extern plainData_t plainText_AES192_test1[PLAIN_BSIZE];
extern plainData_t plainText_AES192_test2[PLAIN_BSIZE];
extern cryptData_t cryptResult_AES192[CRYPT_BSIZE];
extern plainData_t deCryptResult_AES192[PLAIN_BSIZE];
#endif

#ifdef AES256_SUPPORT
extern CypherConfigStr testConfigAes256_test1;
extern CypherConfigStr testConfigAes256_test2;
extern CypherConfigStr testConfigAes256_test3; 
extern plainData_t plainText_AES256_test1[PLAIN_BSIZE];
extern plainData_t plainText_AES256_test2[PLAIN_BSIZE];
extern cryptData_t cryptResult_AES256[CRYPT_BSIZE];
extern plainData_t deCryptResult_AES256[PLAIN_BSIZE];
#endif


#endif



/******************************************************************************
 * Custom test buffers
 *****************************************************************************/
/* buffer size MUST be a multiple of the 16-bytes AES block */
#define BUFFER_SIZE (2*PLAIN_BSIZE)

plainData_t myPlainText[BUFFER_SIZE] __attribute__ ((aligned (4))) = "Cortex-M0 does AES-256,eureka !!";
cryptData_t scrambledText[BUFFER_SIZE] __attribute__ ((aligned (4))) = "";
plainData_t recoveredText[BUFFER_SIZE] __attribute__ ((aligned (4))) = "";

#ifdef AES128_SUPPORT
key_t 		passPhrase128[AES128_KEYSIZE] = "!!Abra-Cadabra!!";
CypherConfigStr testConfigAes128_customTest1 = { AES_128, &passPhrase128[0] };
#endif

#ifdef AES192_SUPPORT
key_t 		passPhrase192[AES192_KEYSIZE] = "!!Abra-Cadabra!!Memento!";
CypherConfigStr testConfigAes192_customTest1 = { AES_192, &passPhrase192[0] };
#endif

#ifdef AES256_SUPPORT
key_t 		passPhrase256[AES256_KEYSIZE] = "!!Abra-Cadabra....Sim-sala-bim!!";
CypherConfigStr testConfigAes256_customTest1 = { AES_256, &passPhrase256[0] };
#endif


ERROR_CODE status = OK;
void runCustomTest(void) {


	uint8_t i;

	#ifdef AES128_SUPPORT

		for(i=0;i<5;i++) {

			initCrypt(&testConfigAes128_customTest1);	
			crypt(&myPlainText[0], BUFFER_SIZE, &scrambledText[0]);

			initDeCrypt(&testConfigAes128_customTest1);
			deCrypt(&scrambledText[0], BUFFER_SIZE, &recoveredText[0]);
		}
		
		status = checkErrors();
		
		/* everything fine = LED GREEN */		
		if( status == OK) toggleLed(LED_OK); 
			
	#endif

	#ifdef AES192_SUPPORT

		for(i=0;i<5;i++) {

			initCrypt(&testConfigAes192_customTest1);	
			crypt(&myPlainText[0], BUFFER_SIZE, &scrambledText[0]);

			initDeCrypt(&testConfigAes192_customTest1);
			deCrypt(&scrambledText[0], BUFFER_SIZE, &recoveredText[0]);
		}
		
		status = checkErrors();
		
		/* everything fine = LED GREEN */		
		if( status == OK) toggleLed(LED_OK); 

	#endif

	#ifdef AES256_SUPPORT

		for(i=0;i<5;i++) {

			initCrypt(&testConfigAes256_customTest1);	
			crypt(&myPlainText[0], BUFFER_SIZE, &scrambledText[0]);

			initDeCrypt(&testConfigAes256_customTest1);
			deCrypt(&scrambledText[0], BUFFER_SIZE, &recoveredText[0]);

		}
		
		status = checkErrors();
		
		/* everything fine = LED GREEN */		
		if( status == OK) toggleLed(LED_OK); 

	#endif

}


int main() {
	
	/****************************************************************************
	* System setup routines
	*****************************************************************************/
	SystemCoreClockUpdate();

	/* Called for system library in core_cmx.h(x=0 or 3). */
	SysTick_Config(SystemCoreClock/1000);
  
#ifdef TARGET_LPC11A
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
#endif

	/* configure visual feedback */
	setupLeds();




	/****************************************************************************
	* AES tests
	*****************************************************************************/


#ifdef TEST_MODE

	/******************************************************************************
	* Specifications reference tests
	*****************************************************************************/

	runFips197Test();
	
	status = checkErrors();
	if( status != OK) {

		setVisualError();

	} else {
	
		setLed(LED_OK);
	};
	
#else

	/******************************************************************************
	* Custom test
	*****************************************************************************/
	while(1) {
	
		runCustomTest();
		status = checkErrors();
		if( status != OK) {

			setVisualError();
			
			break;
		};
	}
	
#endif
	
	while(1);

}






/* SysTick interrupt happens every 1 ms */
volatile uint32_t TimeTick = 0;
void SysTick_Handler(void)
{
	TimeTick++;
	if(TimeTick == 1000) {
		
		TimeTick = 0;
		toggleLed(LED_BLINK);
		
	}
		
}




