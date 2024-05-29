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

#ifndef __EXONAUT_TEMPHUMID_h
#define __EXONAUT_TEMPHUMID_h

#include "ExoNaut.h"
#include <Adafruit_AHTX0.h>

class TempHumid{
	public:
		float temperature;
		float temperatureC;
		float humidity;
		sensors_event_t temp;
		sensors_event_t humid;
		
		bool begin();
		void readSensor();
	private:
};

extern Adafruit_AHTX0 aht;

#endif //end __EXONAUT_TEMPHUMID_h