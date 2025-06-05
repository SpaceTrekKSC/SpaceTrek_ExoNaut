/*
*ExoNaut_Ultrasonic.h
*
*Author:	Andrew Gafford
*Email:		agafford@spacetrek.com
*Date:		May 20th, 2024
*
*This library is for controlling the Space Trek ExoNaut Robot's RGB Ultrasonic
*distance sensor.  It provides functions to read the distance and set the RGB LEDS.
*
*/

#include "ExoNaut_Ultrasonic.h"



//Wire write byte
bool ultrasonic::wireWriteByte(uint8_t addr, uint8_t val){
	Wire.beginTransmission(addr);
	Wire.write(val);
	if(Wire.endTransmission() != 0){
		return false;
	}
	return true;
}

//Wire write multiple bytes
bool ultrasonic::wireWriteDataArray(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len){
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
int ultrasonic::wireReadDataArray(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len){
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




//Set ultrasonic rgb to breathing light mode
//r1, g1, b1 represent the breathing cycle of the right rgb light, such as 20, 20, 20, which represents a cycle of 2s
//r2, g2, b2 represent the breathing cycle of the left rgb light
void ultrasonic::breathing(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2){
	uint8_t breathing[6];
	uint8_t value = RGB_WORK_BREATHING_MODE;
	wireWriteDataArray(ULTRASOUND_I2C_ADDR, RGB_WORK_MODE_REG, &value, 1);
	breathing[0] = r1;breathing[1] = g1;breathing[2] = b1;								//RGB1 Blue
	breathing[3] = r2;breathing[4] = g2;breathing[5] = b2;								//RGB2
	wireWriteDataArray(ULTRASOUND_I2C_ADDR, RGB1_R_BREATHING_CYCLE_REG, breathing, 6);	//Send breathing values, (Address, start register, six breathing bytes in RGB[])
}

//Set the color of ultrasonic rgb light
//r1, g1, b1 represent the ratio of the three primary colors of the right rgb light, ranging from 0-255
//r2, g2, b2 represent the ratio of the three primary colors of the left RGB light, ranging from 0-255
void ultrasonic::color(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2){
	uint8_t RGB[6];
	uint8_t value = RGB_WORK_SIMPLE_MODE;
	wireWriteDataArray(ULTRASOUND_I2C_ADDR, RGB_WORK_MODE_REG,&value,1);
	RGB[0] = r1; RGB[1] = g1; RGB[2] = b1;												//RGB1
	RGB[3] = r2; RGB[4] = g2; RGB[5] = b2;												//RGB2
	wireWriteDataArray(ULTRASOUND_I2C_ADDR, RGB1_R_REG,RGB,6);							//Send color values, (Address, start register, six color bytes in RGB[])
}

//Get the distance unit measured by ultrasonic sensor in cm
uint16_t ultrasonic::getDistance(){
	uint16_t distance;
	wireReadDataArray(ULTRASOUND_I2C_ADDR, DISTANCE_L_REG,(uint8_t *)&distance,2);
	return distance;
}




void ultrasonic::getColor(){
	uint8_t rgbValues[6];
	wireReadDataArray(ULTRASOUND_I2C_ADDR, RGB1_R_REG, (uint8_t *)&rgbValues,6);
	this->rgb1[0] = rgbValues[0];
	this->rgb1[1] = rgbValues[1];
	this->rgb1[2] = rgbValues[2];
	this->rgb2[0] = rgbValues[3];
	this->rgb2[1] = rgbValues[3];
	this->rgb2[2] = rgbValues[5];
}


void getBreathing(){
	
}

uint8_t getBrightness(){
	
	return 0;
}

uint8_t getLEDMode(){
	
	return 0;
}