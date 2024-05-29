/*
*ExoNaut_TempHumid.h
*
*Author:	Andrew Gafford
*Email:		agafford@spacetrek.com
*Date:		May 20th, 2024
*
*This library is for controlling the Space Trek ExoNaut Robot's Temperature & Humidity sensor
*
*/

#include "ExoNaut_TempHumid.h"
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

bool TempHumid::begin(){
	return aht.begin();	
}

		float temperature;
		float temperatureC;
		float humidity;
		sensors_event_t temp;
		sensors_event_t humid;

void TempHumid::readSensor(){
	aht.getEvent(&this->humid, &this->temp);
	this->temperatureC = this->temp.temperature;
	this->temperature = (temperatureC * 1.8) + 32;
	this->humidity = humid.relative_humidity;	
}	
		