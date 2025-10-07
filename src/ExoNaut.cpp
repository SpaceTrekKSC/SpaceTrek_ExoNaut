// THIS IS THE CORRECT CONTENT FOR: ExoNaut.cpp

#include <Arduino.h>
#include "ExoNaut.h" // This now correctly includes ExoNautPixel.h
#include <Wire.h>
// #include <Adafruit_NeoPixel.h> // REMOVED - Ensure this is gone

void rx_task(void *pvParameter); // Forward declaration

// Use the integrated ExoNautPixelController for onboard LEDs
// NUM_PIXELS and NEO_PIXEL_PIN are defined in ExoNaut.h
// NEO_GRB and NEO_KHZ800 are defined in ExoNautPixel.h
// Assign RMT_CHANNEL_0 for the onboard LEDs.
ExoNautPixelController exonaut_pixels(NUM_PIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800, RMT_CHANNEL_0);

// --- Global variables for ExoNaut logic (these are fine here) ---
bool low_power_flag;
bool action_finish = true;

uart2_obj_t uart2_obj = {
	.initilized = false,
	.volt = -1,
	.version = "",
};

ir_obj_t ir = {
	.ir_key = 0,
	.ir_queue = NULL,
};

encoder_motor_obj_t encoder_motor = {
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

// --- Helper function (fine here) ---
inline uint8_t hex2int(uint8_t ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return (ch - '0');
	}
	if (ch >= 'A' && ch <= 'F')
	{
		return (ch - 'A' + 10);
	}
	if (ch >= 'a' && ch <= 'f')
	{
		return (ch - 'a' + 10);
	}
	return (uint8_t)-1;
}
// HEX_TO_INT macro is in ExoNaut.h, which is good

// --- exonaut Class Method Implementations ---
void exonaut::begin(void)
{
	pinMode(BUTTON_A_PIN, INPUT);
	pinMode(BUTTON_B_PIN, INPUT);
	ets_serial.begin(115200, SERIAL_8N1, 16, 17);
	delay(100);
	xTaskCreatePinnedToCore(rx_task, "rx_task", 3072, NULL, 2, &rx_task_handle, 0);
	delay(100);
	encoder_motor_set_speed_base(0, 0);
	delay(100);
	reset_encoder_counter(0);
	delay(100);
	this->set_motor_type(1);
	delay(100);

	Wire.begin();
	Wire.setClock(90000);

	// Initialize onboard LEDs using the new controller
	exonaut_pixels.begin();
	for (int i = 0; i < exonaut_pixels.numPixels(); i++)
	{
		exonaut_pixels.setPixelColor(i, ExoNautPixelController::Color(0, 0, 0));
	}
	exonaut_pixels.show();
	delay(500);
}

void exonaut::set_motor_speed(float m1, float m2)
{
	encoder_motor_set_speed_base(float(m1), float(m2));
}

void exonaut::set_motor_type(uint8_t motortype)
{
	if (motortype == 1 || motortype == 2)
	{
		uint8_t buf[] = {0x55, 0x55, 0x04, 55, 1, 0};
		buf[5] = motortype;
		ets_serial.write(buf, 6);
		switch (motortype)
		{
		case 1:
			encoder_motor.pulse_p_r = PULSE_COUNT;
			break;
		case 2:
			encoder_motor.pulse_p_r = 1400 + 31;
			break;
		default:
			encoder_motor.pulse_p_r = PULSE_COUNT;
			break;
		}
	}
}

void exonaut::encoder_motor_set_speed(uint8_t motorid, float new_speed)
{
	float new_speed1 = 0;
	float new_speed2 = 0;
	switch (motorid)
	{
	case 0:
		new_speed1 = new_speed;
		new_speed2 = new_speed;
		break;
	case 1:
		new_speed1 = new_speed;
		new_speed2 = -encoder_motor.speed_2;
		break;
	case 2:
		new_speed1 = -encoder_motor.speed_1;
		new_speed2 = new_speed;
		break;
	default:
		break;
	}
	encoder_motor_set_speed_base(new_speed1, new_speed2);
}

void exonaut::encoder_motor_get_speed(float items[])
{
	items[0] = -encoder_motor.speed_1;
	items[1] = -encoder_motor.speed_2;
}

void exonaut::stop_motor(uint8_t motorid)
{
	uint8_t buf[] = {0x55, 0x55, 0x05, 55, 0x02, 0x00, 0x00};
	float temp_speed1 = -encoder_motor.speed_1; // Use temp variables for calculation
	float temp_speed2 = -encoder_motor.speed_2;
	temp_speed1 = temp_speed1 / 55 * 90;
	temp_speed2 = temp_speed2 / 55 * 90;

	float rps1 = (float)(-temp_speed1) / 60.0f;
	float rps2 = (float)(-temp_speed2) / 60.0f;
	float pps1 = rps1 * 680;
	float pps2 = rps2 * 680;
	buf[5] = (uint8_t)((int)round(pps1 * 0.01f));
	buf[6] = (uint8_t)((int)round(pps2 * 0.01f));
	switch (motorid)
	{
	case 1:
		encoder_motor.speed_1 = 0;
		buf[5] = 0;
		break;
	case 2:
		encoder_motor.speed_2 = 0;
		buf[6] = 0;
		break;
	default:
		encoder_motor.speed_1 = 0;
		encoder_motor.speed_2 = 0;
		buf[5] = 0;
		buf[6] = 0;
		break;
	}
	ets_serial.write(buf, 7);
}

void exonaut::encoder_motor_set_speed_base(float new_speed1, float new_speed2)
{
	uint8_t buf[] = {0x55, 0x55, 0x05, 55, 0x02, 0x00, 0x00};
	encoder_motor.speed_1 = -new_speed1;
	encoder_motor.speed_2 = -new_speed2;
	// Use temporary variables for ratio conversion to avoid altering the stored speeds prematurely
	float temp_speed1 = new_speed1 / 55.0f * 90.0f;
	float temp_speed2 = new_speed2 / 55.0f * 90.0f;

	float rps1 = (float)(-temp_speed1) / 60.0f;
	float rps2 = (float)(-temp_speed2) / 60.0f;
	float pps1 = rps1 * 680.0f;
	float pps2 = rps2 * 680.0f;
	buf[5] = (uint8_t)((int)round(pps1 * 0.01f));
	buf[6] = (uint8_t)((int)round(pps2 * 0.01f));
	ets_serial.write(buf, 7);
}

float exonaut::encoder_motor_turn_base(float speed, float angle)
{
	float motor_speed = angle > 0 ? 0.2988f * speed : -0.2988f * speed; // Corrected typo: 0.298f to 0.2988f for symmetry
	float time = (float)(fabs(angle)) / speed * 1000.0f;
	encoder_motor_set_speed_base(motor_speed, -motor_speed);
	return time;
}

void exonaut::encoder_motor_turn(float speed, float angle)
{
	float time = encoder_motor_turn_base(speed, angle); // Removed redundant speed_1, angle_1
	delay((int)time);
}

void exonaut::reset_encoder_counter(uint8_t motorid)
{
	uint8_t buf[] = {0x55, 0x55, 0x03, 55, 0x03};
	ets_serial.write(buf, 5);
	delay(30); // Allow time for co-processor to update counts if needed before reading bases
	switch (motorid)
	{
	case 1:
		encoder_motor.count_base_1 = encoder_motor.count_1;
		break;
	case 2:
		encoder_motor.count_base_2 = encoder_motor.count_2;
		break;
	default:
		encoder_motor.count_base_1 = encoder_motor.count_1;
		encoder_motor.count_base_2 = encoder_motor.count_2;
		break;
	}
}

void exonaut::get_encoder_count(float items[])
{
	uint8_t buf[] = {0x55, 0x55, 0x03, 55, 0x03}; // This command might be to request update or just a header for reset logic
	// encoder_motor.counter_updated = false; // This flag isn't used in this function to gate update from rx_task
	ets_serial.write(buf, 5); // This seems to be a command to the co-processor
	delay(30);				  // Wait for potential response to update encoder_motor.count_1/2 via rx_task
	items[1] = (float)(encoder_motor.count_1 - encoder_motor.count_base_1) / encoder_motor.pulse_p_r;
	items[0] = (float)(encoder_motor.count_2 - encoder_motor.count_base_2) / encoder_motor.pulse_p_r;
}

void exonaut::setColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	exonaut_pixels.setPixelColor(n, ExoNautPixelController::Color(r, g, b));
}

void exonaut::setColorAll(uint8_t r, uint8_t g, uint8_t b)
{
	for (int i = 0; i < exonaut_pixels.numPixels(); i++)
	{
		exonaut_pixels.setPixelColor(i, ExoNautPixelController::Color(r, g, b));
	}
	exonaut_pixels.show();
}

void exonaut::show(void)
{
	exonaut_pixels.show();
}

void exonaut::clear(void)
{
	exonaut_pixels.clear();
	exonaut_pixels.show();
}

void exonaut::beginBusServo()
{
	uint8_t buf[10] = {0x55, 0x55, 0x08, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	ets_serial.write(buf, 10);
}

void exonaut::bus_servo_set_pose(uint8_t id, uint16_t pos, uint16_t time)
{
	uint8_t buf[10] = {0x55, 0x55, 0x08, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	buf[5] = time & 0x00FF;
	buf[6] = (time >> 8) & 0x00FF;
	buf[7] = id;
	buf[8] = pos & 0x00FF;
	buf[9] = (pos >> 8) & 0x00FF;
	ets_serial.write(buf, 10);
}

// --- rx_task (Co-processor communication) ---
// This task remains largely the same, ensure loop variables don't clash if modified.
// Renamed loop variables i,j to k_idx, j_idx in case 'E' in original code was a good catch.
void rx_task(void *pvParameter)
{
	static uint8_t rx_buf[64];
	static uint8_t cmd_buf[64];
	uint8_t loop_i = 0, index = 0; // Renamed 'i' to 'loop_i' to avoid conflict
	int8_t rxBytes;
	uint16_t ir_code = 0x0000;
	uint16_t ir_count = 0;

	ir.ir_queue = xQueueCreate(10, sizeof(ir_event_t));
	uart2_obj.initilized = true;

	// Initial command to co-processor?
	// rx_buf[0] = 0x55; // This was in the original, seems overwritten by the next lines
	// rx_buf[1] = 0x55;
	// rx_buf[2] = 0x02;
	// rx_buf[3] = 0x12;
	// ets_serial.write(rx_buf, 4); // If this is an init command, ensure it's correct

	ets_serial.flush();

	for (;;)
	{
		rxBytes = ets_serial.available();
		if (rxBytes > 0)
		{
			rxBytes = rxBytes > 63 ? 63 : rxBytes; // Cap read size
			ets_serial.readBytes(rx_buf, rxBytes);
			for (loop_i = 0; loop_i < rxBytes; ++loop_i)
			{ // Use loop_i
				if ('$' == rx_buf[loop_i])
				{
					uint8_t cmd = cmd_buf[0];
					cmd_buf[index] = '\0'; // Null-terminate
					switch (cmd)
					{
					case 'A':
						if (index == 7)
						{
							if (uart2_obj.volt == -1)
							{
								uart2_obj.volt = (((float)HEX_TO_INT(cmd_buf[1], cmd_buf[2])) * 51.765f);
							}
							else
							{
								uart2_obj.volt = uart2_obj.volt * 0.99f + (((float)HEX_TO_INT(cmd_buf[1], cmd_buf[2])) * 51.765f * 0.01f);
							}
							ir_code = ((((uint16_t)(HEX_TO_INT(cmd_buf[5], cmd_buf[6]))) << 8) & 0xFF00) | (HEX_TO_INT(cmd_buf[3], cmd_buf[4]));
						}
						if (index == 3)
						{ // Assuming this means "OK$" or similar short ack
							action_finish = true;
						}
						if (ir_code != ir.ir_key)
						{
							if (ir_code != 0x00)
							{
								ir_event_t event = {.ir_code = ir_code, .event = 1};
								if (ir.ir_queue != NULL)
									xQueueSend(ir.ir_queue, &event, 0);
							}
							else
							{
								if (ir_count >= 30)
								{
									ir_event_t event = {.ir_code = ir.ir_key, .event = 5};
									if (ir.ir_queue != NULL)
										xQueueSend(ir.ir_queue, &event, 0);
								}
								else
								{
									ir_event_t event = {.ir_code = ir.ir_key, .event = 2};
									if (ir.ir_queue != NULL)
										xQueueSend(ir.ir_queue, &event, 0);
								}
							}
							ir.ir_key = ir_code;
							ir_count = 0;
						}
						else
						{
							if (ir_code != 0)
							{
								if (ir_count == 30)
								{
									ir_event_t event = {.ir_code = ir_code, .event = 4};
									if (ir.ir_queue != NULL)
										xQueueSend(ir.ir_queue, &event, 0);
								}
								++ir_count;
								if (ir_count > 10000)
									ir_count = 31; // Prevent overflow, keep in long press state
							}
							else
							{
								ir_count = 0;
							}
						}
						break;
					case 'V':
						if (index == 4)
						{												   // e.g., "V123$"
							memcpy(uart2_obj.version, cmd_buf, index + 1); // +1 for null if cmd_buf has it
							// uart2_obj.version[index + 1] = '\0'; // Original had index+1, ensure size of version array
						}
						break;
					case 'I':
						// if (index == 4 && cmd_buf[1] == 'R' && cmd_buf[2] == 'O' && cmd_buf[3] == 'K') { }
						break;
					case 'S':
						// if (index == 5) { }
						break;
					case 'E':
						if (cmd_buf[1] == 'M' && cmd_buf[2] == 'M' && index == 19)
						{ // Example "EMM<16 hex chars>$" -> 3+16 = 19
							union
							{
								uint8_t bytes[8];
								int32_t tol[2];
							} i32a;
							for (int k_idx = 0, j_idx = 18; k_idx < 8; ++k_idx, j_idx -= 2)
							{
								i32a.bytes[k_idx] = HEX_TO_INT(cmd_buf[j_idx - 1 + (3 - 1)], cmd_buf[j_idx + (3 - 1)]); // Offset by "EMM"
							}
							encoder_motor.count_1 = -i32a.tol[0];
							encoder_motor.count_2 = -i32a.tol[1];
							encoder_motor.counter_updated = true; // Set flag
						}
						break;
					default:
						break;
					}
					memset(cmd_buf, 0, sizeof(cmd_buf));
					index = 0;
				}
				else if (index < (sizeof(cmd_buf) - 1) && !('\n' == rx_buf[loop_i]) && !('\r' == rx_buf[loop_i]))
				{ // Check bounds, ignore CR/LF
					cmd_buf[index++] = rx_buf[loop_i];
				}
				else
				{
					// Ignore, or handle buffer full / unexpected char
				}
			}
		}
		vTaskDelay(pdMS_TO_TICKS(20)); // Use FreeRTOS delay
	}
}