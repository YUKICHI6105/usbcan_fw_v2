/*
 *  MoterCtrl.hpp
 *
 *  Created on: 2023/09/07
 *      Author: ykc
 */

#include <cstring>
#include <main.h>
#include "stm32f3xx_hal.h"
#include "led.h"

extern"C"{
extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef TxHeader1;
extern CAN_TxHeaderTypeDef TxHeader2;
extern uint32_t TxMailbox;
}

enum class Mode{
	dis,
	vel,
	pos,
	hom,
};

struct MotorParam{
	Mode mode[8] = {Mode::dis,Mode::dis,Mode::dis,Mode::dis,Mode::dis,Mode::dis,Mode::dis,Mode::dis};
	float gool[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};//PID処理後の操作量
	float target[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};//目標値
	float e_pre[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	float mechanical_angle[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};//機械角
	float velocity[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};//rpm->rad/s
	float current[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	uint8_t temp[8] = {0,0,0,0,0,0,0,0};
	uint8_t limitTemp[8] = {50,50,50,50,50,50,50,50};
	float Kp[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	float Ki[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	float Kd[8] = {0.8,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
};

class MotorCtrl{
private:
	MotorParam param;
	float e = 0.0;
	//uint16_t vel = 0;
public:
	void setAng(uint16_t data, uint32_t receiveID);
	void setVel(uint16_t data, uint32_t receiveID);
	void setCur(uint16_t data, uint32_t receiveID);
	bool update(uint32_t ReceiveID,uint8_t receiveData[8]);
	void reset(uint8_t i);
	void setMode(uint8_t usb_msg[]);
	void setTemp(uint8_t usb_msg[]);
	void setTarget(uint8_t usb_msg[]);
	void setKp(uint8_t usb_msg[]);
	void setKi(uint8_t usb_msg[]);
	void setKd(uint8_t usb_msg[]);
	uint8_t value1[8];
	uint8_t value2[8];
	void transmit1();
	void transmit2();
	void ems();
	uint8_t diag = 1;
};

