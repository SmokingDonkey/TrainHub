/**********************************************************************
* $Id$		sbl_slave.c			2012-05-04
*//**
* @file		sbl_slave.c	
* @brief	SBL Slave.
* @version	1.0
* @date		04. May. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
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
#include "includes.h"
#include "sbl_slave.h"

/** Read Function ID sent by master */
SBL_RET_CODE SBL_SlaveReadFuncID(SBL_FUNC_ID* pFnID);
/** Read Firmware data from Master */
SBL_RET_CODE SBL_SlaveRecvFwData(uint32_t* pBlockNum);

/*********************************************************************//**
 * @brief      Initialize RW data
 * @param[in]  None
 * @return     None
 **********************************************************************/
static void initialize_data(void)
{
    uint32_t i = 0;
    /* Init Buffer */
    uint8_t*  Slave_Buffer = (uint8_t*)SBL_RAM_BUFFER_ADDR;
    for(i = 0; i < SBL_FW_DATA_BLOCK_SIZE; i++)
    {
         Slave_Buffer[i] = 0;
    }
}
/*********************************************************************//**
 * @brief      read byte from master
 * @param[out] pByte	pointer to buffer storing received byte.
 * @return     SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
static SBL_RET_CODE read_byte(uint8_t* pByte)
{
    SBL_TRANSER_Type transfer;
    SBL_RET_CODE ret;

    if(pByte == NULL)
        return SBL_INVALID_PARAM;

    /* Read Function ID*/
    transfer.TxBuf = NULL;
    transfer.TxLen = 0;
    transfer.RxBuf = pByte;
    transfer.RxLen = 1;
    transfer.Type = START_STOP_PACKET;
    ret = SBL_ReadWrite(&transfer);
    return ret;
}
/*********************************************************************//**
 * @brief      write byte from master
 * @param[in]  pByte	pointer to buffer storing the byte which will be sent.
 * @return     SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
static SBL_RET_CODE write_byte(uint8_t* pByte)
{
    SBL_TRANSER_Type transfer;
    SBL_RET_CODE ret;

    if(pByte == NULL)
        return SBL_INVALID_PARAM;

    /* Send byte*/
    transfer.TxBuf = pByte;
    transfer.TxLen = 1;
    transfer.RxBuf = NULL;
    transfer.RxLen = 0;
    transfer.Type = START_STOP_PACKET;
    ret = SBL_ReadWrite(&transfer);
    return ret;
}
/*********************************************************************//**
 * @brief      SBL Initialize for Slave
 * @param[in]  none
 * @return     SBL_OK
 **********************************************************************/
SBL_RET_CODE SBL_SlaveInit(void)
{
    SBL_RET_CODE ret;

    ret = SBL_Init();
    if(ret != SBL_OK)
        return ret;

    return ret;    
}

/*********************************************************************//**
 * @brief      Get Function ID from master
 * @param[out] pFnID	pointer to buffer storing received function ID.
 * @return     SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_SlaveReadFuncID(SBL_FUNC_ID* pFnID)
{
    SBL_RET_CODE ret = SBL_ERR;

    ret = SBL_SlaveReady();
    if(ret != SBL_OK)
    {
        return ret;
    }
    ret = read_byte(pFnID);
    
    return ret;
}
/*********************************************************************//**
 * @brief      Send Ready byte
 * @param[in]  None
 * @return     SBL_OK/SBL_INVALID_PARAM
 **********************************************************************/
SBL_RET_CODE SBL_SlaveReady(void)
{
    SBL_RET_CODE ret;
    uint8_t ready_byte = SBL_READY_BYTE;

    ret = write_byte(&ready_byte);
    
    return ret;
}


/*********************************************************************//**
 * @brief        Send Firmware Version ID to master
 * @param[in]    None.
 * @return       SBL_OK/SBL_COMM_ERR
 **********************************************************************/
SBL_RET_CODE SBL_SlaveSendFwVersionID(void)
{
    SBL_TRANSER_Type transfer;
    uint32_t* firm_vers;
    SBL_RET_CODE ret;

    /* Get Version ID */
    firm_vers = (uint32_t*)SBL_SLV_FIRMWARE_ID_ADDR;

    /* Send to master */
    transfer.TxBuf = (uint8_t*)firm_vers;
    transfer.TxLen = SBL_FwVersionID_ParamSize;
    transfer.RxBuf = NULL;
    transfer.RxLen = 0;
    transfer.Type = START_STOP_PACKET;
    ret = SBL_ReadWrite(&transfer);
    
    return ret;
}

/*********************************************************************//**
 * @brief     Upgrade firmware with data sent by master
 * @param[in] None.
 * @return    SBL_OK/SBL_FLASH_WRITE_ERR
 **********************************************************************/
SBL_RET_CODE SBL_SlaveUpgradeFw(void)
{
    SBL_RET_CODE ret = SBL_OK;
    IAP_STATUS_CODE iap_ret = CMD_SUCCESS;
    uint32_t flash_memory = SBL_SLV_FIRMWARE_START;
    uint32_t sec_num = 0;
    uint32_t block_num = 0;
    uint8_t* Slave_Buffer = (uint8_t*)SBL_RAM_BUFFER_ADDR;
    
    __disable_irq();

    /* Erase the last sector which includes the firmware id */
    iap_ret = EraseSector(SBL_SLV_LAST_SECTOR_NUM,SBL_SLV_LAST_SECTOR_NUM);
    if( iap_ret != CMD_SUCCESS)
    {
        return SBL_FLASH_WRITE_ERR;
    }

    /* receive firmware data */
    while(1)
    {
        /* Read a block of data */
        ret = SBL_SlaveRecvFwData(&block_num);

        /* Check result */
        if(ret == SBL_CHECKSUM_ERR)
        {
            iap_ret = (IAP_STATUS_CODE)-1;
            /* Checksum is incorrect */
            goto send_status;
        }

        /* End of receiving firmware data */
        if(ret == SBL_FW_UPGRADE_END)
        {
            ret = SBL_OK;
            break;
        }
        
        if(ret != SBL_OK)
        {
            return ret;
        }
        
        /* Write flash */
        flash_memory = SBL_SLV_FIRMWARE_START + (block_num<<10);

        sec_num = GetSecNum(flash_memory);
        iap_ret = CMD_SUCCESS;
        
        /* Erase sectos ( if any) */
        if(block_num%4 == 0) // first block of a sector
        {
            iap_ret = EraseSector(sec_num,sec_num);
        }
        /* Write firmware data */
        if( iap_ret == CMD_SUCCESS)
        {
            iap_ret = CopyRAM2Flash((uint8_t*)flash_memory,Slave_Buffer,(IAP_WRITE_SIZE)SBL_FW_DATA_BLOCK_SIZE);
        }
     send_status:
        // Send status
        if( iap_ret == CMD_SUCCESS)
        {
            SBL_SlaveSendStatus(SBL_Status_OK);
        }
        else
        {
            SBL_SlaveSendStatus(SBL_Status_ERR);
            ret = SBL_FLASH_WRITE_ERR;
        }
        
    }

    __enable_irq();

    return ret;
}

/*********************************************************************//**
 * @brief        Receive firmware data from master
 * @param[out]   pBlockNum pointer to the buffer which is used to store the block number of the received data.
 * @return       SBL_OK/SBL_INVALID_PARAM/SBL_CHECKSUM_ERR/SBL_FW_UPGRADE_END
 **********************************************************************/
SBL_RET_CODE SBL_SlaveRecvFwData(uint32_t* pBlockNum)
{
    uint8_t checksum = 0;
    uint8_t MSB, LSB;
    SBL_RET_CODE ret;
    uint8_t tmp;
    uint32_t j = 0;
    SBL_TRANSER_Type transfer;
    uint8_t* Slave_Buffer = (uint8_t*)SBL_RAM_BUFFER_ADDR;

    if(pBlockNum == NULL)
        return SBL_INVALID_PARAM;

    transfer.TxBuf = NULL;
    transfer.TxLen = 0;
    transfer.RxBuf = Slave_Buffer;
    transfer.RxLen = SBL_FwUpgrade_Fw_Idx;
    transfer.Type = START_PACKET;
    ret = SBL_ReadWrite(&transfer);
    if(ret != SBL_OK)
        return ret;
    
    /* MSB */
    MSB = Slave_Buffer[SBL_FwUpgrade_MSB_Idx];
    checksum -= MSB;

    /* LSB */
    LSB = Slave_Buffer[SBL_FwUpgrade_LSB_Idx];
    checksum -= LSB;    

    if(MSB == 0 && LSB == 0) // End of data
    {
        /* checksum */
        transfer.RxBuf = &tmp;
        transfer.RxLen = 1;
        transfer.Type = STOP_PACKET;
        ret = SBL_ReadWrite(&transfer);
        if(ret != SBL_OK)
            return ret;
        
        if(checksum != tmp)
            return SBL_CHECKSUM_ERR;

        return SBL_FW_UPGRADE_END;
    }

    /* Fw Data */
    transfer.RxBuf = Slave_Buffer;
    transfer.RxLen = SBL_FW_DATA_BLOCK_SIZE;
    transfer.Type = MID_PACKET;
    ret = SBL_ReadWrite(&transfer);
    if(ret != SBL_OK)
        return ret;

    /* Check sum receive */
    transfer.RxBuf = &tmp;
    transfer.RxLen = 1;
    transfer.Type = STOP_PACKET;
    ret = SBL_ReadWrite(&transfer);
    if(ret != SBL_OK)
       return ret;

    *pBlockNum = ((MSB<<8)|LSB)-1;

    for(j = 0; j < SBL_FW_DATA_BLOCK_SIZE;j++)
    {
        checksum -= Slave_Buffer[j];
    }

    /* checksum */
    if(checksum != tmp)
        return SBL_CHECKSUM_ERR;

    return SBL_OK;
}

/*********************************************************************//**
 * @brief        Send status to master
 * @param[in]    status 	The status which will be sent.
 * @return       SBL_OK/SBL_COMM_ERR
 **********************************************************************/
SBL_RET_CODE SBL_SlaveSendStatus(SBL_STATUS status)
{
    uint8_t arrStatus[SBL_Status_ParamSize];
    uint32_t tmp = 0;
    SBL_RET_CODE ret;

    SBL_TRANSER_Type transfer;

    /* Prepare buffer to send */
    for(tmp = 0; tmp < SBL_Status_ParamSize; tmp++)
        arrStatus[tmp] = status;

    /* Send to master */
    transfer.TxBuf = arrStatus;
    transfer.TxLen = SBL_Status_ParamSize;
    transfer.RxBuf = NULL;
    transfer.RxLen = 0;
    transfer.Type = STOP_PACKET;
    ret = SBL_ReadWrite(&transfer);
    return ret;
}

/*********************************************************************//**
 * @brief        Run the new firmware
 * @param[in]    None.
 * @return       SBL_OK
 **********************************************************************/
SBL_RET_CODE SBL_SlaveRunUserCode(void)
{
    USER_ENTRY_PFN user_entry;
    user_entry = (USER_ENTRY_PFN)*((uint32_t*)(SBL_SLV_FIRMWARE_START +4));
    (user_entry)();
    return SBL_OK;
} 
/*********************************************************************//**
 * @brief        Handle Function ID received from Master
 * @param[in]    fnID  Function ID.
 * @return       None
 **********************************************************************/
SBL_RET_CODE SBL_FnHandler(SBL_FUNC_ID fnID)
{
    switch(fnID)
    {
        /* Read Firmware Version ID */
        case SBL_ReadFirmVerionID_FnID:
            return SBL_SlaveSendFwVersionID();
        /* Upgrade Firmware */
        case SBL_FwUpgrade_FnID:
            initialize_data();
            SBL_SlaveUpgradeFw();
            NVIC_SystemReset();
        default:
            return SBL_ERR;
    }
}
/*********************************************************************//**
 * @brief        SBL Entry. It does:
 *		         - Wait for function ID from master.
 *		         - If master aks to read firmware Version ID, send it to master.
 *		         - If master asks to upgrade firmware, receive firmware from master,
 *		         and then run the new firmware.
 * @param[in]    None.
 * @return       None.
 **********************************************************************/
void SBL_SlaveEntry(void)
{
    SBL_FUNC_ID fnID;
    SBL_RET_CODE ret;

    ret = SBL_SlaveReadFuncID(&fnID);
    if(ret == SBL_OK)
    {
        SBL_FnHandler(fnID);
    }
}

/*********************************************************************//**
 * @brief        Handle SBL API Call
 * @param[in]    API API Id. It can be:
 *                 SBL_SLAVE_INIT_FUNC
 *                 SBL_SLAVE_DEINIT_FUNC
 *                 SBL_CMD_HANDLER_FUNC
 *               pData   Input parameters for the API called.  
 *                      In case the SBL_CMD_HANDLER_FUNC is called, It should be 
 *                          the received command over bus. 
 * @return       None.
 **********************************************************************/
void SBL_APICall_Handler(uint32_t API, uint8_t* pData)
{
    switch(API)
    {
        case SBL_SLAVE_INIT_FUNC:
            SBL_SlaveInit();
            break;
        case SBL_SLAVE_DEINIT_FUNC:
            break;
        case SBL_CMD_HANDLER_FUNC:
            SBL_FnHandler(*pData);
            break;
        default:
            break;
    }
}
/*********************************************************************//**
 * @brief        Call SBL API
 * @param[in]    API API Id. It can be:
 *                 SBL_SLAVE_INIT_FUNC
 *                 SBL_SLAVE_DEINIT_FUNC
 *                 SBL_CMD_HANDLER_FUNC
 *               pData   Input parameters for the API called.  
 *                      In case the SBL_CMD_HANDLER_FUNC is called, It should be 
 *                          the received command over bus. 
 * @return       None.
 **********************************************************************/
void SBL_APICall(uint32_t API, uint8_t* pData)
{
    SBL_APICall_Handler(API, pData);
}

