#ifndef _FUZZY_ENGINE_H
#define _FUZZY_ENGINE_H

#include "Arduino.h"
#include "Servo.h"

class FuzzyEngine {
  public:
    FuzzyEngine(int frontPin, int backPin, int leftPin, int rightPin);
    void setup(int low, int high);
    void stop();

    int setFront(int speed);
    int setBack(int speed);
    int setLeft(int speed);
    int setRight(int speed);
    void setAll(int speed);

    double setFront(double speed);
    double setBack(double speed);
    double setLeft(double speed);
    double setRight(double speed);

    int getFront();
    int getBack();
    int getLeft();
    int getRight();

    void changeRight(int i);
    void changeLeft(int i);

  private:
    Servo front;
    Servo back;
    Servo left;
    Servo right;

    int low;
    int high;
    int frontSpeed;
    int backSpeed;
    int leftSpeed;
    int rightSpeed;

    int frontPin;
    int backPin;
    int leftPin;
    int rightPin;

    int setSpeed(Servo *servo, int speed, int *speedContainer);
    double setSpeed(Servo *servo, double speed, int *speedContainer);
};

#endif
