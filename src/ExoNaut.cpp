#include <Arduino.h>
#include "ExoNaut.h"
#include <Wire.h>

#include <Adafruit_NeoPixel.h>


Adafruit_NeoPixel pixels(NUM_PIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);


//bool low_power_flag;
bool action_finish = true;



void exonaut::begin(void){
	pinMode(BUTTON_A_PIN,INPUT);
	pinMode(BUTTON_B_PIN,INPUT);
	ets_serial.begin(115200);
	xTaskCreatePinnedToCore(rx_task, "rx_task", 3072, NULL, 2, &rx_task_handle, 0);
	//set_motor_speed(0, 0);
	//encoder_motor_set_speed_base(0, 0);
	//reset_encoder_counter(0);
	this->set_motor_type(1);
	
	Wire.begin();
	Wire.setClock(90000);
	
	pixels.begin();
	for(int i = 0; i < NUM_PIXELS; i++){
		pixels.setPixelColor(i, pixels.Color(0, 0, 0));
	}
	pixels.show();
}

void exonaut::set_motor_type(uint8_t motortype){
    if (motortype == 1 || motortype == 2) {
        uint8_t buf[] = {0x55, 0x55, 0x04, 55, 1, 0};
        buf[5] = motortype;
        ets_serial.write(buf,6);
		
		switch (motortype){
			case 1:{
				encoder_motor.pulse_p_r = PULSE_COUNT;
				break;
			}
			case 2:{
				encoder_motor.pulse_p_r = 1400 + 31;
				break;
			}
			default:{
				encoder_motor.pulse_p_r = PULSE_COUNT;
				break;
			}
		}

	}
}

void exonaut::encoder_motor_set_speed_base(float new_speed1, float new_speed2){		//this is the hw_encoder_motor_set_speed_base function
    uint8_t buf[] = {0x55, 0x55, 0x05, 55, 0x02, 0x00, 0x00};
	encoder_motor.speed_1 = -new_speed1;
	encoder_motor.speed_2 = -new_speed2;
	new_speed1 = new_speed1 / 55 * 90;			//1:55 -> 1:90
	new_speed2 = new_speed2 / 55 * 90;			//1:55 -> 1:90
	
    float rps1 = (float)(-new_speed1) / 60.0f;
    float rps2 = (float)(-new_speed2) / 60.0f;
    float pps1 = rps1 * 680;
    float pps2 = rps2 * 680;
    buf[5] = (uint8_t)((int)round(pps1 * 0.01f));
    buf[6] = (uint8_t)((int)round(pps2 * 0.01f));
    ets_serial.write(buf,7);
    //return 0;
}

float exonaut::encoder_motor_turn_base(float speed, float angle){					//Speed unit: degree/second
	float motor_speed = angle > 0 ? 0.2988f * speed : -0.298f * speed;
	float time = (float)(fabs(angle)) / speed * 1000.0f; // 单位 ms (unit: ms)
	encoder_motor_set_speed_base(motor_speed, -motor_speed);
	return time;
}

void exonaut::set_motor_speed(int m1, int m2){				//this is the set_motor_speed function
    uint8_t buf[6] = {0x55, 0x55, 0x04, 0x32, 0x00, 0x00};
    m1 = m1 > 100 ? 100 : m1;
    m2 = m2 > 100 ? 100 : m2;
    m1 = m1 < -100 ? -100 : m1;
    m2 = m2 < -100 ? -100 : m2;
    m1 = -m1;
    m2 = -m2;
    buf[4] = m2 & 0xFF;
    buf[5] = m1 & 0xFF;
    ets_serial.write(buf,6);
}















void exonaut::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
	pixels.setPixelColor(n, pixels.Color(r, g, b));
}

void exonaut::setColorAll(uint8_t r, uint8_t g, uint8_t b){
	for(int i = 0; i < NUM_PIXELS; i++){
		pixels.setPixelColor(i, pixels.Color(r, g, b));
	}
	pixels.show();
}

void exonaut::show(void){
	pixels.show();
}

void exonaut::clear(void){
	pixels.clear();
	pixels.show();
}










typedef struct __ir_event_t {
    uint16_t ir_code;
    int8_t event;
} ir_event_t;

// create uart2 instance
uart2_obj_t uart2_obj = {
    .initilized = false,
    .volt = -1,
    // .version[0] = "\0",
};

// IR remote instance
ir_obj_t ir = {
    .ir_key = 0,
    .ir_queue = NULL,
};

//encode motor object instance
encoder_motor_obj_t  encoder_motor = {
    .pulse_p_r = PULSE_COUNT,
    .motor_type = 1,
    .counter_updated = false,
    .speed_1 = 0,
    .speed_2 = 0,
    .count_1 = 0,
    .count_2 = 0,
    .count_base_1 = 0,
    .count_base_2 = 0,
};

inline uint8_t hex2int(uint8_t ch)
{//rx_task
    if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    }
    if (ch >= 'A' && ch <= 'F') {
        return (ch - 'A' + 10);
    }
    if (ch >= 'a' && ch <= 'f') {
        return (ch - 'a' + 10);
    }
    return (uint8_t) -1;
}









void rx_task(void *pvParameter){
	static uint8_t rx_buf[64];
	static uint8_t cmd_buf[64];
	uint8_t i = 0, index = 0;
	int8_t  rxBytes;
	uint16_t ir_code = 0x0000;
	uint16_t ir_count = 0;
	//static uint32_t low_power_first_ms = 0;

	ir.ir_queue = xQueueCreate(10, sizeof(ir_event_t));
	uart2_obj.initilized = true;

	rx_buf[0] = 0x55;
	rx_buf[1] = 0x55;
	rx_buf[2] = 0x02;
	rx_buf[3] = 0x12;

	ets_serial.flush();						// uart_flush(UART2_UART_NUM);
	// while(ets_serial.read() >= 0){}
	ets_serial.write(rx_buf,4);

	for(;;){
		rxBytes = ets_serial.available();
		if(rxBytes > 0){
			rxBytes = rxBytes > 63 ? 63 : rxBytes;
			ets_serial.readBytes(rx_buf, rxBytes);
			for (i = 0; i < rxBytes; ++i) {
				if ('$' == rx_buf[i]){
					uint8_t cmd = cmd_buf[0];
					cmd_buf[index] = '\0';
					switch(cmd){
						case 'A':  	// voltage and received ir code
							if(index == 7){
								if(uart2_obj.volt == -1){
									uart2_obj.volt = (((float)HEX_TO_INT(cmd_buf[1], cmd_buf[2])) * 51.765f);
								}
								else{
									uart2_obj.volt = uart2_obj.volt * 0.99f + (((float)HEX_TO_INT(cmd_buf[1], cmd_buf[2])) * 51.765f * 0.01f);
								}
								ir_code = ((((uint16_t)(HEX_TO_INT(cmd_buf[5], cmd_buf[6]))) << 8) & 0xFF00) | (HEX_TO_INT(cmd_buf[3], cmd_buf[4]));
								
                                // if (low_power_flag && uart2_obj.volt > 5000 && uart2_obj.volt < 6300 && !disable_low_power_alarm) {
                                //     if (low_power_first_ms == 0){
                                //         low_power_first_ms = mp_hal_ticks_ms();
                                //     }
                                //     uint32_t cur_ms = mp_hal_ticks_ms();
                                //     if(cur_ms - low_power_first_ms > 8000){
                                //         low_power_flag = false;
                                //         esp_restart();
                                //     }
                                // } else {
                                //   low_power_first_ms = 0;
                                // }
							}
							if (index == 3) {
								action_finish = true;
							}
							if (ir_code != ir.ir_key) { 											// ir key state changes
								if (ir_code != 0x00) { 												// If the new key status is not equal to 0, it means a transition from not pressed to pressed.
									ir_event_t event = {.ir_code = ir_code, .event = 1};			// Key is pressed
									// xQueueSend(ir.ir_queue, &event, 0);
								}
								else{ // When the new key state is not equal to 0, it indicates a transition from not pressed to pressed
									if(ir_count >= 30){
										ir_event_t event = {.ir_code = ir.ir_key, .event = 5};		// Release from long press
										// xQueueSend(ir.ir_queue, &event, 0);
									}
									else{
                                        ir_event_t event = {.ir_code = ir.ir_key, .event = 2};		// Release from short press
										// xQueueSend(ir.ir_queue, &event, 0);
									}
								}
                                ir.ir_key = ir_code;
                                ir_count = 0;
							}
							else{ // Button state doesn't change
								if(ir_code != 0){
									if(ir_count == 30){
										ir_event_t event = {.ir_code = ir_code, .event = 4};		// The button remains pressed without any change. Check the count, trigger a long press when reached
										// xQueueSend(ir.ir_queue, &event, 0);
									}
									++ir_count;
								}
								else{
									ir_count = 0;
								}
							}
							break;
						case 'V': // co-processor firmware version
							if(index == 4){
								memcpy(uart2_obj.version, cmd_buf, index);
								uart2_obj.version[index + 1] = '\0';
							}
							break;
						case 'I':    // learned ir code
							if(index == 4){
								if(cmd_buf[1] == 'R' && cmd_buf[2] == 'O' && cmd_buf[3] == 'K'){  // learning ok
									// play_tone_c(1000, 100, false);
									// play_tone_c(2000, 100, false);
								}
							}
							break;
						case 'S':				//??????????????
							if(index == 5){
							}
							break;
						case 'E':				// Encoder counts
							if(cmd_buf[1] == 'M' && cmd_buf[2] == 'M'){
								union {
									uint8_t bytes[8];
									int32_t tol[2];
								} i32a;
								for(int i = 0, j = 18; i < 8; ++i, j -= 2){
									i32a.bytes[i] = HEX_TO_INT(cmd_buf[j - 1], cmd_buf[j]);
								}
								encoder_motor.count_1 = -i32a.tol[0];
								encoder_motor.count_2 = -i32a.tol[1];
							}
							break;	//added by AG
						default:
							break;
					}
					memset(cmd_buf, 0, sizeof(cmd_buf));
					index = 0;
				}
				else if(!('\n' == rx_buf[i])){
					cmd_buf[index++] = rx_buf[i];
				}
				else{
					continue;
				}
			}
		}
		delay(20);
	}
}















/*
//Wire write byte
bool exonaut::wireWriteByte(uint8_t addr, uint8_t val){
	Wire.beginTransmission(addr);
	Wire.write(val);
	if(Wire.endTransmission() != 0){
		return false;
	}
	return true;
}

//Wire write multiple bytes
bool exonaut::wireWriteDataArray(uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len){
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
int exonaut::wireReadDataArray(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len){
	unsigned char i = 0;  
	if(!wireWriteByte(addr, reg)){				// Indicate which register we want to read from
		return -1;
	}
	Wire.requestFrom(addr, len, false);
	while(Wire.available()){					// Read block data
		if(i >= len){
			return -1;
		}
		val[i] = Wire.read();
		i++;
    }    
    return i;
}
*/