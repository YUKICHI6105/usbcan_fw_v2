
#include "can_usb.h"
#include "cobs.h"
#include "usbd_cdc_if.h"
#include "stm32f3xx_hal.h"
#include "main.h"
#include "buffer.hpp"

#include "led.h"

extern CAN_HandleTypeDef hcan;
Buffer buffer;

void usb_to_can(uint8_t usb_msg[], const uint8_t len);

// it process  all usb messages
void usb_process(uint8_t usb_msg[], const uint8_t len)
{
    // data structure
    /*
    uint8_t command & frame_type:
    ... : some data
    */

    // if you want to add new command, you can add it here
    // attention: it is called in interrupt, so it should be short
    switch (usb_msg[0] >> 4)
    {
    case 0x00: // normal can frame
    {
        usb_to_can(usb_msg, len);
    }
    break;
    case 0x01: // establishment of communication
    {
        // encoded data
        static uint8_t HelloSLCAN_encoded[] = {0x0c, 0x01 << 4, 'H', 'e', 'l', 'l', 'o', 'S', 'L', 'C', 'A', 'N', 0x00};
        CDC_Transmit_FS(HelloSLCAN_encoded, 11 + 2);
    }
    break;
    default:
        break;
    }
}

// it process all can messages
// the Data is used for USB buffer. can_process set header information to Data[0~5].
// It is a terrible code. Sorry for hard work to read the code.
void can_process(const CAN_RxHeaderTypeDef *RxHeader, uint8_t Data[])
{
    // data structure
    /*
    uint8_t command & frame_type: (command: if it is normal can frame, it is 0x00.)<<4 | is_rtr << 2 | is_extended << 1 | is_error
    uint8_t id[4] : can id
    uint8_t dlc : data length
    uint8_t data[8] : data (it is pre-writtten.)
    */

    if (RxHeader->IDE == CAN_ID_STD)
    {
        // standard id
        Data[1] = (RxHeader->StdId >> 24) & 0xFF;
        Data[2] = (RxHeader->StdId >> 16) & 0xFF;
        Data[3] = (RxHeader->StdId >> 8) & 0xFF;
        Data[4] = (RxHeader->StdId >> 0) & 0xFF;

        // is_extended not set
        Data[0] = 0x00;
    }
    else
    {
        // extended id
        Data[1] = (RxHeader->ExtId >> 24) & 0xFF;
        Data[2] = (RxHeader->ExtId >> 16) & 0xFF;
        Data[3] = (RxHeader->ExtId >> 8) & 0xFF;
        Data[4] = (RxHeader->ExtId >> 0) & 0xFF;

        // is_extended set
        Data[0] = 0x1 << 1;
    }

    // is_rtr
    if (RxHeader->RTR == CAN_RTR_REMOTE)
    {
        Data[0] |= 0x1 << 2;
    }

    // is_error
    //?

    // dlc
    Data[5] = RxHeader->DLC;

    // encode data
    uint8_t encoded_data[14 + 2];

    cobs_encode(Data, encoded_data, 14);

    if (CDC_Transmit_FS(encoded_data, 14 + 2) == USBD_OK)
    {
        led_on(green);
        // transmit success
    }
    else
    {
        // transmit fail
    }
}

CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;
// it process usb messages to normal can messages
void usb_to_can(uint8_t usb_msg[], const uint8_t len)
{
    // data structure
    /*
    uint8_t command & frame_type: (command: if it is normal can frame, it is 0x00.)<<4 | is_rtr << 2 | is_extended << 1 | is_error
    uint8_t id[4] : can id
    uint8_t dlc : data length
    uint8_t data[8] : data
    */
    if (usb_msg[0] & 0x02)
    {
        // extended id
        TxHeader.IDE = CAN_ID_EXT;
        TxHeader.ExtId = (usb_msg[1] << 24) | (usb_msg[2] << 16) | (usb_msg[3] << 8) | (usb_msg[4] << 0);
    }
    else
    {
        // standard id
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = (usb_msg[1] << 24) | (usb_msg[2] << 16) | (usb_msg[3] << 8) | (usb_msg[4] << 0);
    }

    // is_rtr
    if (usb_msg[0] & 0x04)
    {
        TxHeader.RTR = CAN_RTR_REMOTE;
    }
    else
    {
        TxHeader.RTR = CAN_RTR_DATA;
    }

    // is_error
    //?

    TxHeader.TransmitGlobalTime = DISABLE;
    // dlc
    TxHeader.DLC = usb_msg[5];

    if (0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
    {
        led_on(can);
        HAL_CAN_AddTxMessage(&hcan, &TxHeader, usb_msg + 6, &TxMailbox);
    }else{
    	buffer.setBuffer(usb_msg);
    }
}

