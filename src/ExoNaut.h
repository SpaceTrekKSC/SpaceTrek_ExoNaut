/*
*ExoNaut.h
*
*Author:	Andrew Gafford
*Email:		agafford@spacetrek.com
*Date:		May 20th, 2024
*
*This library is for controlling the Space Trek ExoNaut Robot.  It provides
*control of the drive motors and CoreX components.
*
*/

#ifndef __EXONAUT_h
#define __EXONAUT_h

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Adafruit_NeoPixel.h>

//Port Pin Mappings

//Serial for motor control (possible other CoreX features)
#define ets_serial Serial2
#define HEX_TO_INT(high, low)   ((uint8_t)((0xF0 & (((hex2int((high))) << 4))) | (0x0F & (hex2int((low))))))

//Co-processor definitions

//encoder definitions
#define PULSE_COUNT 				1120			//encoder pulses per revolution of output shaft

//On board Neo Pixel definitions
#define NEO_PIXEL_PIN				23
#define NUM_PIXELS					6

//On board button PIN definitions
#define BUTTON_A_PIN				0
#define BUTTON_B_PIN				2

//On board buzzer Definitions

//On board IR Definitions

//On board Mic definitions



//AI-Camera Definitions

//IMU Definitions

//Temp & Humidity Definitions

//MP3 Definitions

//7-Segment Display Definitions

//Dot matrix definitions






typedef struct __uart_obj_t {
	bool initilized;
	float volt;
	char version[8];
} uart2_obj_t;

typedef struct __ir_obj_t {
	uint16_t ir_key;
	QueueHandle_t ir_queue;
} ir_obj_t;

typedef struct __ir_event_t {
    uint16_t ir_code;
    int8_t event;
} ir_event_t;

typedef struct __encoder_motor_obj_t {
	float pulse_p_r;
	uint8_t motor_type;
	bool counter_updated;
	float speed_1;
	float speed_2;
	int32_t count_1;
	int32_t count_2;
	int32_t count_base_1;
	int32_t count_base_2;
} encoder_motor_obj_t;





class exonaut{
	public:
		//Motor control
		void begin(void);
		void set_motor_speed(float m1, float m2);								//set normal motor speed //this is the set_motor_speed function
		void set_motor_type(uint8_t motortype);								//set encoder motor type //this is the hw_encoder_motor_set_motor_type function
		void encoder_motor_set_speed(uint8_t motorid, float new_speed);		//set speed //this is the hw_encoder_motor_set_speed function
		void encoder_motor_get_speed(float items[]);						//get speed of both motors
		void stop_motor(uint8_t motorid);									//stop the motorid's encoder motor
		void encoder_motor_turn(float speed, float angle);					//rotate angle in degrees per second
		
		//Encoder Control
		void reset_encoder_counter(uint8_t motorid);						//Reset the encoder count value of motorid's encoder motor (ie set to 0)
		void get_encoder_count(float items[]);								//Get the encoder count value (ie the number of turns)

		//LED control
		void setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
		void setColorAll(uint8_t r, uint8_t g, uint8_t b);
		void show(void);
		void clear(void);
		
		//bus servos
		void bus_servo_set_pose(uint8_t id, uint16_t pos, uint16_t time);

	private:
		TaskHandle_t rx_task_handle;
		float encoder_motor_turn_base(float speed, float angle); 		
		void encoder_motor_set_speed_base(float new_speed1, float new_speed2);		//this is the hw_encoder_motor_set_speed_base function
};


extern Adafruit_NeoPixel pixels;
extern uart2_obj_t uart2_obj;
extern encoder_motor_obj_t encoder_motor;

#endif //__EXONAUT_h