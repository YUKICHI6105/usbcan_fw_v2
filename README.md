# usbcan_fw_v2
COBSを利用したUSBCAN用のコードです。以下のプログラムで利用できます。 

[can_plugins2](https://github.com/IndigoCarmine/can_plugins2) : ROS2　Package  
[can_anylzer](https://github.com/IndigoCarmine/can_analyzer) : Flutter Application for Android and iOS  
[serial_tester](https://github.com/IndigoCarmine/serial_tester) : Python Code for test  
[can_plugins_porting](https://github.com/IndigoCarmine/can_plugins2_porting) : ROS Package (porting from ROS2 Package)  

#Commandの種類
0x00:normal can frame
0x01:establishment of communication
0x02:EMS Change Nortice

# LEDの点灯パターンについて
USBCANには２種類の状態があります。
1. USBの接続確立途中
2. 接続完了後

## USBの接続確立途中
USBCANはUSBの接続確立途中はLEDのStatusに応じて点灯します。
USBD_STATE_DEFAULT : 赤色LEDが点灯
USBD_STATE_ADDRESSED : 黄色LEDが点灯
USBD_STATE_SUSPENDED : 赤色LEDと黄色LEDが点灯
USBD_STATE_CONFIGURED : 緑色LEDが点灯 -> 状態2へ即座に移行

## 接続後
USBCANは接続後は通信状態とCANの状態に応じてLEDの点灯パターンが変わります。
USBへの送信完了：緑色LEDが点灯
CANへの送信完了：CANLEDが点灯
HAL_CAN_ERROR_BD（Bit Dominant Error）：赤色LEDが点灯
HAL_CAN_ERROR_ACK（Acknowledge Error）：黄色LEDが点灯
