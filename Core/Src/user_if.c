/*
 * user_if.c
 *
 *  Created on: Dec 26, 2022
 *      Author: taman
 */

#include "usbd_cdc_if.h"
#include "cobs.h"


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t Data[15];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data+7) == HAL_OK)
    {
        // data structure
        /*
        uint8_t command : if it is normal can frame, it is 0x00.
        uint8_t id[4] : can id
        uint8_t frame_type :  is_rtr << 2 | is_extended << 1 | is_error
        uint8_t dlc : data length
        uint8_t data[8] : data
        */

        // Data[0] = 0x00;

        if(RxHeader.IDE == CAN_ID_STD){
            //standard id
            Data[1] = (RxHeader.StdId >> 24) & 0xFF;
            Data[2] = (RxHeader.StdId >> 16) & 0xFF;
            Data[3] = (RxHeader.StdId >> 8) & 0xFF;
            Data[4] = (RxHeader.StdId >> 0) & 0xFF;
            
            //is_extended not set
            // Data[5] = 0x00;
        }else{
            //extended id
            Data[1] = (RxHeader.ExtId >> 24) & 0xFF;
            Data[2] = (RxHeader.ExtId >> 16) & 0xFF;
            Data[3] = (RxHeader.ExtId >> 8) & 0xFF;
            Data[4] = (RxHeader.ExtId >> 0) & 0xFF;

            //is_extended set
            Data[5] = 0x02; // 0x02 = 0b00000010
        }

        //is_rtr
        if(RxHeader.RTR == CAN_RTR_REMOTE){
            Data[5] |= 0x04; // 0x04 = 0b00000100
        }

        //is_error
        //? 

        //dlc
        Data[6] = RxHeader.DLC;


        //encode data
        uint8_t encoded_data[15+2];

        cobs_encode(Data, encoded_data,15);


        if(CDC_Transmit_FS(encoded_data,15+2)== USBD_OK){
            //transmit success
            

        }else{
            //transmit fail
        }
    }
}

