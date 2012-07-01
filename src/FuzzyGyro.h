#ifndef _FUZZY_GYRO_H
#define _FUZZY_GYRO_H

#include "Arduino.h"
#include "FuzzyLogger.h"
#include "MPU6050.h"

class FuzzyGyro {
  public:
    void setup();
    void update();

    float getYaw();
    float getPitch();
    float getRoll();
  private:
    MPU6050 mpu;
    bool dmpReady;
    uint16_t packetSize;
    Quaternion quaternion;
    VectorFloat gravity;
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
};

#endif
