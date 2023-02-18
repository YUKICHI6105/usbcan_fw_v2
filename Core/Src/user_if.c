/*
 * user_if.c
 *
 *  Created on: Dec 26, 2022
 *      Author: taman
 */
 
#include "main.h"
#include "can_usb.h"

#include "led.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	led_on(can);
    uint8_t Data[15];
    CAN_RxHeaderTypeDef RxHeader;
    //the Data is used for USB buffer. can_process set header infomation to Data[0~5].
    //It is a terrible code. Sorry for hard work to read the code.
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data+6) == HAL_OK)
    {

        can_process(&RxHeader,Data);
    }
}

