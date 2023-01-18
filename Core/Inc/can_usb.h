#include <main.h>





//it process all usb messages
void usb_process(uint8_t *usb_msg, uint8_t len);
//it process all can messages
void can_process(CAN_RxHeaderTypeDef *RxHeader,uint8_t Data[]);
