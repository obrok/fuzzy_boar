#define MPU6050_ADDRESS 0x68
#define MPU6050_RA_WHO_AM_I 0x75
#define MPU6050_WHO_AM_I_BIT 6
#define MPU6050_WHO_AM_I_LENGTH 6
#define MPU6050_RA_GYRO_CONFIG 0x1B
#define MPU6050_GCONFIG_FS_SEL_BIT 4
#define MPU6050_GCONFIG_FS_SEL_LENGTH 2
#define MPU6050_RA_GYRO_XOUT_H 0x43
#define MPU6050_RA_ACCEL_XOUT_H 0x3B
#define MPU6050_RA_CONFIG 0x1A
#define MPU6050_CFG_DLPF_CFG_BIT 2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#include <Wire.h>

uint8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length) {
  // Read length bits from bitStart, LSB is 0
  uint8_t b = readByte(devAddr, regAddr);
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  b &= mask;
  b >>= (bitStart - length + 1);

  return b;
}

void writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
  uint8_t b = readByte(devAddr, regAddr);
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  data <<= (bitStart - length + 1); // shift data into correct position
  data &= mask; // zero all non-important bits in data
  b &= ~(mask); // zero all important bits in existing byte
  b |= data; // combine data with existing byte

  writeByte(devAddr, regAddr, b);
}

uint8_t readByte(int address, int reg) {
  uint8_t result = 0;
  readBytes(address, reg, 1, &result);

  return result;
}

void readBytes(int address, int reg, int number, uint8_t* bytes) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.requestFrom(address, number);
  for(int i = 0; i < number && Wire.available(); i++){
    bytes[i] = Wire.read();
  }
  Wire.endTransmission();
}

void writeByte(int address, int reg, uint8_t byte) {
  writeBytes(address, reg, 1, &byte);
}

void writeBytes(int address, int reg, int number, uint8_t* bytes) {
  Wire.beginTransmission(address);
  Wire.write(reg);

  for(int i = 0; i < number; i++){
    Wire.write(bytes[i]);
  }
  Wire.endTransmission();
}

uint8_t deviceID() {
  return readBits(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH);
}


/*          |   ACCELEROMETER    |           GYROSCOPE */
/* DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate */
/* ---------+-----------+--------+-----------+--------+------------- */
/* 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz */
/* 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz */
/* 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz */
/* 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz */
/* 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz */
/* 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz */
/* 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz */
uint8_t lowPassFilterMode() {
  return readBits(MPU6050_ADDRESS, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH);
}

uint8_t setLowPassFilterMode(uint8_t value) {
  writeBits(MPU6050_ADDRESS, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, value);
}

void getRotation(int16_t* x, int16_t* y, int16_t* z) {
  uint8_t buffer[6];
  readBytes(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 6, buffer);

  *x = (((int16_t)buffer[0]) << 8) | buffer[1];
  *y = (((int16_t)buffer[2]) << 8) | buffer[3];
  *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
  uint8_t buffer[6];
  readBytes(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);

  *x = (((int16_t)buffer[0]) << 8) | buffer[1];
  *y = (((int16_t)buffer[2]) << 8) | buffer[3];
  *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void setup() {
  Wire.begin();
  Serial.begin(9600);

  Serial.println("Device ID");
  Serial.println(deviceID(), HEX);

  Serial.println("Low pass filter mode");
  Serial.println(lowPassFilterMode(), HEX);
}

void loop() {
  int x, y, z;
  getRotation(&x, &y, &z);
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println(" ");

  delay(100);
}
