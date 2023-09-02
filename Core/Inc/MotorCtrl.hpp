/*
 *  MoterCtrl.hpp
 *
 *  Created on: 2023/08/07
 *      Author: ykc
 */

#include <cstring>
#include <main.h>
#include "stm32f3xx_hal.h"


enum class Mode{
	dis,
	vel,
	pos,
	hom,
};

struct MotorParam{
	Mode mode[8];
	float gool[8];//PID処理後の操作量
	float target[8];//目標値
	float e_pre[8];
	float mechanical_angle[8];//機械角
	float velocity[8];//rpm->rad/s
	float current[8];
	uint8_t temp[8];
	uint8_t limitTemp[8];
	float Kp[8];
	float Ki[8];
	float Kd[8];
};

class MotorCtrl{
private:
	MotorParam param;
	uint8_t diag = 0;
public:
	bool update(uint32_t& ReceiveID,uint8_t receiveData[8]);
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
	void reset();
};



inline bool MotorCtrl::update(uint32_t& ReceiveID,uint8_t receiveData[8]){
	if(ReceiveID<0x201||ReceiveID>0x208){return false;}
	MotorCtrl::param.mechanical_angle[ReceiveID-0x201]
		= static_cast<float>((static_cast<uint16_t>(receiveData[0]) << 8) | receiveData[1])*360/8191;
	MotorCtrl::param.velocity[ReceiveID-0x201]
		= static_cast<float>((static_cast<uint16_t>(receiveData[2]) << 8) | receiveData[3])*3.1415/60.0;
	MotorCtrl::param.current[ReceiveID-0x201]
		= static_cast<float>((static_cast<uint16_t>(receiveData[4]) << 8) | receiveData[5]);
	MotorCtrl::param.temp[ReceiveID-0x201] = receiveData[6];
	if(param.mode[ReceiveID-0x201]==Mode::vel){
		float e = param.target - param.velocity;
		param.gool[ReceiveID-0x201] = param.gool[ReceiveID-0x201]+param.Kp[ReceiveID-0x201]*e+param.Ki[ReceiveID-0x201]*(e+param.e_pre[ReceiveID-0x201])*0.001/2+param.Kd[ReceiveID-0x201]*(e-param.e_pre[ReceiveID-0x201])/0.001;
		param.e_pre[ReceiveID-0x201]=e;
	}
	return true;
}

inline void MotorCtrl::setMode(uint8_t usb_msg[]){
	for(int i =0;i<8;i++){
		if(usb_msg[i]==0){
			param.mode[i] = Mode::dis;
		}else if(usb_msg[i]==1){
			param.mode[i] = Mode::vel;
		}else if(usb_msg[i]==2){
			param.mode[i] = Mode::pos;
		}else if(usb_msg[i]==3){
			param.mode[i] = Mode::hom;
		}
	}
	if(usb_msg[8] == 1){
		diag=1;
	}else if(usb_msg[8] == 1){
		diag=0;
	}
}

inline void MotorCtrl::setTemp(uint8_t usb_msg[]){
	for(int i = 0;i<8;i++){
		param.limitTemp[i]=usb_msg[i];
	}
}

inline void MotorCtrl::setTarget(uint8_t usb_msg[]){
	for(int i =0;i<8;i++){
		uint32_t buf = (usb_msg[4*i] << 24) | (usb_msg[4*i+1] << 16) | (usb_msg[4*i+2] << 8) | (usb_msg[4*i+3] << 0);
		std::memcpy(&param.target[i],&buf,1);
	}
}

inline void MotorCtrl::setKp(uint8_t usb_msg[]){
	for(int i =0;i<8;i++){
		uint32_t buf = (usb_msg[4*i] << 24) | (usb_msg[4*i+1] << 16) | (usb_msg[4*i+2] << 8) | (usb_msg[4*i+3] << 0);
		std::memcpy(&param.Kp[i],&buf,1);
	}
}

inline void MotorCtrl::setKi(uint8_t usb_msg[]){
	for(int i =0;i<8;i++){
		uint32_t buf = (usb_msg[4*i] << 24) | (usb_msg[4*i+1] << 16) | (usb_msg[4*i+2] << 8) | (usb_msg[4*i+3] << 0);
		std::memcpy(&param.Ki[i],&buf,1);
	}
}

inline void MotorCtrl::setKd(uint8_t usb_msg[]){
	for(int i =0;i<8;i++){
		uint32_t buf = (usb_msg[4*i] << 24) | (usb_msg[4*i+1] << 16) | (usb_msg[4*i+2] << 8) | (usb_msg[4*i+3] << 0);
		std::memcpy(&param.Kp[i],&buf,1);
	}
}


