#include "Wire.h"
#include "Servo.h"
#include "FuzzyCom.h"
#include "FuzzyEngine.h"
#include "FuzzyLogger.h"
#include "FuzzyGyro.h"
#include "FuzzyController.h"

#define PIN_LED 13
#define PIN_FRONT_ENGINE 3
#define PIN_BACK_ENGINE 4
#define PIN_LEFT_ENGINE 5
#define PIN_RIGHT_ENGINE 6

#define VAR_IDX_SETUP 0
#define VAR_IDX_LEFT_ENGINE 1
#define VAR_IDX_RIGHT_ENGINE 2
#define VAR_IDX_K 3

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
FuzzyController controller(&engine, &gyro);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(PIN_LED, OUTPUT);

  notify(3);

  while (com.get(VAR_IDX_SETUP) == 0) {
    if (Serial.available() > 0) {
      com.read();
      if (com.hasMessage()) {
        Serial.println(com.getResponse());
      }
    }
  }

  com.set(VAR_IDX_K, 25);
  notify(4);

  engine.setup(1200, 1800);
  gyro.setup();
  notify(5);
}

void loop() {
  logger.log("loop", "Starting loop");
  if (logger.isAfterTimeout()) {
    logger.stop();
  }

  if (Serial.available() > 0) {
    com.read();
    if (com.hasMessage()) {
      char *request = com.getRequest();
      if (request[0] == 'l' && request[1] == 'o' && request[2] == 'g') {
        logger.start(atoi(request + 4));
      } else {
        Serial.println(com.getResponse());
      }
    }
  }

  int left = com.get(VAR_IDX_LEFT_ENGINE);
  engine.setLeft(left / 100.0);

  int right = com.get(VAR_IDX_RIGHT_ENGINE);
  engine.setRight(right / 100.0);

  gyro.update();
  controller.update();
  logger.log("gyro", "yaw %d pitch %d roll %d", (int)(gyro.getYaw() * 180 / M_PI), (int)(gyro.getPitch() * 180 / M_PI), (int)(gyro.getRoll() * 180 / M_PI));
}
