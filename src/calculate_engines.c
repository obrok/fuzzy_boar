#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

time_t start_time;

void init() {
  struct timeval t;
  gettimeofday(&t, NULL);
  start_time = t.tv_sec;
}

float get_current_time() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (float)(t.tv_sec - start_time) * 1000.0 + (float)(t.tv_usec) / 1000.0;
}

typedef struct measurement {
  float angle;
  float time;
} measurement;

typedef struct engines {
  float left;
  float right;
} engines;

float estimate_derivative(measurement old, measurement new) {
  return 1.0;
}

float estimate_integral(measurement old, measurement new) {
  float delta_time = new.time - old.time;
  float delta_angle = new.angle - old.angle;

  return old.angle * delta_time + 0.5 * delta_angle * delta_time;
}

measurement previous_measurment = {0.0, 0.0};

engines calculate_power(measurement old, measurement new) {
  engines e = {0.0, 0.0};
  float proportional_gain = 0.2;
  float derivative_gain = -0.2;
  float integral_gain = 1.0;

  float outcome = old.angle * proportional_gain + estimate_derivative(old, new) * derivative_gain + estimate_integral(old, new) * integral_gain;

  if (outcome >= 1.0) {
    outcome = 1.0;
  }

  if (outcome <= -1.0) {
    outcome = -1.0;
  }

  if (outcome > 0.0) {
    e.left = -outcome;
    e.right = 0.0;
  } else {
    e.right = outcome;
    e.left = 0.0;
  }

  return e;
}

engines calculate_engines(float angle) {
  engines e;
  measurement new;

  new.angle = angle;
  new.time = get_current_time();
  e = calculate_power(previous_measurment, new);
  previous_measurment = new;

  return e;
}
