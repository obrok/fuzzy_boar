#include "FuzzyGyro.h"

extern FuzzyLogger logger;

volatile bool mpuInterrupt;
void dmpDataReady() {
  mpuInterrupt = true;
}

void FuzzyGyro::setup() {
  mpu.initialize();

  logger.log("gyro", "Initializing DMP...");
  int devStatus = mpu.dmpInitialize();

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    mpuInterrupt = false;
    mpu.setDMPEnabled(true);
    attachInterrupt(0, dmpDataReady, RISING);
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();

    logger.log("gyro", "Initialization complete");
  } else {
    logger.log("gyro", "Initialization failed");
  }
}

void FuzzyGyro::update() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
  }

  mpuInterrupt = false;
  uint8_t mpuIntStatus = mpu.getIntStatus();
  uint16_t fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }

    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &quaternion);
    mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
  }
}

float FuzzyGyro::getYaw() {
  return ypr[0];
}

float FuzzyGyro::getPitch() {
  return ypr[1];
}

float FuzzyGyro::getRoll() {
  return ypr[2];
}
