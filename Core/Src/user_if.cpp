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
#include "MotorCtrl.hpp"

extern"C"{
extern MotorCtrl motor;
extern CAN_TxHeaderTypeDef TxHeader1;
extern CAN_TxHeaderTypeDef TxHeader2;
extern uint32_t TxMailbox;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim3;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // data structure
    /*
    uint8_t command & frame_type: (command: if it is normal can frame, it is 0x00.)<<4 | is_rtr << 2 | is_extended << 1 | is_error
    uint8_t id[4] : can id
    uint8_t dlc : data length
    uint8_t data[8] : data (it is pre-writtten.)
    */
    uint8_t Data[14];
    CAN_RxHeaderTypeDef RxHeader;
    // the Data is used for USB buffer. can_process set header infomation to Data[0~5].
    // It is a terrible code. Sorry for hard work to read the code.
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data + 6) == HAL_OK)
    {
    	if(motor.update(RxHeader.StdId,Data + 6)){
    		if(motor.diag == 1){can_process(&RxHeader, Data);}
    	}else{
    		can_process(&RxHeader, Data);
    	}
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == EMS_Pin){
		led_on(red);
		if(HAL_GPIO_ReadPin(EMS_GPIO_Port, EMS_Pin)){
	        static uint8_t HelloSLCAN_encoded[] = {0x03, 0x02 << 4,'H', 0x00};
	        CDC_Transmit_FS(HelloSLCAN_encoded, 2 + 2);
		}else{
	        static uint8_t HelloSLCAN_encoded[] = {0x03, 0x02 << 4,'L', 0x00};
	        CDC_Transmit_FS(HelloSLCAN_encoded, 2 + 2);
		}
	}
}

void MotorCtrl::transmit1(){
	for(int i=0;i<4;i++){
			value1[2*i] = static_cast<uint8_t>(static_cast<uint16_t>(param.gool[i]) >> 8);
			value1[2*i+1] = static_cast<uint8_t>(static_cast<uint16_t>(param.gool[i]) & 0xFF);
		}
	if (0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
	    {
	        led_on(can);
	        HAL_CAN_AddTxMessage(&hcan, &TxHeader1, value1, &TxMailbox);
	    }
}

void MotorCtrl::transmit2(){
	for(int i=4;i<8;i++){
			value1[2*i] = static_cast<uint8_t>(static_cast<uint16_t>(param.gool[i]) >> 8);
			value1[2*i+1] = static_cast<uint8_t>(static_cast<uint16_t>(param.gool[i]) & 0xFF);
		}
	if (0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
	    {
	        led_on(can);
	        HAL_CAN_AddTxMessage(&hcan, &TxHeader1, value1, &TxMailbox);
	    }
}

void MotorCtrl::ems(){
	uint8_t value3[8];
	led_on(can);
	HAL_CAN_AddTxMessage(&hcan, &TxHeader1, value3, &TxMailbox);
	HAL_CAN_AddTxMessage(&hcan, &TxHeader2, value3, &TxMailbox);
}

void MotorCtrl::reset(){
	for(int i = 0;i<8;i++){
		value1[i]=0;
		value2[i]=0;
		param.target[i]=0;
		param.mode[i]=Mode::dis;
		param.gool[i]=0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim == &htim3){
		if(HAL_GPIO_ReadPin(EMS_GPIO_Port, EMS_Pin)){
			motor.transmit1();
			motor.transmit2();
		}else{
			motor.ems();
			motor.reset();
		}
	}
}
}
