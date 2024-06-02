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

#ifndef __EXONAUT_LINEFOLLOW_h
#define __EXONAUT_LINEFOLLOW_h

#include "ExoNaut.h"

//Line Follower Definitions
#define LINE_FOLLOWER_I2C_ADDR		0x78
#define LF_CH1(x)					(x & 0x01)
#define LF_CH2(x)					((x>>1) & 0x01)
#define LF_CH3(x)					((x>>2) & 0x01)
#define LF_CH4(x)					((x>>3) & 0x01)


class lineFollower{
	public:
		bool CH1;
		bool CH2;
		bool CH3;
		bool CH4;
		bool readLineFollower(uint8_t &val);
	
	private:
		bool wireWriteByte(uint8_t addr, uint8_t val);
		bool wireWriteDataArray(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len);
		int wireReadDataArray(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
	
};




#endif