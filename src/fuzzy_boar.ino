#include "Wire.h"
#include "Servo.h"
#include "FuzzyCom.h"
#include "FuzzyEngine.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

#define PIN_LED 13
#define PIN_FRONT_ENGINE 3
#define PIN_BACK_ENGINE 4
#define PIN_LEFT_ENGINE 5
#define PIN_RIGHT_ENGINE 6

bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 gv;         // [x, y, z]            gyro sensor measurements
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

FuzzyCom com;
FuzzyEngine engine(PIN_FRONT_ENGINE, PIN_BACK_ENGINE, PIN_LEFT_ENGINE, PIN_RIGHT_ENGINE);

void setup() {
  Serial.begin(115200);
  com.set(0, 150);
  com.set(1, 995);
  engine.setup(1200, 1800);

  return;
  Wire.begin();

  while (Serial.available() && Serial.read()); // empty buffer
  while (!Serial.available());                 // wait for data
  while (Serial.available() && Serial.read()); // empty buffer again

  // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
  // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
  // the baud timing being too misaligned with processor ticks. You must use
  // 38400 or slower in these cases, or use some kind of external separate
  // crystal solution for the UART timer.

  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // wait for ready

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  // configure LED for output
  pinMode(PIN_LED, OUTPUT);
}

double k = 0.25;
int iter = 0;

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
//
//
void loop() {
  if (Serial.available() > 0) {
    com.read();
    if (com.hasMessage()) {
      Serial.println(com.message());
    }
  }
  return;
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    if (Serial.available() > 0) {
      int b = Serial.read();
      b = map(b, 0, 255, -128, 127);
      k = b / 128.0;
    }

    double angle = ypr[1] * 180/M_PI;
    double diff = k * 100 * angle / abs(angle);
    double base = 1400;

    double left = base + diff;
    double right = base - diff;

    engine.setLeft((int)left);
    engine.setRight((int)left);

    if (iter % 20 == 0) {
      Serial.println("===");
      Serial.println(ypr[0] * 180/M_PI);
      Serial.println(ypr[1] * 180/M_PI);
      Serial.println(ypr[2] * 180/M_PI);
      Serial.println("===");

      Serial.print("K ");
      Serial.println(k);
      Serial.print("Angle ");
      Serial.println(angle);
      Serial.print("Left ");
      Serial.println(left);
      Serial.print("Right ");
      Serial.println(right);
    }
    iter += 1;

  }
  delay(20);
}
