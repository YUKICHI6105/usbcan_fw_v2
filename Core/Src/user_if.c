/*
 * user_if.c
 *
 *  Created on: Dec 26, 2022
 *      Author: taman
 */
 

#include "cobs.h"
#include "main.h"   


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t Data[15];
    CAN_RxHeaderTypeDef RxHeader;
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data+7) == HAL_OK)
    {
        can_process(&RxHeader,Data);
    }
}

