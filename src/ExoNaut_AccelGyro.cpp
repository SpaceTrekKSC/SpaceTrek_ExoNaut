/*
 * ExoNaut_AccelGyro.cpp
 *
 * Author:    Andrew Gafford
 * Email:     agafford@spacetrek.com
 * Date:      February 26, 2025
 *
 * Implementation for the ExoNaut_AccelGyro library with MPU6050 sensor.
 */

 #include "ExoNaut_AccelGyro.h"

 // Global instance of the IMU data
 imu_data_t imu_data = {
     .accel_x = 0,
     .accel_y = 0,
     .accel_z = 0,
     .gyro_x = 0,
     .gyro_y = 0,
     .gyro_z = 0,
     .temp = 0,
     .pitch = 0,
     .roll = 0,
     .yaw = 0,
     .last_update = 0,
     .calibrated = false
 };
 
 ExoNautAccelGyro::ExoNautAccelGyro() : 
     gyro_offset_x(0), 
     gyro_offset_y(0), 
     gyro_offset_z(0),
     prev_imu_update_time(0)
 {
     // Constructor - initializes member variables
 }
 
 void ExoNautAccelGyro::writeMPU6050Register(uint8_t reg, uint8_t value) {
     Wire.beginTransmission(MPU6050_ADDR);
     Wire.write(reg);
     Wire.write(value);
     Wire.endTransmission(true);
 }
 
 uint8_t ExoNautAccelGyro::readMPU6050Register(uint8_t reg) {
     Wire.beginTransmission(MPU6050_ADDR);
     Wire.write(reg);
     Wire.endTransmission(false);
     Wire.requestFrom(MPU6050_ADDR, (uint8_t)1);
     return Wire.read();
 }
 
 void ExoNautAccelGyro::readMPU6050Data(int16_t* ax, int16_t* ay, int16_t* az, int16_t* temp, int16_t* gx, int16_t* gy, int16_t* gz) {
     Wire.beginTransmission(MPU6050_ADDR);
     Wire.write(MPU6050_REG_ACCEL_XOUT_H);
     Wire.endTransmission(false);
     Wire.requestFrom(MPU6050_ADDR, (uint8_t)14);
     
     *ax = (Wire.read() << 8) | Wire.read();
     *ay = (Wire.read() << 8) | Wire.read();
     *az = (Wire.read() << 8) | Wire.read();
     *temp = (Wire.read() << 8) | Wire.read();
     *gx = (Wire.read() << 8) | Wire.read();
     *gy = (Wire.read() << 8) | Wire.read();
     *gz = (Wire.read() << 8) | Wire.read();
 }
 
 bool ExoNautAccelGyro::beginIMU(void) {
     // Check if the sensor is responding
     uint8_t whoAmI = readMPU6050Register(MPU6050_REG_WHO_AM_I);
     if (whoAmI != 0x68 && whoAmI != 0x78) {  // Accept either address as valid
         Serial.print("Unexpected WHO_AM_I value: 0x");
         Serial.println(whoAmI, HEX);
         return false;
     }
     
     // Wake up the MPU6050
     writeMPU6050Register(MPU6050_REG_PWR_MGMT_1, 0x00); // Clear sleep bit
     
     // Set sample rate to 1kHz
     writeMPU6050Register(MPU6050_REG_SMPLRT_DIV, 0x07);
     
     // Configure low-pass filter
     writeMPU6050Register(MPU6050_REG_CONFIG, 0x06); // 5Hz low-pass filter
     
     // Configure gyroscope range (±500 deg/s)
     writeMPU6050Register(MPU6050_REG_GYRO_CONFIG, 0x08);
     
     // Configure accelerometer range (±4g)
     writeMPU6050Register(MPU6050_REG_ACCEL_CONFIG, 0x08);
     
     // Initialize timestamp
     prev_imu_update_time = millis();
     
     Serial.println("MPU6050 initialized successfully");
     return true;
 }
 
 bool ExoNautAccelGyro::updateIMU(void) {
     int16_t ax, ay, az, temp_raw, gx, gy, gz;
     
     // Read raw data from sensor
     readMPU6050Data(&ax, &ay, &az, &temp_raw, &gx, &gy, &gz);
     
     // Convert raw values to physical units
     // For accelerometer: 1g = 8192 LSB for ±4g range
     imu_data.accel_x = ax / 8192.0;
     imu_data.accel_y = ay / 8192.0;
     imu_data.accel_z = az / 8192.0;
     
     // For gyroscope: 1 deg/s = 65.5 LSB for ±500 deg/s range
     // Also apply calibration offsets
     imu_data.gyro_x = gx / 65.5 - gyro_offset_x;
     imu_data.gyro_y = gy / 65.5 - gyro_offset_y;
     imu_data.gyro_z = gz / 65.5 - gyro_offset_z;
     
     // For temperature: T = (TEMP_OUT / 340) + 36.53°C
     imu_data.temp = temp_raw / 340.0 + 36.53;
     
     // Calculate orientation
     calculateOrientation();
     
     // Update timestamp
     unsigned long current_time = millis();
     imu_data.last_update = current_time;
     prev_imu_update_time = current_time;
     
     return true;
 }
 
 void ExoNautAccelGyro::calibrateIMU(void) {
     Serial.println("Calibrating gyroscope - keep the robot still...");
     
     // Calibration routine - collect multiple samples to determine gyro offset
     const int num_samples = 100;
     float gyro_x_sum = 0, gyro_y_sum = 0, gyro_z_sum = 0;
     
     // Make sure the robot is still during calibration
     stop_motor(0);  // Stop all motors
     delay(500);     // Wait for robot to settle
     
     // Collect samples
     for (int i = 0; i < num_samples; i++) {
         int16_t ax, ay, az, temp_raw, gx, gy, gz;
         readMPU6050Data(&ax, &ay, &az, &temp_raw, &gx, &gy, &gz);
         
         // Convert to deg/s
         gyro_x_sum += gx / 65.5;
         gyro_y_sum += gy / 65.5;
         gyro_z_sum += gz / 65.5;
         
         delay(10);
     }
     
     // Calculate average offsets
     gyro_offset_x = gyro_x_sum / num_samples;
     gyro_offset_y = gyro_y_sum / num_samples;
     gyro_offset_z = gyro_z_sum / num_samples;
     
     Serial.println("Gyroscope calibration complete");
     Serial.print("Offsets: X=");
     Serial.print(gyro_offset_x);
     Serial.print(" Y=");
     Serial.print(gyro_offset_y);
     Serial.print(" Z=");
     Serial.println(gyro_offset_z);
     
     imu_data.calibrated = true;
 }
 
 void ExoNautAccelGyro::calculateOrientation(void) {
     // Calculate time since last update
     unsigned long current_time = millis();
     float dt = (current_time - prev_imu_update_time) / 1000.0f; // Time in seconds
     
     // Limit dt in case of delays
     if (dt > 0.2f) dt = 0.05f; // Assume 50Hz if there's a large gap
     
     // Convert gyro values from degrees to radians for calculation
     float gyro_x_rad = imu_data.gyro_x * PI / 180.0;
     float gyro_y_rad = imu_data.gyro_y * PI / 180.0;
     float gyro_z_rad = imu_data.gyro_z * PI / 180.0;
     
     // Get acceleration magnitudes
     float accel_magnitude = sqrt(imu_data.accel_x * imu_data.accel_x + 
                                 imu_data.accel_y * imu_data.accel_y + 
                                 imu_data.accel_z * imu_data.accel_z);
     
     // Only update with accelerometer data if we're stable
     if (accel_magnitude > 0.8 && accel_magnitude < 1.2) { // Check if close to 1g (±0.2g)
         // Calculate pitch and roll from accelerometer (in radians)
         float accel_pitch = atan2(imu_data.accel_y, sqrt(imu_data.accel_x * imu_data.accel_x + imu_data.accel_z * imu_data.accel_z));
         float accel_roll = atan2(-imu_data.accel_x, imu_data.accel_z);
         
         // Convert to degrees
         accel_pitch = accel_pitch * 180.0 / PI;
         accel_roll = accel_roll * 180.0 / PI;
         
         // Complementary filter - combine accelerometer and gyroscope data
         // Typically the gyroscope is weighted higher (0.95-0.98) to reduce noise
         const float alpha = 0.96;
         
         // If this is the first valid measurement, initialize with accelerometer values
         if (!imu_data.calibrated || (imu_data.pitch == 0 && imu_data.roll == 0)) {
             imu_data.pitch = accel_pitch;
             imu_data.roll = accel_roll;
         } else {
             // Complementary filter
             imu_data.pitch = alpha * (imu_data.pitch + imu_data.gyro_x * dt) + (1 - alpha) * accel_pitch;
             imu_data.roll = alpha * (imu_data.roll + imu_data.gyro_y * dt) + (1 - alpha) * accel_roll;
         }
     } else {
         // If accelerometer is unreliable (e.g., during movement), use only gyroscope
         imu_data.pitch += imu_data.gyro_x * dt;
         imu_data.roll += imu_data.gyro_y * dt;
     }
     
     // Integrate gyro for yaw (will drift over time without magnetometer)
     imu_data.yaw += imu_data.gyro_z * dt;
     
     // Keep yaw in 0-360 range
     if (imu_data.yaw < 0) imu_data.yaw += 360;
     if (imu_data.yaw >= 360) imu_data.yaw -= 360;
 }
 
 void ExoNautAccelGyro::getAcceleration(float *x, float *y, float *z) {
     *x = imu_data.accel_x;
     *y = imu_data.accel_y;
     *z = imu_data.accel_z;
 }
 
 void ExoNautAccelGyro::getRotation(float *x, float *y, float *z) {
     *x = imu_data.gyro_x;
     *y = imu_data.gyro_y;
     *z = imu_data.gyro_z;
 }
 
 void ExoNautAccelGyro::getOrientation(float *pitch, float *roll, float *yaw) {
     *pitch = imu_data.pitch;
     *roll = imu_data.roll;
     *yaw = imu_data.yaw;
 }
 
 float ExoNautAccelGyro::getTemperature(void) {
     return imu_data.temp;
 }
 
 bool ExoNautAccelGyro::detectShake(float threshold) {
     // Simple shake detection based on acceleration magnitude
     float mag = sqrt(imu_data.accel_x * imu_data.accel_x + 
                      imu_data.accel_y * imu_data.accel_y + 
                      imu_data.accel_z * imu_data.accel_z);
     
     // Check if acceleration exceeds the threshold (in g)
     return (mag > threshold);
 }
 
 bool ExoNautAccelGyro::isFlat(float tolerance) {
     // Check if the robot is on a flat surface by examining pitch and roll angles
     return (abs(imu_data.pitch) < tolerance && abs(imu_data.roll) < tolerance);
 }