/*
*ExoNaut_LineFollower.h
*
*Author:	Andrew Gafford
*Email:		agafford@spacetrek.com
*Date:		May 20th, 2024
*
*This library is for controlling the Space Trek ExoNaut Robot's Line Follower sensor.
*
*/

#include "ExoNaut_LineFollower.h"



//Wire write byte
bool lineFollower::wireWriteByte(uint8_t addr, uint8_t val){
	Wire.beginTransmission(addr);
	Wire.write(val);
	if(Wire.endTransmission() != 0){
		return false;
	}
	return true;
}

//Wire write multiple bytes
bool lineFollower::wireWriteDataArray(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len){
	unsigned int i;
	Wire.beginTransmission(addr);
	Wire.write(reg);
	for(i = 0; i < len; i++){
		Wire.write(val[i]);
	}
	if(Wire.endTransmission() != 0){
		return false;
	}
	return true;
}

//Read specified length of bytes
int lineFollower::wireReadDataArray(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len){
	unsigned char i = 0;  
	if(!wireWriteByte(addr, reg)){				/* Indicate which register we want to read from */
		return -1;
	}
	Wire.requestFrom(addr, len, false);
	while(Wire.available()){					/* Read block data */
		if(i >= len){
			return -1;
		}
		val[i] = Wire.read();
		i++;
    }    
    return i;
}


bool lineFollower::readLineFollower(uint8_t &val){
	Wire.beginTransmission(LINE_FOLLOWER_I2C_ADDR);
	Wire.write(0x01);
	if(Wire.endTransmission() != 0){
		//should set val to all sensors not tripped....
		return false;
	}
	Wire.requestFrom(LINE_FOLLOWER_I2C_ADDR, 1);
	while(Wire.available()){
		val = Wire.read();
	}
	
	this->CH1 = val & 0b00000001;
	this->CH2 = (val & 0b00000010) >> 1;
	this->CH3 = (val & 0b00000100) >> 2;
	this->CH4 = (val & 0b00001000) >> 3;
	
	return true;
}