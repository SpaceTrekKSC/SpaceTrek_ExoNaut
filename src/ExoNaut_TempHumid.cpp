/*
 * ExoNaut_TempHumid.cpp
 *
 * Author:	Andrew Gafford
 * Email:		agafford@spacetrek.com
 * Date:		May 20th, 2024
 * Updated: Self-contained version without external dependencies
 *
 * This library is for controlling the Space Trek ExoNaut Robot's Temperature & Humidity sensor
 * Direct I2C implementation for AHT20/AHT21/AHT25 sensors
 */

#include "ExoNaut_TempHumid.h"
#include "ExoNaut.h" // Assuming ExoNaut.h provides access to Wire and delay

bool TempHumid::begin()
{
	// Wait for sensor to be ready after power-on (from Adafruit: 20ms)
	delay(20);

	// Check if sensor is connected
	if (!isConnected())
	{
		return false;
	}

	// Soft reset the sensor first (matching Adafruit sequence)
	if (!sendCommand(AHT_CMD_SOFTRESET))
	{
		return false;
	}
	delay(20);

	// Wait for sensor to not be busy after reset
	while (getStatus() & AHT_STATUS_BUSY)
	{
		delay(10);
	}

	// Send calibration command (matching Adafruit)
	if (!sendCommand(AHT_CMD_CALIBRATE, AHT_INIT_DATA1, AHT_INIT_DATA2))
	{
		return false;
	}

	// Wait for calibration to complete
	while (getStatus() & AHT_STATUS_BUSY)
	{
		delay(10);
	}

	// Verify sensor is calibrated
	if (!(getStatus() & AHT_STATUS_CALIBRATED))
	{
		return false;
	}

	return true;
}

void TempHumid::readSensor()
{
	// Send trigger measurement command (matching Adafruit)
	if (!sendCommand(AHT_CMD_TRIGGER, AHT_TRIGGER_DATA1, AHT_TRIGGER_DATA2))
	{
		this->temperature = 0.0;
		this->temperatureC = 0.0;
		this->humidity = 0.0;
		return;
	}

	// Wait for measurement to complete (matching Adafruit approach)
	while (getStatus() & AHT_STATUS_BUSY)
	{
		delay(10);
	}

	// Read measurement data (6 bytes)
	uint8_t data[6];
	Wire.requestFrom(AHT_I2C_ADDR, 6);
	int bytesRead = 0;
	while (Wire.available() && bytesRead < 6)
	{
		data[bytesRead] = Wire.read();
		bytesRead++;
	}

	// Verify we got all the data
	if (bytesRead >= 6)
	{
		// Calculate temperature and humidity from raw data (using Adafruit method)
		calculateValues(data);
	}
	else
	{
		// Set to error values if read failed
		this->temperature = 0.0;
		this->temperatureC = 0.0;
		this->humidity = 0.0;
	}
}

bool TempHumid::isConnected()
{
	Wire.beginTransmission(AHT_I2C_ADDR);
	return (Wire.endTransmission() == 0);
}

uint8_t TempHumid::getStatus()
{
	Wire.requestFrom(AHT_I2C_ADDR, 1);
	if (Wire.available())
	{
		return Wire.read();
	}
	return 0xFF; // Error indicator
}

bool TempHumid::sendCommand(uint8_t cmd, uint8_t data1, uint8_t data2)
{
	Wire.beginTransmission(AHT_I2C_ADDR);
	Wire.write(cmd);
	if (data1 != 0 || data2 != 0)
	{
		Wire.write(data1);
		Wire.write(data2);
	}
	return (Wire.endTransmission() == 0);
}

bool TempHumid::waitForReady(uint16_t timeout_ms)
{
	unsigned long startTime = millis();

	while (millis() - startTime < timeout_ms)
	{
		Wire.requestFrom(AHT_I2C_ADDR, 1);
		if (Wire.available())
		{
			uint8_t status = Wire.read();
			// Check if busy bit (bit 7) is clear
			if ((status & 0x80) == 0)
			{
				return true;
			}
		}
		delay(5);
	}
	return false;
}

void TempHumid::calculateValues(uint8_t data[6])
{
	// Using exact Adafruit calculation method
	// Extract humidity (20 bits) - Adafruit method
	uint32_t h = data[1];
	h <<= 8;
	h |= data[2];
	h <<= 4;
	h |= data[3] >> 4;
	this->humidity = ((float)h * 100) / 0x100000;

	// Extract temperature (20 bits) - Adafruit method
	uint32_t tdata = data[3] & 0x0F;
	tdata <<= 8;
	tdata |= data[4];
	tdata <<= 8;
	tdata |= data[5];
	this->temperatureC = ((float)tdata * 200 / 0x100000) - 50;

	// Convert to Fahrenheit
	this->temperature = (this->temperatureC * 1.8) + 32.0;

	// Constrain humidity to reasonable ranges
	if (this->humidity > 100.0)
		this->humidity = 100.0;
	if (this->humidity < 0.0)
		this->humidity = 0.0;
}

void TempHumid::debugSensor()
{
	Serial.println("=== AHT Sensor Debug ===");

	// Check connection
	Serial.print("Sensor connected: ");
	Serial.println(isConnected() ? "YES" : "NO");

	// Read status using the new getStatus method
	uint8_t status = getStatus();
	Serial.print("Status byte: 0x");
	Serial.println(status, HEX);
	Serial.print("Busy: ");
	Serial.println((status & AHT_STATUS_BUSY) ? "YES" : "NO");
	Serial.print("Calibrated: ");
	Serial.println((status & AHT_STATUS_CALIBRATED) ? "YES" : "NO");

	// Try reading raw data
	if (sendCommand(AHT_CMD_TRIGGER, AHT_TRIGGER_DATA1, AHT_TRIGGER_DATA2))
	{
		// Wait for measurement using status checking (like Adafruit)
		while (getStatus() & AHT_STATUS_BUSY)
		{
			delay(10);
		}

		uint8_t data[6];
		Wire.requestFrom(AHT_I2C_ADDR, 6);
		int bytesRead = 0;
		while (Wire.available() && bytesRead < 6)
		{
			data[bytesRead] = Wire.read();
			bytesRead++;
		}

		Serial.print("Bytes read: ");
		Serial.println(bytesRead);
		Serial.print("Raw data: ");
		for (int i = 0; i < bytesRead; i++)
		{
			Serial.print("0x");
			if (data[i] < 16)
				Serial.print("0");
			Serial.print(data[i], HEX);
			Serial.print(" ");
		}
		Serial.println();

		if (bytesRead >= 6)
		{
			// Using Adafruit's method for extraction
			uint32_t h = data[1];
			h <<= 8;
			h |= data[2];
			h <<= 4;
			h |= data[3] >> 4;

			uint32_t tdata = data[3] & 0x0F;
			tdata <<= 8;
			tdata |= data[4];
			tdata <<= 8;
			tdata |= data[5];

			Serial.print("Raw humidity: ");
			Serial.println(h);
			Serial.print("Raw temperature: ");
			Serial.println(tdata);
		}
	}
	else
	{
		Serial.println("Failed to send trigger command");
	}

	Serial.println("========================");
}

void TempHumid::setUnit(char unit)
{
	switch (unit)
	{
	case 'C':
	case 'c':
		_unit = CELSIUS;
		break;
	case 'K':
	case 'k':
		_unit = KELVIN;
		break;
	case 'F':
	case 'f':
	default:
		_unit = FAHRENHEIT;
		break;
	}
}

float TempHumid::getTemperature()
{
	switch (_unit)
	{
	case CELSIUS:
		return temperatureC;
	case FAHRENHEIT:
		return temperature;
	case KELVIN:
		return temperatureC + 273.15;
	default:
		return temperature; // Default to Fahrenheit
	}
}