/***********************************************************************
 * $Id:: CAN_Node_Def.h 1604 2012-04-24 11:34:47Z nxp31103     $
 *
 * Project: CANopen Application Example for LPC11Cxx (master)
 *
 * Description:
 *   CANopen definition header file
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
 **********************************************************************/

#ifndef _CAN_NODE_DEF_H
#define _CAN_NODE_DEF_H

#include <stdint.h>
#include "canopen_driver.h"

#define CAN_MASTER_NODE			0x7D		/* 125 */
#define CAN_SLAVE1_NODE			0x01		/* 1 */
#define CAN_SLAVE2_NODE			0x02		/* 2 */

#define CAN_NODE_ID				CAN_MASTER_NODE
#define CANOPEN_TIMEOUT_VAL		100			/* in ms */

/* Application variables used in variable OD */
extern uint8_t  Error_Register;						/* CANopen error register */
extern uint32_t CANopen_Heartbeat_Producer_Value;	/* heartbeat producer value */

extern WatchNode_t WatchList[];						/* Watch 2 nodes for their NMT state / bootup message and heartbeat */
extern uint8_t WatchListLength;						/* required so that number of nodes in watchlist is known */
extern CAN_ODCONSTENTRY myConstOD[];				/* OD for constant values, e.g. vendor ID */
extern uint32_t NumberOfmyConstODEntries;			/* required so that the number of entries in the variable OD is known */
extern CAN_ODENTRY myOD[];							/* OD for variable values, e.g. for controlling LEDs */
extern uint32_t NumberOfmyODEntries;				/* required so that the number of entries in the variable OD is known */

#endif /* _CAN_NODE_DEF_H */
