
#include "can_usb.h"

#include "usbd_cdc_if.h"
#include "stm32f3xx_hal.h"

extern CAN_HandleTypeDef hcan;

void usb_to_can(uint8_t *usb_msg, uint8_t len);
void can_to_usb(uint8_t *can_msg, uint8_t len);


//it process all usb messages
void usb_process(uint8_t *usb_msg, uint8_t len){
    // data structure
    /*
    uint8_t command : if it is normal can frame, it is 0x00.
    ... : some data
    */

    //if you want to add new command, you can add it here
    //attention: it is called in interrupt, so it should be short
    switch (usb_msg[0])
    {
    case 0x00://normal can frame
        {
            usb_to_can(usb_msg+1, len-1);
        }
        break;
    case 0x01://establishment of communication
        {
            const uint8_t HelloUSBCAN_encode[] = {0x0b,'H','e','l','l','o','U','S','B','C','A','N',0x00};
            CDC_Transmit_FS(HelloUSBCAN_encode,11+2);
        }
    default:
        break;
    }

}


//it process all can messages
void can_process(CAN_RxHeaderTypeDef *RxHeader,uint8_t Data[]){
    // data structure
    /*
    uint8_t command : if it is normal can frame, it is 0x00.
    uint8_t id[4] : can id
    uint8_t frame_type :  is_rtr << 2 | is_extended << 1 | is_error
    uint8_t dlc : data length
    uint8_t data[8] : data
    */

    // Data[0] = 0x00;

    if(RxHeader->IDE == CAN_ID_STD){
        //standard id
        Data[1] = (RxHeader->StdId >> 24) & 0xFF;
        Data[2] = (RxHeader->StdId >> 16) & 0xFF;
        Data[3] = (RxHeader->StdId >> 8) & 0xFF;
        Data[4] = (RxHeader->StdId >> 0) & 0xFF;
        
        //is_extended not set
        // Data[5] = 0x00;
    }else{
        //extended id
        Data[1] = (RxHeader->ExtId >> 24) & 0xFF;
        Data[2] = (RxHeader->ExtId >> 16) & 0xFF;
        Data[3] = (RxHeader->ExtId >> 8) & 0xFF;
        Data[4] = (RxHeader->ExtId >> 0) & 0xFF;

        //is_extended set
        Data[5] = 0x02; // 0x02 = 0b00000010
    }

    //is_rtr
    if(RxHeader->RTR == CAN_RTR_REMOTE){
        Data[5] |= 0x04; // 0x04 = 0b00000100
    }

    //is_error
    //? 

    //dlc
    Data[6] = RxHeader->DLC;


    //encode data
    uint8_t encoded_data[15+2];

    cobs_encode(Data, encoded_data,15);


    if(CDC_Transmit_FS(encoded_data,15+2)== USBD_OK){
        //transmit success
        

    }else{
        //transmit fail
    }
}






CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;
//it process usb messages to normal can messages
void usb_to_can(uint8_t *usb_msg, uint8_t len){
    // data structure
    /*
    (uint8_t command : it is 0x00. deleted)
    uint8_t id[4] : can id
    uint8_t frame_type :  is_rtr << 2 | is_extended << 1 | is_error
    uint8_t dlc : data length
    uint8_t data[8] : data
    */
   if(usb_msg[4] & 0x02){
       //extended id
       TxHeader.IDE = CAN_ID_EXT;
       TxHeader.ExtId = (usb_msg[0] << 24) | (usb_msg[1] << 16) | (usb_msg[2] << 8) | (usb_msg[3] << 0);
    }else{
        //standard id
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = (usb_msg[0] << 24) | (usb_msg[1] << 16) | (usb_msg[2] << 8) | (usb_msg[3] << 0);
    }

    //is_rtr
    if(usb_msg[4] & 0x04){
        TxHeader.RTR = CAN_RTR_REMOTE;
    }else{
        TxHeader.RTR = CAN_RTR_DATA;
    }

    //is_error
    //?

    //dlc
    TxHeader.DLC = usb_msg[5];
    
    if(0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan)){
    	HAL_CAN_AddTxMessage(&hcan, &TxHeader, usb_msg+6, &TxMailbox);
    }


}
