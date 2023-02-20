/*
 * user_if.c
 *
 *  Created on: Dec 26, 2022
 *      Author: taman
 */
 
#include "main.h"
#include "can_usb.h"
#include "usbd_cdc_if.h"

#include "led.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    uint8_t Data[15];
    CAN_RxHeaderTypeDef RxHeader;
    //the Data is used for USB buffer. can_process set header infomation to Data[0~5].
    //It is a terrible code. Sorry for hard work to read the code.
    led_on(green);
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data+6) == HAL_OK)
    {

        can_process(&RxHeader,Data);
    }
}

uint8_t send_buf[4]={0};

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan){
	led_on(can);
	if(hcan->ErrorCode == HAL_CAN_ERROR_NONE )led_on(yellow);
	if(hcan->ErrorCode == HAL_CAN_ERROR_BD)led_on(red);
	uint32_t errorcode = hcan->ErrorCode;
	uint32_t MSR = hcan->Instance->MSR;
//	memcpy(send_buf,&errorcode,4);
//	CDC_Transmit_FS(send_buf, 4);
//	memcpy(send_buf,&MSR,4);
//	CDC_Transmit_FS(send_buf, 4);

}

