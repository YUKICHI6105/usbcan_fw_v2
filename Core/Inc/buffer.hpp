#include <main.h>

struct Data{
	uint8_t data[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	bool sended = true;
};

class Buffer{
private:
	Data buffer[4];
public:
	void setBuffer(uint8_t usb_msg[]);
	void resend();
};

inline void Buffer::setBuffer(uint8_t usb_msg[]){
	for(uint8_t j = 0;j<4;j++){
		if(buffer[j].sended == true){
			for(uint8_t i = 0;i<8;i++){
				buffer[j].data[i] = usb_msg[i];
			}
			buffer[j].sended = false;
		}
	}
}

