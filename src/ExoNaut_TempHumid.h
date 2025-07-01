/*
 * ExoNaut_TempHumid.h
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 * Updated: Self-contained version without external dependencies
 *
 * This library is for controlling the Space Trek ExoNaut Robot's Temperature & Humidity sensor
 * Direct I2C implementation for AHT20/AHT21/AHT25 sensors
 */

#ifndef __EXONAUT_TEMPHUMID_h
#define __EXONAUT_TEMPHUMID_h

#include "ExoNaut.h"

// AHT sensor I2C address and commands (from Adafruit library)
#define AHT_I2C_ADDR 0x38
#define AHT_CMD_CALIBRATE 0xE1	   // Calibration command (was 0xBE)
#define AHT_CMD_TRIGGER 0xAC	   // Trigger reading command
#define AHT_CMD_SOFTRESET 0xBA	   // Soft reset command
#define AHT_STATUS_BUSY 0x80	   // Status bit for busy
#define AHT_STATUS_CALIBRATED 0x08 // Status bit for calibrated
#define AHT_INIT_DATA1 0x08
#define AHT_INIT_DATA2 0x00
#define AHT_TRIGGER_DATA1 0x33
#define AHT_TRIGGER_DATA2 0x00

// Enum for temperature units (used internally)
enum TempUnit
{
	CELSIUS,
	FAHRENHEIT,
	KELVIN
};

class TempHumid
{
public:
	float temperature;	// Temperature in Fahrenheit
	float temperatureC; // Temperature in Celsius
	float humidity;		// Relative humidity percentage

	bool begin();		 // Initialize the sensor
	void readSensor();	 // Read temperature and humidity
	bool isConnected();	 // Check if sensor is connected
	void debugSensor();	 // Debug function to print raw data
	uint8_t getStatus(); // Get sensor status byte

	void setUnit(char unit); // Set the desired temperature unit using a character
	float getTemperature();	 // Get temperature in the selected unit

private:
	bool sendCommand(uint8_t cmd, uint8_t data1 = 0, uint8_t data2 = 0);
	bool waitForReady(uint16_t timeout_ms = 100);
	void calculateValues(uint8_t data[6]);
	TempUnit _unit = FAHRENHEIT; // Default to Fahrenheit for backward compatibility
};

#endif // __EXONAUT_TEMPHUMID_h