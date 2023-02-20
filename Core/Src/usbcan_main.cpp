

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"


//#include <CRSLib/Can/RM0365/include/can_manager.hpp>
//#include <CRSLib/Can/RM0365/include/filter_manager.hpp>
//#include <CRSLib/Can/RM0365/include/letterbox.hpp>
//#include <CRSLib/Can/RM0365/include/pillarbox.hpp>

#include "led.h"
#include "can_usb.h"

//using namespace CRSLib::IntegerTypes;
//using namespace CRSLib::Can;
//using namespace CRSLib::Can::RM0365;

extern CAN_HandleTypeDef hcan;
extern USBD_HandleTypeDef hUsbDeviceFS;


void main_cpp()
{
//    CanManager can_manager{&hcan};
//
//    FilterManager::dynamic_initialize();
//
//    Filter<FilterWidth::bit32, FilterMode::mask> filter =
//    {
//        .masked32 =
//        {
//            .id = {0x0, 0x0, false, false},
//            .mask = {0x0, 0x0, false, false}
//        }
//    };
//
//    FilterManager::ConfigFilterArg<FilterWidth::bit32, FilterMode::mask> filter_arg
//    {
//        .filter = filter,
//        .fifo = FifoIndex::fifo0,
//        .filter_match_index = 0, // なんでもいい。
//        .activate = true
//    };
//
//    FilterManager::config_filter_bank(15, filter_arg);
//    [[maybe_unused]] const u32 fmi = filter_arg.filter_match_index;
//
    HAL_CAN_Start(&hcan);
//    RxFrame rx_frame{};
    uint8_t debug_state = 0;
    while(true)
    {
    	if(READ_BIT(CAN->MSR,CAN_MSR_INAK)==1){
    	    //can start
    	    CLEAR_BIT(CAN->MCR, CAN_MCR_INRQ);
    	    led_on(green);
    	}
    	if(debug_state==0){
    		switch(hUsbDeviceFS.dev_state){
    		case USBD_STATE_DEFAULT:
    			led_on(red);
    			break;
    		case USBD_STATE_ADDRESSED :
    			led_on(yellow);
    			break;
    		case USBD_STATE_SUSPENDED  :
    			led_on(yellow);
    			break;
    		case USBD_STATE_CONFIGURED :
    			led_on(green);
    			debug_state =1;
    			break;
    		}
    	}else if(debug_state==1){
    		switch(hcan.ErrorCode){
    		case HAL_CAN_ERROR_BD: //bit dominant
    			led_on(green);
    			break;
    		case HAL_CAN_ERROR_ACK: //a
    			led_on(yellow);
    			break;
    		default:
    			break;
    		}
//    		static uint8_t send_buf[4]={0};
//    		uint32_t ESR = hcan.Instance->MCR;
//    		memcpy(send_buf,&ESR,4);
//    		CDC_Transmit_FS(send_buf, 4);
    	}
//    	uint8_t data[] = {0x00,0x00,0x00,0x07,0xff,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
//    	uint8_t len = 14;
//    	usb_process(data, len);

        led_process();
    }
}
