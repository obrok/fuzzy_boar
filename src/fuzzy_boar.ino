#include "Wire.h"
#include "Servo.h"
#include "FuzzyCom.h"
#include "FuzzyEngine.h"
#include "FuzzyLogger.h"
#include "FuzzyGyro.h"
#include "FuzzyPIDController.h"

#define PIN_LED 13
#define PIN_FRONT_ENGINE 3
#define PIN_BACK_ENGINE 4
#define PIN_LEFT_ENGINE 5
#define PIN_RIGHT_ENGINE 6

#define VAR_IDX_SETUP 0
#define VAR_IDX_SAFEWORD 1
#define VAR_IDX_LOOP_DELAY 2
#define VAR_IDX_FRONT_ENGINE 3
#define VAR_IDX_BACK_ENGINE 4
#define VAR_IDX_LEFT_ENGINE 5
#define VAR_IDX_RIGHT_ENGINE 6

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
FuzzyAbstractController *controller = new FuzzyPIDController();

void setup() {
  com.set(VAR_IDX_LOOP_DELAY, 0);
  com.set(VAR_IDX_SAFEWORD, 1);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);
  Wire.begin();

  gyro.setup();
  engine.setup(1000, 2000);
  notify(3);

  while (com.get(VAR_IDX_SETUP) == 0) {
    if (com.update()) {
      Serial.println(com.getResponse());
    }
  }

  if (com.get(VAR_IDX_SETUP) == 2) {
    engine.setAll(1000);
  }

  while (com.get(VAR_IDX_SETUP) == 2) {
    if (com.update()) {
      Serial.println(com.getResponse());
    }
  }

  notify(4);

  controller -> setup(&engine, &gyro);
  notify(5);
  engine.setLeft(1500);
  engine.setRight(1500);
}

void loop() {
  return;
  if (logger.isAfterTimeout()) {
    logger.stop();
  }


  if (com.update()) {
    char *request = com.getRequest();
    if (request[0] == 'l' && request[1] == 'o' && request[2] == 'g') {
      logger.start(atoi(request + 4));
    } else {
      Serial.println(com.getResponse());
    }
  }

  delay(com.get(VAR_IDX_LOOP_DELAY));

  if (com.get(VAR_IDX_SAFEWORD) == 0) {
    controller -> stop();
    return;
  }

  controller -> update();
}
