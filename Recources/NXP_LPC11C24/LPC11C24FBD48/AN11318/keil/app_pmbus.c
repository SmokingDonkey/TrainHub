/*********************************************************************************************************
 *
 * File: app_pmbus.c
 * Description: Creates a task to handle the PMBus (Power Management Bus) application.
 *
 * Software that is described herein is for illustrative purposes only which
 * provides customers with programming information regarding the products.
 * This software is supplied "AS IS" without any warranties. NXP Semiconductors
 * assumes no responsibility or liability for the use of the software, conveys no
 * license or title under any patent, copyright, or mask work right to the
 * product. NXP Semiconductors reserves the right to make changes in the
 * software without notification. NXP Semiconductors also make no representation
 * or warranty that such application will be suitable for the specified use without
 * further testing or modification.
 *********************************************************************************************************/
#include "LPC13xx.h"
#include "type.h"
#include "timer32.h"
#include "i2c.h"
#include "gpio.h"
#include <math.h>

#include "app_pmbus.h"

extern  volatile uint8_t   PMBus_I2C_Address;
extern  volatile i2c_struct   i2c;        // The I2C buffers and indx

unsigned char const static_parameter[2] =
{
		0x18,		//  SW major revision number		Sofware version x.x.x.x
		0x01    	//  SW minor revision number
};
/************************************************************************
 *					valid PMBus command array	*
 *									*
 *************************************************************************

This is an array used to determine if the received PMBus command 
is implemented in this system for either reading or writing.

bit 0 = one -> valid for reading
bit 1 = one -> valid for writing

Most commands will have a value of either 0x03 or 0x00, but a few,
such as VOUT MODE, are valid to read but not to write.
 */

unsigned char cmd_ok[256] =
{
		(0  << 3) | 0, (1  << 3) | 3,		// 0x00 - 0x01
		(1  << 3) | 3, (0  << 3) | 2,		// 0x02 - 0x03
		(0  << 3) | 0, (0  << 3) | 0,		// 0x04 - 0x05
		(0  << 3) | 0, (0  << 3) | 0,		// 0x06 - 0x07
		(0  << 3) | 0, (0  << 3) | 0,		// 0x08 - 0x09
		(0  << 3) | 0, (0  << 3) | 0,		// 0x0A - 0x0B
		(0  << 3) | 0, (0  << 3) | 0,		// 0x0C - 0x0D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x0E - 0x0F
		(14 << 3) | 3, (0  << 3) | 2,		// 0x10 - 0x11
		(0  << 3) | 2, (0  << 3) | 0,		// 0x12 - 0x13
		(0  << 3) | 0, (0  << 3) | 2,		// 0x14 - 0x15
		(0  << 3) | 2, (0  << 3) | 0,		// 0x16 - 0x17
		(0  << 3) | 0, (3  << 3) | 1,		// 0x18 - 0x19
		(0  << 3) | 1, (0  << 3) | 0,		// 0x1A - 0x1B
		(0  << 3) | 0, (0  << 3) | 0,		// 0x1C - 0x1D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x1E - 0x1F
		(29 << 3) | 3, (23 << 3) | 6,		// 0x20 - 0x21
		(20 << 3) | 6, (19 << 3) | 6,		// 0x22 - 0x23
		(16 << 3) | 6, (15 << 3) | 6,		// 0x24 - 0x25
		(14 << 3) | 6,  (13  << 3) | 6,		// 0x26 - 0x27
		(0  << 3) | 0, (13 << 3) | 6,		// 0x28 - 0x29
		(12  << 3) | 6, (0  << 3) | 0,		// 0x2A - 0x2B
		(0  << 3) | 0, (0  << 3) | 0,		// 0x2C - 0x2D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x2E - 0x2F
		(0  << 3) | 3, (0  << 3) | 0,		// 0x30 - 0x31
		(0  << 3) | 0, (0  << 3) | 0,		// 0x32 - 0x33
		(0  << 3) | 0, (21 << 3) | 6,		// 0x34 - 0x35
		(20 << 3) | 6, (0  << 3) | 0,		// 0x36 - 0x37
		(20 << 3) | 6, (19 << 3) | 6,		// 0x38 - 0x39
		(18 << 3) | 3, (17 << 3) | 7,		// 0x3A - 0x3B
		(16 << 3) | 7, (0  << 3) | 0,		// 0x3C - 0x3D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x3E - 0x3F
		(18 << 3) | 6, (17 << 3) | 3,		// 0x40 - 0x41
		(16 << 3) | 6, (15  << 3) | 6,		// 0x42 - 0x43
		(14 << 3) | 6,  (13 << 3) | 3,		// 0x44 - 0x45
		(12 << 3) | 6, (11 << 3) | 3,		// 0x46 - 0x47
		(0  << 3) | 0, (0  << 3) | 0,		// 0x48 - 0x49
		(12 << 3) | 6, (0  << 3) | 0,		// 0x4A - 0x4B
		(0  << 3) | 0, (0  << 3) | 0,		// 0x4C - 0x4D
		(0  << 3) | 0, (15 << 3) | 6,		// 0x4E - 0x4F
		(14 << 3) | 3, (13 << 3) | 6,		// 0x50 - 0x51
		(0  << 3) | 0, (0  << 3) | 0,		// 0x52 - 0x53
		(0  << 3) | 0, (15 << 3) | 6,		// 0x54 - 0x55
		(14 << 3) | 3, (13 << 3) | 6,		// 0x56 - 0x57
		(12 << 3) | 6, (11 << 3) | 6,		// 0x58 - 0x59
		(10 << 3) | 3, (9 << 3) | 6,		// 0x5A - 0x5B
		(8 << 3) | 3, (7 << 3) | 6,		// 0x5C - 0x5D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x5E - 0x5F
		(8 << 3) | 7, (7 << 3) | 0,		// 0x60 - 0x61
		(6 << 3) | 7, (5 << 3) | 3,		// 0x62 - 0x63
		(4 << 3) | 7, (0 << 3) | 0,		// 0x64 - 0x65
		(0  << 3) | 0, (0  << 3) | 0,		// 0x66 - 0x67
		(0  << 3) | 0, (0  << 3) | 0,		// 0x68 - 0x69
		(0  << 3) | 0, (0  << 3) | 0,		// 0x6A - 0x6B
		(0  << 3) | 0, (0  << 3) | 0,		// 0x6C - 0x6D
		(0  << 3) | 0, (0  << 3) | 0,		// 0x6E - 0x6F
		(0  << 3) | 0, (0  << 3) | 0,		// 0x70 - 0x71
		(0  << 3) | 0, (0  << 3) | 0,		// 0x72 - 0x73
		(0  << 3) | 0, (0  << 3) | 0,		// 0x74 - 0x75
		(0  << 3) | 0, (0  << 3) | 0,		// 0x76 - 0x77
		(8  << 3) | 1, (19 << 3) | 5,		// 0x78 - 0x79
		(18 << 3) | 1, (18 << 3) | 1,		// 0x7A - 0x7B
		(18  << 3) | 1, (18  << 3) | 1,		// 0x7C - 0x7D
		(18  << 3) | 1, (18 << 3) | 1,		// 0x7E - 0x7F
		(18  << 3) | 1, (18  << 3) | 1,		// 0x80 - 0x81
		(0  << 3) | 0, (0  << 3) | 0,		// 0x82 - 0x83
		(0  << 3) | 0, (0  << 3) | 0,		// 0x83 - 0x85
		(0  << 3) | 0, (0  << 3) | 0,		// 0x86 - 0x87
		(24 << 3) | 4, (19 << 3) | 4,		// 0x88 - 0x89
		(0  << 3) | 0, (23 << 3) | 4,		// 0x8A - 0x8B
		(22 << 3) | 4, (21 << 3) | 4,		// 0x8C - 0x8D
		(20 << 3) | 4, (0  << 3) | 0,		// 0x8E - 0x8F
		(20 << 3) | 4, (19 << 3) | 4,		// 0x90 - 0x91
		(0  << 3) | 0, (0  << 3) | 0,		// 0x92 - 0x93
		(0  << 3) | 0, (0  << 3) | 0,		// 0x94 - 0x95
		(0  << 3) | 0, (0  << 3) | 0,		// 0x96 - 0x97
		(0 << 3) | 1, (25 << 3) | 7,		// 0x98 - 0x99
		(24 << 3) | 7, (23 << 3) | 7,		// 0x9A - 0x9B
		(22 << 3) | 7, (21 << 3) | 7,		// 0x9C - 0x9D
		(20 << 3) | 7, (0  << 3) | 0,		// 0x9E - 0x9F
		(20 << 3) | 7, (19 << 3) | 7,		// 0xA0 - 0xA1
		(18 << 3) | 7, (17 << 3) | 7,		// 0xA2 - 0xA3
		(16 << 3) | 7, (15 << 3) | 7,		// 0xA4 - 0xA5
		(12 << 3) | 7, (11 << 3) | 7,		// 0xA6 - 0xA7
		(10 << 3) | 7, (9 << 3) | 7,		// 0xA8 - 0xA9
		(0  << 3) | 0, (0  << 3) | 0,		// 0xAA - 0xAB
		(0  << 3) | 0, (0  << 3) | 0,		// 0xAC - 0xAD
		(0  << 3) | 0, (0  << 3) | 0,		// 0xAE - 0xAF
		(10 << 3) | 7, (0  << 3) | 0,		// 0xB0 - 0xB1
		(0  << 3) | 0, (0  << 3) | 0,		// 0xB2 - 0xB3
		(0  << 3) | 0, (0  << 3) | 0,		// 0xB4 - 0xB5
		(0  << 3) | 0, (0  << 3) | 0,		// 0xB6 - 0xB7
		(0  << 3) | 0, (0  << 3) | 0,		// 0xB8 - 0xB9
		(0  << 3) | 0, (0  << 3) | 0,		// 0xBA - 0xBB
		(0  << 3) | 0, (0  << 3) | 0,		// 0xBC - 0xBD
		(0  << 3) | 0, (0  << 3) | 0,		// 0xBE - 0xBF
		(0  << 3) | 0, (0  << 3) | 0,		// 0xC0 - 0xC1
		(0  << 3) | 0, (0  << 3) | 0,		// 0xC2 - 0xC3
		(0  << 3) | 0, (0  << 3) | 0,		// 0xC4 - 0xC5
		(0  << 3) | 0, (0  << 3) | 0,		// 0xC6 - 0xC7
		(0  << 3) | 0, (0  << 3) | 0,		// 0xC8 - 0xC9
		(0  << 3) | 0, (0  << 3) | 0,		// 0xCA - 0xCB
		(0  << 3) | 0, (0  << 3) | 0,		// 0xCC - 0xCD
		(0  << 3) | 0, (0  << 3) | 0,		// 0xCE - 0xCF
		(18 << 3) | 7, (17 << 3) | 7,		// 0xD0 - 0xD1
		(16 << 3) | 7, (15 << 3) | 7,		// 0xD2 - 0xD3
		(0  << 3) | 0, (0  << 3) | 0,		// 0xD4 - 0xD5
		(22 << 3) | 3, (0  << 3) | 4,		// 0xD6 - 0xD7
		(0  << 3) | 7, (0  << 3) | 2,		// 0xD8 - 0xD9
		(25 << 3) | 3, (0  << 3) | 0,		// 0xDA - 0xDB
		(0  << 3) | 0, (16 << 3) | 3,		// 0xDC - 0xDD
		(16 << 3) | 3, (13 << 3) | 5,		// 0xDE - 0xDF
		(0  << 3) | 2, (0  << 3) | 0,		// 0xE0 - 0xE1
		(0  << 3) | 0, (0  << 3) | 0,		// 0xE2 - 0xE3
		(0  << 3) | 0, (0  << 3) | 0,		// 0xE4 - 0xE5
		(0  << 3) | 0, (0  << 3) | 0,		// 0xE6 - 0xE7
		(0  << 3) | 0, (0  << 3) | 0,		// 0xE8 - 0xE9
		(0  << 3) | 0, (0  << 3) | 0,		// 0xEA - 0xEB
		(0  << 3) | 0, (0  << 3) | 0,		// 0xEC - 0xED
		(0  << 3) | 0, (0  << 3) | 0,		// 0xEE - 0xEF
		(0  << 3) | 0, (0  << 3) | 0,		// 0xF0 - 0xF1
		(0  << 3) | 0, (0  << 3) | 0,		// 0xF2 - 0xF3
		(0  << 3) | 0, (0  << 3) | 0,		// 0xF4 - 0xF5
		(0  << 3) | 0, (0  << 3) | 0,		// 0xF6 - 0xF7
		(0  << 3) | 0, (0  << 3) | 0,		// 0xF8 - 0xF9
		(0  << 3) | 7, (0  << 3) | 0,		// 0xFA - 0xFB
		(0  << 3) | 0, (0  << 3) | 0,		// 0xFC - 0xFD
		(0  << 3) | 0, (0  << 3) | 0,		// 0xFE - 0xFF
};
/*
Table of CRC-8's of all single-byte values (made by make_crc_table)
on the polynomial: x^8 + x^2 + x^1 + x^0
 */
char const crctable[256] = {
		0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
		0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
		0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
		0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
		0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
		0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
		0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
		0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
		0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
		0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
		0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
		0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
		0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
		0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
		0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
		0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
		0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
		0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
		0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
		0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
		0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
		0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
		0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
		0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
		0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
		0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
		0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
		0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
		0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
		0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
		0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
		0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};


/*
 *********************************************************************************************************
 *                                       LOCAL  VARIABLES
 *********************************************************************************************************
 */
pmbus_commands   			pmbus;		// command code
response_struct				response;
calculation_temperaty_value calc_temp;
timeObject_struct 			g_sTime;
liniar_struct  		 	  	linear;
unsigned char *             PmbusValue;

int  main (void )
{

	SystemCoreClockUpdate();
	
	/* Initialize GPIO domain*/
	GPIOInit();

	//This is for the STOP condition detect
	/* Set PIO0_7 as input with hi-z*/
	GPIOSetDir(PORT0, 7, 0);
	LPC_IOCON->PIO0_7 &= ~(0x3<<3);
	/* Set to PORT0, PIO0_7, edge, single, rising edge*/
	GPIOSetInterrupt(PORT0, 7, 0, 0, 1);

	/* Initialize 32-bit timer 0. TIME_INTERVAL is defined as 10mS */
	/* You may also want to use the Cortex SysTick timer to do this */
	init_timer32(0, TIME_INTERVAL);
	/* Enable timer 0. Our interrupt handler will begin incrementing */
	/* the TimeTick global each time timer 0 matches and resets.   */
	enable_timer32(0);
	PMBus_Defaults();

	if ((pmbus.i2c_address < 0x07) || pmbus.i2c_address == 0xFF)
	{
		pmbus.i2c_address = SMBUS_DEFAULT; 	// SMBUS_DEFAULT default I2C address
		pmbus.operation=0x00;		        // assume configuration settings are corrupt or not available switch off
		pmbus.on_off_config=0x18;	        // assume configuration settings are corrupt or not available switch off
		pmbus.config |= 0x80;			// error indicator for NVRAM settings
	} 

	i2c.i2c_address_and_pin = pmbus.i2c_address;       
	I2CSlaveInit();			/* initialize I2c */
	NVIC_SetPriority(I2C_IRQn, 0);
	NVIC_SetPriority(EINT0_IRQn, 4);
	process_cmd(PMBUS_CLEAR_FAULTS);	// set all pmbus status registers to zero
	while (1) {

		/* process the received I2C buffer when it is a write of parameters */
		if ((i2c.status & I2C_MSG) && (i2c.status & I2C_STOP))// Received I2C packet and go process it now, make sure stop condition is detected
		{
			i2c.crc = 1;
			/* received a 1 byte command can not have CRC by definition  (need 2 bytes)*/
			if (i2c.buf_indx == 1) i2c.crc = 0;   
			else
			{
				if (i2c.buf[0] == PMBUS_COEFFICIENTS) calc_temp.crc_temp = 10;	  /* 10 coefficiant write command */
				else if (cmd_ok[i2c.buf[0]] & 0x04) calc_temp.crc_temp = 3 ;	  /* 2 byte command + 1 for CRC */
				else calc_temp.crc_temp = 2 ;								  /* 1 byte command */
				if (i2c.buf_indx > calc_temp.crc_temp) /* did we receive a CRC-8	*/
				{	
					i2c.crc = crctable[(0x00 ^ pmbus.i2c_address) & 0xFF];	// initial CRC value for our I2C address;			
					CRC8_BlockChecksum(i2c.buf_indx);  /* if yes check CRC_8 on received I2C data and compare to the received CRC-8	   */
				}else i2c.crc = 0; 					   /* we did not receive a CRC. Hence just write */
			}					
			if (i2c.crc == 0) process_cmd(i2c.buf[0]); // process write command 
			else pmbus.status_mfr_specific |= 0x80; // CRC error indication 
			//clear I2C_MSG, I2C_READ, I2C_BUSY, and I2C_STOP
			i2c.status &= 0x6C;
		}

		if (timer32_0_counter = 100)
		{
			timer32_0_counter = 0;
			g_sTime.Second++;
			// update time up.
			if(g_sTime.Second == ((unsigned char)60))  {
				g_sTime.Second = ((unsigned char)0);
				g_sTime.Minute++;
				if(g_sTime.Minute == ((unsigned char)60))  {
					g_sTime.Minute = ((unsigned char)0);
					g_sTime.Hour++;
					if(g_sTime.Hour == ((unsigned int)FIVE_YEARS)) {
						g_sTime.Hour = ((unsigned int)0);
					}
				}
			}
		}

	}
}
/*
*********************************************************************************************************
 *                                            process_cmd()
 *
 * Description : PMBus command processor function.
 *
 * Argument(s) : none.
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */

void process_cmd(unsigned char cmd)
{
  coefficients_struct *temp;
	i2c.buf_indx_out = 1; 	 // used to point to currently processed 1 or 2 byte data.
	

	switch (cmd)
	{
	case	PMBUS_CLEAR_FAULTS:							// * cmd_clear_faults *
	{
		pmbus.status_cml		= 0;
		pmbus.status_vout		= 0;
		pmbus.status_iout 		= 0;
		pmbus.status_input 		= 0;
		pmbus.status_mfr_specific	= 0;
		pmbus.status_other	        = 0;
		pmbus.status_temperature	= 0;
		pmbus.status_word 		= 0;
		pmbus.status_fans_1_2		= 0;
		response.required 		= 0; 	 // clear response in progress

		response.wait_for_clear 	= 0xFF;  // All faults will be handled
			
		break;
	}
	case	PMBUS_STORE_DEFAULT_ALL:							// * cmd_store_default_all *
	{
		WriteEEPROM(PMBUS_FACTORY_SETTINGS,PARMS_MAX);   				// write all factory PARMS into EEPROM
		break;
	}
	case	PMBUS_STORE_USER_ALL:							// * cmd_store_user_all *
	{
		WriteEEPROM(PMBUS_USER_SETTINGS,PARMS_MAX);   // write all user PARMS into EEPROM
		break;
	}

	case	PMBUS_RESTORE_USER_ALL:							// * cmd_restore_user_all *
	{
		ReadEEPROM(PMBUS_USER_SETTINGS,PARMS_MAX);	// read all user values into PARMS
		break;
	}
	case	PMBUS_RESTORE_DEFAULT_ALL:							// * cmd_restore_default_all *
	{
		ReadEEPROM(PMBUS_FACTORY_SETTINGS,PARMS_MAX);
		break;
	}
	case	PMBUS_QUERY:
	{
		if (i2c.status & I2C_READ)	// if read info is send to Host
		{
			i2c.buf[i2c.buf_indx++] = 0x01;    // page 55 chapter 14.1 PMBus
			i2c.buf_indx_out = 3;
			switch (cmd_ok[i2c.buf[2]]& 0x07)
			{
			case 1:
			case 5:
			{
				i2c.buf[i2c.buf_indx++] = 0xAC;  // supported for read only in Direct Mode
				break;
			}
			case 2:
			{
				i2c.buf[i2c.buf_indx++] = 0xCC;  // supported for write in Direct Mode
				break;
			}
			case 3:
			case 7:
			{
				i2c.buf[i2c.buf_indx++] = 0xEC;  // supported for read/write in Direct Mode
				break;
			}
			case 6:
			{
				if (pmbus.config & 0x02) i2c.buf[i2c.buf_indx++] = 0xE0;  // supported for read/write in Linear Mode
				else i2c.buf[i2c.buf_indx++] = 0xEC;  // supported for read/write in Direct Mode
				break;
			}
			case 4:
			{
				if (pmbus.config & 0x02) i2c.buf[i2c.buf_indx++] = 0xA0;  // supported for read/write in Linear Mode
				else i2c.buf[i2c.buf_indx++] = 0xAC;  // supported for read only in Direct Mode
				break;
			}default:
			{
				i2c.buf[i2c.buf_indx++] = 0x0C;  // not supported command
				break;
			}
			}
		}
		break;
	}
	case	PMBUS_COEFFICIENTS:					// 	 cmd_coefficients
		// 	send_or_store_coefficients picks up the data byte
		//  from the inbuf. This is translated into a pmbus.]
		//  index value in a switch() cmd table. Starting with
		//  this index we stroe or output this index plus the next three
	{
		switch ( i2c.buf[2]) // this is the hex num of the varible with the coeficients
		{
		//case PMBUS_VOUT_COMMAND: calc_temp.coefficients = vout_command_scale_lo;break;
		case PMBUS_READ_VOUT: 	        PmbusValue = (unsigned char *) &pmbus.read_vout_scale;break;
		case PMBUS_READ_IOUT: 	        PmbusValue = (unsigned char *) &pmbus.read_iout_scale;break;
		case PMBUS_READ_VIN: 	        PmbusValue = (unsigned char *) &pmbus.read_vin_scale;break;
		case PMBUS_READ_IIN: 	        PmbusValue = (unsigned char *) &pmbus.read_iin_scale;break;
		case PMBUS_READ_TEMPERATURE_1:  PmbusValue = (unsigned char *) &pmbus.read_temp_1_scale;break;
		case PMBUS_READ_TEMPERATURE_2:  PmbusValue = (unsigned char *) &pmbus.read_temp_2_scale;break;
		case PMBUS_READ_USER_AD:        PmbusValue = (unsigned char *) &pmbus.read_user_ad_scale;break;
		default:                        PmbusValue = (unsigned char *) &pmbus.vout_command_scale;break;
		}
		i2c.buf_indx_out = 3;
		if (i2c.status & I2C_READ) // if read info is send to Host
		{
			i2c.buf[i2c.buf_indx++] = 0x05;    // page 55 chapter 14.1 PMBus
			i2c.buf_indx_out = 4;
		}
		temp = (coefficients_struct *)PmbusValue;
		read_or_store_parms(2);
		read_or_store_parms(2);
		read_or_store_parms(1);
		break;
	}
	case	PMBUS_CONFIG_COMMAND:							// 	cmd_config;
	{
		read_or_store_parms (0);
		if (!(i2c.status & I2C_READ)) // This was a write signal host of changed I2C address
		{

		}

		break;
	}
	case	PMBUS_PIP_VERSION:
	{
		i2c.buf[i2c.buf_indx++] = static_parameter[1];	// output buffer for master reading
		i2c.buf[i2c.buf_indx++] = static_parameter[0];	// copy the parameter to the I2C
		break;
	}
	case	PMBUS_TIME_UP:
	{
		i2c.buf[i2c.buf_indx++] = g_sTime.Second;	// output buffer for master reading
		i2c.buf[i2c.buf_indx++] = g_sTime.Minute;	// copy the parameter to the I2C
		i2c.buf[i2c.buf_indx++] = (unsigned char)g_sTime.Hour;
		i2c.buf[i2c.buf_indx++] = (unsigned char)(g_sTime.Hour >> 8);
		break;
	}
	case	PMBUS_TIME_RESET:
	{
		g_sTime.Second = 0;
		g_sTime.Minute =0;
		g_sTime.Hour =0;
		break;
	}
	case 	PMBUS_PIP_RESET:
	{
		break;
	}
	case	PMBUS_MFR_SERIAL:
	{
		i2c.buf[i2c.buf_indx++] = pmbus.mfr_serial;
		i2c.buf[i2c.buf_indx++] = pmbus.mfr_serial;
		break;
	}
	case	PMBUS_PIP_PARM_READ:
	{
		PmbusValue = &pmbus.operation;
		PmbusValue += pmbus.alert_config;
		if (i2c.status & I2C_READ)	// if read info is send to Host
		{
			i2c.buf[i2c.buf_indx++] = *PmbusValue;		//ThK output buffer for master reading
		}
		else
		{
			*PmbusValue = i2c.buf[i2c.buf_indx_out];		// output buffer for master reading
		}
		if (pmbus.alert_config  ==10)
		{
			*PmbusValue = i2c.buf[i2c.buf_indx_out];		// output buffer for master reading
		}

		break;
	}

	case	PMBUS_VOUT_COMMAND:							// * cmd_vout_command *
	case	PMBUS_OPERATION:
	{
		if  (!(i2c.status & I2C_READ))	response.status_bits |= 0x08;	 /*	 the powertrain must be turned on by CTL, PMBUS_OPERATIONs or automatic(set by PMBUS_OPERATION) */
	}
	default:
	{
		read_or_store_parms (0);
		break;
	}
	}
}


/*
 *********************************************************************************************************
 *                                            ms_timer()
 *
 * Description : Synchronous wait function. \param dly_cnt Sleep duration in 1-millisecond units.
 *
 * Argument(s) : none.
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */

void ms_timer(uint16_t dly_cnt)
{  
	response.dly_ms	  =  dly_cnt;
}

/*
 *********************************************************************************************************
 *                                            PMBusTimer()
 *
 * Description : Synchronous wait function. \param dly_cnt Sleep duration in 1-millisecond units.
 *
 * Argument(s) : none.
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void PMBusTimer (void)
{
	if(response.dly_ms > 0)
	{
		response.dly_ms--;   // decrease delay_ms by one
	}
}


/*
 *********************************************************************************************************
 *                                            read_or_store_parms()
 *
 * Description : PMBus .
 *
 * Argument(s) : none.
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void read_or_store_parms (unsigned char bytes)
{

	coefficients_struct *temp;

	unsigned int  Value = 0;


	if (i2c.buf[0] != PMBUS_COEFFICIENTS)
	{
		PmbusValue = &pmbus.operation + ( (unsigned char) i2c.buf[0]-(cmd_ok[i2c.buf[0]] >> 3));
		switch ( i2c.buf[0]) // the command for wich we want to read or write data
		{
		case PMBUS_VOUT_COMMAND:
		case PMBUS_VOUT_MAX:
		case PMBUS_VOUT_MARGIN_HIGH:
		case PMBUS_VOUT_MARGIN_LOW:
		{
			temp = (coefficients_struct *) &pmbus.vout_command_scale;
			bytes |= 0x40;
			break;
		}
		case PMBUS_READ_VOUT:
			//case PMBUS_VOUT_TRIM:
			//case PMBUS_VOUT_CAL_OFFSET:
			//case PMBUS_VOUT_TRANSITION_RATE:
			//case PMBUS_VOUT_SCALE_LOOP:
			//case PMBUS_VOUT_SCALE_MONITOR:
		case PMBUS_VOUT_OV_FAULT_LIMIT:
		case PMBUS_VOUT_OV_WARN_LIMIT:
		case PMBUS_VOUT_UV_WARN_LIMIT:
		case PMBUS_VOUT_UV_FAULT_LIMIT:
		{
			temp = (coefficients_struct *) &pmbus.read_vout_scale;
			bytes |= 0x40;
			break;
		}
		case PMBUS_READ_IOUT:
				//case PMBUS_IOUT_CAL_GAIN:
				//case PMBUS_IOUT_CAL_OFFSET:
				case PMBUS_IOUT_OC_FAULT_LIMIT:
				case PMBUS_IOUT_OC_LV_FAULT_LIMIT:
				case PMBUS_IOUT_OC_WARN_LIMIT: 	{ temp = (coefficients_struct *) &pmbus.read_iout_scale;break;}
				case PMBUS_READ_VIN:
				case PMBUS_VIN_ON:
				case PMBUS_VIN_OFF:
				case PMBUS_VIN_OV_FAULT_LIMIT:
				case PMBUS_VIN_OV_WARN_LIMIT:
				case PMBUS_VIN_UV_WARN_LIMIT:
				case PMBUS_VIN_UV_FAULT_LIMIT:  {temp = (coefficients_struct *) &pmbus.read_vin_scale;break;}
				case PMBUS_READ_IIN:
				case PMBUS_IIN_OC_FAULT_LIMIT:
				case PMBUS_IIN_OC_WARN_LIMIT:   {temp = (coefficients_struct *) &pmbus.read_iin_scale;break;}
				case PMBUS_READ_TEMPERATURE_1:
				case PMBUS_OT_FAULT_LIMIT:
				case PMBUS_OT_WARN_LIMIT:	{temp = (coefficients_struct *) &pmbus.read_temp_1_scale;break;}
				case PMBUS_READ_TEMPERATURE_2: 	{temp = (coefficients_struct *) &pmbus.read_temp_2_scale;break;}
				case PMBUS_READ_USER_AD:
				{
					temp = (coefficients_struct *) &pmbus.read_user_ad_scale;
					break;
				}
				default:  Value = 10; break;
		}
	}



	linear.N = (pmbus.vout_mode & 0x1F); 		 // needed for VOUT_MODE read and write hence here
	if (linear.N & 0x10) linear.N |= 0xF0;		 // needed for VOUT_MODE read and write hence here saves 6 bytes in flash 




	/* Handle read info to be send to Host */
	if (i2c.status & I2C_READ)	// if read info is send to Host
	{

		if ((cmd_ok[i2c.buf[0]] & 0x04) || (bytes & 0x02))
		{
			if ((Value != 10) && (pmbus.config & 0x02)) // Linear Mode ON
			{
				linear.Temp = temp->scale / pow(10,temp->r);
				linear.X = (linear.Temp - temp->offset ) / temp->scale;

				if (((pmbus.vout_mode & 0xE0) == 0) && (bytes & 0x40))
				{
					linear.Temp = linear.X / (pow(2,linear.N));
					linear.Y = (int) linear.Temp;	
				}else
				{
					linear.Temp = 1023 / linear.X;
					if (linear.Temp <0) 
					{
						linear.Temp *= -1; // make Temp positive  	
						linear.sign = 0x00;
					}else linear.sign = 0x01;
					linear.Y = (int) linear.Temp;
					linear.N = 0;
					while (linear.Y  > 1)	
					{	
						linear.Y  = linear.Y  >> 1;
						linear.N++;
					}
					linear.Temp = linear.X * pow(2,linear.N);
					linear.Temp = ceil(linear.Temp);
					if (linear.sign == 0)linear.Temp *=-1;
					else linear.N *= -1;	// set sign bit
					linear.Y = (int) linear.Temp;
					linear.Y |= (linear.N << 11);
				}
				i2c.buf[i2c.buf_indx++] = (unsigned char) linear.Y; 
				i2c.buf[i2c.buf_indx++] = (unsigned char) (linear.Y >> 8);
			}else // direct mode now
			{
				i2c.buf[i2c.buf_indx++] = *PmbusValue++;
				i2c.buf[i2c.buf_indx++] = *PmbusValue++;
			}
		}else
		{
			i2c.buf[i2c.buf_indx++] = *PmbusValue++;
		}
		/* end send readinfo to Host */ 
	}else // if write info is received from Host
	{
		response.status_bits &= ~0x04;  // clear 0x0100
		switch ( pmbus.write_protect)  // can we write the register ?
		{
		case 0x00: response.status_bits |= 0x04;break;
		case 0x40: 
		{
			if ((i2c.buf[0] == PMBUS_WRITE_PROTECT) ||
					(i2c.buf[0] == PMBUS_OPERATION)) response.status_bits |= 0x04 ;
			break;
		}
		case 0x20: 
		{
			if ((i2c.buf[0] == PMBUS_WRITE_PROTECT) ||
					(i2c.buf[0] == PMBUS_OPERATION) ||
					(i2c.buf[0] == PMBUS_ON_OFF_CONFG) ||
					(i2c.buf[0] == PMBUS_VOUT_COMMAND)) response.status_bits |= 0x04 ;
			break;
		}
		default: if (i2c.buf[0] == PMBUS_WRITE_PROTECT) response.status_bits |= 0x04 ;break;// 	case 0x80:
		}
		{
			if ((cmd_ok[i2c.buf[0]] & 0x04) || (bytes & 0x02)) // if this a 2 byte value ?
			{  		
				if ((Value != 10) && (pmbus.config & 0x02)) // Linear Mode ON
				{
					linear.Y = 	i2c.buf[i2c.buf_indx_out++];	 // bit 0 to 7
					linear.Y |=  (i2c.buf[i2c.buf_indx_out++] << 8);
					if (!(((pmbus.vout_mode & 0xE0) == 0) && (bytes & 0x40)))
					{			 	
						linear.N = linear.Y >> 11; 
						linear.Y &= 0x03FF;					 		
					}
					if (linear.N & 0x10) linear.N |= 0xF0;
					linear.X = linear.Y * pow(2,linear.N);
					linear.Temp= (linear.X * temp->scale) + temp->offset;
					linear.Y = linear.Temp * pow(10,temp->r);

					*PmbusValue = linear.Y;	// copy the received I2C data

				}else
				{
					*PmbusValue++ = i2c.buf[i2c.buf_indx_out++];	// copy the received I2C data
					*PmbusValue++ = i2c.buf[i2c.buf_indx_out++];	// copy the received I2C data
				}
			} else
			{
				*PmbusValue++ = i2c.buf[i2c.buf_indx_out++];	// copy the received I2C data
			}
		}
	}
}

/** Write EEPROM to array				*/
void WriteEEPROM(uint16_t adr_start,uint16_t adr_end)
{

} 
/** read EEProm into array variables		 */
void ReadEEPROM(uint16_t adr_start,uint16_t adr_end)
{

}
void CRC8_BlockChecksum( unsigned char length )
{
	unsigned int crc_i;
	for (crc_i = 0; crc_i < length; crc_i++) i2c.crc = crctable[(i2c.crc ^ i2c.buf[crc_i]) & 0xFF];
}

void PMBus_Defaults( void )
{
	pmbus.operation             = 0x80;
	pmbus.on_off_config         = 0x18;
	pmbus.write_protect         = 0x00;
	pmbus.vout_mode             = 0x18;
	pmbus.vout_command_scale    = 0x3a33;
	pmbus.vout_command_offset   = 0xee03;
	pmbus.vout_command_r        = 0xfe;
	pmbus.read_vout_scale       = 0x3cb4;
	pmbus.read_vout_offset      = 0xffab;
	pmbus.read_vout_r           = 0xfe;
	pmbus.vout_command          = 0x00fb;
	/* not used */
	pmbus.i2c_address_and_pin           = 18;
	pmbus.vout_trim                     = 0x22;
	pmbus.vout_cal                      = 0x23;
	/* end not used */
	pmbus.capability            = 0xb0;
	pmbus.vout_max              = 0x034f;
	pmbus.vout_margin_high      = 0x01c9;
	pmbus.vout_margin_low       = 0x0152;
	/* not used */
	pmbus.vout_transition_rate          = 0x0000;
	pmbus.vout_scale_loop               = 0x0000;
	pmbus.vout_scale_monitor            = 0x0000;
	/* end not used */
	pmbus.vin_on                = 0x026e;
	pmbus.vin_off               = 0x0329;
	/* not used */
	pmbus.iout_cal_gain                 = 0x0000;
	pmbus.iout_cal_offset               = 0x0000;
	/* end not used */
	pmbus.fan_config_1_2        = 0xd5;
	pmbus.fan_command_1         = 0xffff;
	pmbus.fan_command_2         = 0x0002;
	pmbus.vout_ov_fault_limit      = 0x0308;
	pmbus.vout_ov_fault_response   = 0x0B9;
	pmbus.vout_ov_warn_limit       = 0x026c;
	pmbus.vout_uv_warn_limit       = 0x009a;
	pmbus.vout_uv_fault_limit      = 0x004b;
	pmbus.vout_uv_fault_response   = 0x00;
	pmbus.iout_oc_fault_limit      = 0x0018;
	pmbus.iout_oc_fault_response   = 0x80;
	pmbus.iout_oc_warn_limit       = 0x0011;
	pmbus.ot_fault_limit           = 0x00e6;
	pmbus.ot_fault_response        = 0x80;
	pmbus.ot_warn_limit            = 0x00bf;
	pmbus.vin_ov_fault_limit       = 0x0368;
	pmbus.vin_ov_fault_response    = 0x00;
	pmbus.vin_ov_warn_limit        = 0x0328;
	pmbus.vin_uv_warn_limit        = 0x02ac;
	pmbus.vin_uv_fault_limit       = 0x026e;
	pmbus.vin_uv_fault_response    = 0x00;
	pmbus.iin_oc_fault_limit       = 0x0043;
	pmbus.iin_oc_fault_response    = 0x00;
	pmbus.iin_oc_warn_limit        = 0x0032;
	pmbus.ton_delay                = 0x00c8;
	/* not used */
	pmbus.ton_rise                         = 0x0000;
	/* end not used */
	pmbus.ton_max_fault_limit      = 0x0064;
	pmbus.ton_max_fault_response   = 0x00;
	pmbus.toff_delay               = 0x0000;
	/* not used */
	pmbus.toff_fall                        = 0x0000;
	/* end not used */
	pmbus.read_iout_scale          = 0x1980;
	pmbus.read_iout_offset         = 0xf913;
	pmbus.read_iout_r              = 0xfd;
	pmbus.read_vin_scale           = 0x1874;
	pmbus.read_vin_offset          = 0xfe48;
	pmbus.read_vin_r               = 0xfe;
	pmbus.read_iin_scale           = 0x7f5c;
	pmbus.read_iin_offset          = 0x0804;
	pmbus.read_iin_r               = 0xfd;
	pmbus.read_temp_1_scale        = 0x04ff;
	pmbus.read_temp_1_offset       = 0x9c19;
	pmbus.read_temp_1_r            = 0xfe;
	pmbus.status_fans_1_2          = 0xa0;
	pmbus.status_word              = 0x0000;
	pmbus.read_fan1                = 0x00;
	pmbus.read_fan2                = 0x00;
	pmbus.status_vout              = 0x00;
	pmbus.status_iout              = 0x00;
	pmbus.status_input             = 0x00;
	pmbus.status_temperature       = 0x00;
	pmbus.status_cml               = 0x00;
	pmbus.status_other             = 0x00;
	pmbus.status_mfr_specific      = 0x00;
	pmbus.pmbus_revision  	       = 0x00;
	pmbus.read_vin 		       = 0x88;
	pmbus.read_iin 		       = 0x89;
	pmbus.read_vout 	       = 0x9b;
	pmbus.read_iout 	       = 0x8c;
	pmbus.read_temperature_1       = 0x8d;
	pmbus.read_temperature_2       = 0x8e;
	pmbus.mfr_id                   = 0x99;
	pmbus.mfr_model                = 0x9a;
	pmbus.mfr_revision             = 0x9b;
	pmbus.mfr_location              =0x9c;
	pmbus.mfr_date                  =0x9d;
	pmbus.mfr_serial                =0x9e;
	pmbus.mfr_vin_min               =0xa0;
	pmbus.mfr_vin_max               =0xa1;
	pmbus.mfr_iin_max               =0xa2;
	pmbus.mfr_pin_max               =0xa3;
	pmbus.mfr_vout_min              =0xa4;
	pmbus.mfr_vout_max              =0xa5;
	pmbus.mfr_iout_max              =0xa6;
	pmbus.mfr_pout_max              =0xa7;
	pmbus.mfr_tambient_max          =0xa8;
	pmbus.mfr_tambient_min          =0xa9;
	pmbus.user_data_00              =0xb0;

	pmbus.read_user_ad_scale        =0x29b8;
	pmbus.read_user_ad_offset       =0x00b4;
	pmbus.read_user_ad_r            =0xfe;
	pmbus.read_temp_2_scale         =0x04ff;
	pmbus.read_temp_2_offset        =0x9c19;
	pmbus.read_temp_2_r             =0xfe;
	/* not used */
	pmbus.mfr_serial_end                    =182;
	/* not used */
	pmbus.fan_1_us_warn_limit       =0x00;
	pmbus.fan_1_us_fault_limit      =0x00;
	pmbus.fan_2_us_warn_limit       =0x00;
	pmbus.fan_2_us_fault_limit      =0x00;
	pmbus.retry_time_ms             =0x0a;
	pmbus.config                    =0x05;
	pmbus.alert_config              =0x00;
	pmbus.i2c_address               =0x26;
	pmbus.read_user_ad 	        =0x0000;
	pmbus.version 		        =0x00;
}
