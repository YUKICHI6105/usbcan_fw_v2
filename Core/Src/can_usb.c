
#include "can_usb.h"
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
    default:
        break;
    }

}

//it process all can messages
void can_process(uint8_t *can_msg, uint8_t len){

}




//it process normal can messages to usb messages
void can_to_usb(uint8_t *can_msg, uint8_t len){

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
