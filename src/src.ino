#include "Wire.h"
#include "Servo.h"
#include "FuzzyCom.h"
#include "FuzzyEngine.h"
#include "FuzzyLogger.h"
#include "FuzzyGyro.h"
#include "FuzzyPIDController.h"
#include "Indexes.h"

#define PIN_LED 13
#define PIN_FRONT_ENGINE 3
#define PIN_BACK_ENGINE 4
#define PIN_LEFT_ENGINE 5
#define PIN_RIGHT_ENGINE 6

void notify(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_LED, HIGH);
    delay(50);
    digitalWrite(PIN_LED, LOW);
    delay(50);
  }
}

FuzzyCom com;
FuzzyEngine engine(PIN_FRONT_ENGINE, PIN_BACK_ENGINE, PIN_LEFT_ENGINE, PIN_RIGHT_ENGINE);
FuzzyGyro gyro;
FuzzyLogger logger;
FuzzyPIDController controller;

void setup() {
  com.set(VAR_IDX_LOOP_DELAY, 0);
  com.set(VAR_IDX_SAFEWORD, 1);
  com.set(VAR_IDX_KP, 100);
  com.set(VAR_IDX_KI, 0);
  com.set(VAR_IDX_KD, 0);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);
  Wire.begin();

  gyro.setup();
  engine.setup(1000, 2000);
  controller.setup(&engine, &gyro, &com);
  notify(3);

  engine.setAll(1000);
  logger.log("setup", "Throttle down, set 0 1 to start");
  while (com.get(VAR_IDX_SETUP) == 0) {
    com.update();
  }
  delay(3000);
  notify(2);
  delay(1000);
}

void loop() {
  com.update();

  if (com.get(VAR_IDX_SAFEWORD) == 0) {
    controller.stop();
    return;
  }

  controller.update();
}
