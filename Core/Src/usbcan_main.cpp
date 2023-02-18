

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

#include <CRSLib/Can/RM0365/include/can_manager.hpp>
#include <CRSLib/Can/RM0365/include/filter_manager.hpp>
#include <CRSLib/Can/RM0365/include/letterbox.hpp>
#include <CRSLib/Can/RM0365/include/pillarbox.hpp>

#include "led.h"

using namespace CRSLib::IntegerTypes;
using namespace CRSLib::Can;
using namespace CRSLib::Can::RM0365;

extern CAN_HandleTypeDef hcan;
extern USBD_HandleTypeDef hUsbDeviceFS;


void main_cpp()
{
    CanManager can_manager{&hcan};

    FilterManager::dynamic_initialize();

    Filter<FilterWidth::bit32, FilterMode::mask> filter =
    {
        .masked32 =
        {
            .id = {0x0, 0x0, false, false},
            .mask = {0x0, 0x0, false, false}
        }
    };

    FilterManager::ConfigFilterArg<FilterWidth::bit32, FilterMode::mask> filter_arg
    {
        .filter = filter,
        .fifo = FifoIndex::fifo0,
        .filter_match_index = 0, // なんでもいい。
        .activate = true
    };

    FilterManager::config_filter_bank(15, filter_arg);
    [[maybe_unused]] const u32 fmi = filter_arg.filter_match_index;

    HAL_CAN_Start(&hcan);
//    RxFrame rx_frame{};
    uint8_t state =23;
    uint8_t debug_state = 0;
    while(true)
    {
    	if(debug_state==0&&hUsbDeviceFS.dev_state !=state){
    		state = hUsbDeviceFS.dev_state;
    		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,
    				LED_GREEN_Pin|
    				LED_YELLOW_Pin|
    				LED_RED_Pin
    				,GPIO_PIN_RESET);
    		switch(hUsbDeviceFS.dev_state){
    		case USBD_STATE_DEFAULT:
    			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
    			break;
    		case USBD_STATE_ADDRESSED :
    			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_SET);
    			break;
    		case USBD_STATE_SUSPENDED  :
    			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_SET);
    			break;
    		case USBD_STATE_CONFIGURED :
    			led_on(green);
    			debug_state =1;
    			break;
    		}
    	}

        led_process();


//        if(!can_manager.letterbox0.empty())
//        {
//            can_manager.letterbox0.receive(rx_frame);
//
//            TxFrame tx_frame{{rx_frame.header.dlc}, rx_frame.data};
//
//            if(can_manager.pillarbox.not_full()) can_manager.pillarbox.post(0x100, tx_frame);
//        }


    }
}
