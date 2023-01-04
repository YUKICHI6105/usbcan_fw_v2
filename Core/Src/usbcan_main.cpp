#include <CRSLib/Can/RM0365/include/can_manager.hpp>
#include <CRSLib/Can/RM0365/include/filter_manager.hpp>
#include <CRSLib/Can/RM0365/include/letterbox.hpp>
#include <CRSLib/Can/RM0365/include/pillarbox.hpp>

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

using namespace CRSLib::IntegerTypes;
using namespace CRSLib::Can;
using namespace CRSLib::Can::RM0365;

extern CAN_HandleTypeDef hcan;
extern USBD_HandleTypeDef hUsbDeviceFS;


void main_cpp()
{
    CanManager can_manager{&hcan};

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

    HAL_CAN_Start(&hcan);
    RxFrame rx_frame{};

    while(true)
    {

        if(!can_manager.letterbox0.empty())
        {
            can_manager.letterbox0.receive(rx_frame);

            TxFrame tx_frame{{rx_frame.header.dlc}, rx_frame.data};

            if(can_manager.pillarbox.not_full()) can_manager.pillarbox.post(0x100, tx_frame);
        }
        switch(hUsbDeviceFS.dev_state){
        case USBD_STATE_DEFAULT:
            HAL_GPIO_WritePin(GPIOB,LED_RED_Pin,GPIO_PIN_SET);
            break;
        case USBD_STATE_ADDRESSED :
            HAL_GPIO_WritePin(GPIOB,LED_YELLOW_Pin,GPIO_PIN_SET);
            break;
        case USBD_STATE_SUSPENDED  :
            HAL_GPIO_WritePin(GPIOB,LED_RED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,LED_YELLOW_Pin,GPIO_PIN_SET);
            break;
        case USBD_STATE_CONFIGURED :
            HAL_GPIO_WritePin(GPIOB,LED_CAN_Pin,GPIO_PIN_SET);
            break;
        }
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOB,LED_RED_Pin|LED_YELLOW_Pin|LED_CAN_Pin,GPIO_PIN_RESET);

    //	 CDC_Transmit_FS(buffer,6);
    }
}
