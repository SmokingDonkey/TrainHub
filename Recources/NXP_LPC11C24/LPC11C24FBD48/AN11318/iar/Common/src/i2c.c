/*****************************************************************************
 *   i2cslave.c:  I2CSlave C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Preliminary version, first Release
 *
*****************************************************************************/
#include "LPC13xx.h"				/* LPC134x Peripheral Registers */
#include "type.h"
#include "i2c.h"
#include "app_pmbus.h" 				/* TK PMBus */
#include "gpio.h"

volatile i2c_struct	i2c;        	// The I2C buffers and indx
extern pmbus_commands       pmbus;	// command code
extern unsigned char cmd_ok[256];
extern char const crctable[256];


/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with PMBus slave mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C_IRQHandler(void) 
{
	uint8_t StatValue;

	/* this handler deals with slave read and slave write only */
	StatValue = LPC_I2C->STAT;
	switch ( StatValue & 0xF8)
	{
	case 0x60:					/* An own SLA_W has been received. */
	case 0x68:
		LPC_GPIO0->IE |= (1<<7);	//enable STOP condition detection
		if ((i2c.status	& I2C_READ) ||  (i2c.status	& I2C_MSG)) 	// NO I2C_MSG and NO I2C_READ I2C message is NOT been processed in main loop
		{
			pmbus.status_word	|= 0x80; // set Busy bit in status
			LPC_I2C->CONCLR = I2CONCLR_AAC;			/* assert NACK  */
			LPC_I2C->CONCLR = I2CONCLR_SIC;

		} else
		{
			i2c.buf_indx = 0;	// set in-buffer pointer to beginning of buffer
			i2c.status = 0;	// clear busy bit
			LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after SLV_W is received */
			LPC_I2C->CONCLR = I2CONCLR_SIC;
		}
		break;
	case 0x80:					/*  data receive */
		i2c.buf[i2c.buf_indx] = LPC_I2C->DAT;	// read the data, save it
		{
			if (i2c.buf_indx <= IIC_BUF_SIZE) i2c.buf_indx++;
			LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after SLV_W is received */
			LPC_I2C->CONCLR = I2CONCLR_SIC;
		}
		break;
	case 0xA8:					// A8 = slave addr & rd, ack sent
	case 0xB0:					// B0 = slave addr & rd, ack sent, arbt loss
		// setup buffer to send data
		if (i2c.status & I2C_READ) // is this a supported read command  Is I2C message being processed in main loop?
		{
			if ( LPC_I2C->ADR0 == SMBUS_ARA)	// are we in ARA mode?
			{

				i2c.buf_indx_out = i2c.buf_indx; // set output pointer to I2C addres
				i2c.buf[i2c.buf_indx++] = i2c.i2c_address_and_pin;  // ARA resonse with my I2C address
				i2c.crc = crctable[(0x00 ^ SMBUS_ARA) & 0xFF];	// initial CRC value for ARA I2C address;
				LPC_I2C->ADR0 = i2c.i2c_address_and_pin; // ARA finished so go back to my real I2C addr
			}else
			{
				process_cmd(i2c.buf[0]); // process write command
				i2c.crc = crctable[(0x00 ^ i2c.i2c_address_and_pin) & 0xFF];	// initial CRC value for our I2C address;
			}
			CRC8_BlockChecksum(i2c.buf_indx);  		// check CRC_8 on received I2C data.
			i2c.buf[i2c.buf_indx] = i2c.crc;		// put the CRC-8 in the i2c buffer. if requested by the host it can be send.
			LPC_I2C->DAT = i2c.buf[i2c.buf_indx_out++];	// write the data
		}
		else
		{
			pmbus.status_cml |= 0x01; // A communication error
		}
		LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after SLV_R is received */
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

	case 0xB8:					/* Data byte has been transmitted */
		LPC_I2C->DAT = i2c.buf[i2c.buf_indx_out];	// write the data
		if (i2c.buf_indx_out <= IIC_BUF_SIZE) i2c.buf_indx_out++;
		LPC_I2C->CONSET = I2CONSET_AA;		/* assert ACK  */
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;
	case 0xC8: 		// we're done sending so what to do next ?
		i2c.status &= 0xEC; // clear I2C_MSG and I2C_READ and I2C_BUSSY done with processing ready for next packet.
		LPC_I2C->CONSET = I2CONSET_AA;		/* assert ACK  */
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;
	case 0x88: /* Previously addressed with own SLA; DATA byte has been received; NOT ACK has been returned. */
		LPC_I2C->CONCLR = I2CONCLR_AAC;			/* assert NACK  */
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;
	case 0xC0:	/* Data byte has been transmitted, NACK */
		LPC_I2C->CONSET = I2CONSET_AA;		/* assert ACK  */
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		i2c.status &= 0xEC; // clear I2C_MSG and I2C_READ and I2C_BUSSY done with processing ready for next packet.
		break;
	case 0xA0:					/* Stop condition or repeated start has */
		if (LPC_I2C->ADR0 == SMBUS_ARA)	// are we in ARA mode?
		{
			i2c.status |= I2C_READ;			    // This is a read.
		}else if ((i2c.buf[1] == 2) && (i2c.buf[0] == PMBUS_COEFFICIENTS))
		{
			i2c.status |= I2C_READ;			    // This is a read of the PMBUS_COEFFICIENTS.
		}else if ((i2c.buf_indx == 1) &&
				(i2c.buf[0] != PMBUS_STORE_DEFAULT_ALL) &&
				(i2c.buf[0] != PMBUS_RESTORE_DEFAULT_ALL) &&
				(i2c.buf[0] != PMBUS_STORE_USER_ALL) &&
				(i2c.buf[0] != PMBUS_RESTORE_USER_ALL) &&
				(i2c.buf[0] != PMBUS_CLEAR_FAULTS) &&
				(i2c.buf[0] != PMBUS_TIME_RESET) &&
				(i2c.buf[0] != PMBUS_PIP_RESET))
		{
			i2c.status |= I2C_READ; // This is a read.
		} else if (i2c.buf[0] == PMBUS_QUERY)
		{
			i2c.status |= I2C_READ;			    // This is a read.
		}

		if (!( i2c.status & I2C_READ))
		{
			i2c.status |= I2C_MSG;				// This is a Write command signal the mail loop to handle it

		}
		LPC_I2C->CONSET = I2CONSET_AA;	/* been received, assert ACK.  */
		LPC_I2C->CONCLR = I2CONCLR_SIC;


		break;

	default:
		LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;
		LPC_I2C->CONSET = I2CONSET_I2EN | I2CONSET_SI;
		i2c.status &= 0x6C; // clear I2C_MSG and I2C_READ, I2C_BUSSY, and I2C_STOP done with processing ready for next packet.
		break;
	}
	return;

}

/*****************************************************************************
** Function name:		I2CSlaveInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
void I2CSlaveInit( void ) 
{
#if 1
  LPC_SYSCON->PRESETCTRL |= (0x1<<1);
#else
  LPC_SYSCON->PRESETCTRL |= (0x1<<0);
#endif
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
  LPC_IOCON->PIO0_4 &= ~0x3F;	/*  I2C I/O config */
  LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL */
  LPC_IOCON->PIO0_5 &= ~0x3F;	
  LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA */

  /*--- Clear flags ---*/
  LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
#if FAST_MODE_PLUS
  LPC_IOCON->PIO0_4 |= (0x1<<9);	//scl
  LPC_IOCON->PIO0_5 |= (0x1<<9);	//sda
  LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
  LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
#else
  LPC_I2C->SCLL   = I2SCLL_SCLL;
  LPC_I2C->SCLH   = I2SCLH_SCLH;
#endif

  LPC_I2C->ADR0 = i2c.i2c_address_and_pin;

  /* Enable the I2C Interrupt */
  NVIC_EnableIRQ(I2C_IRQn);

  LPC_I2C->CONSET = I2CONSET_I2EN | I2CONSET_AA;
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

