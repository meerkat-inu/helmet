#include "Arduino_BMI270_BMM150.h"
#include "TaskScheduler.h"
#include "sense.h"
#include "evaluate.h"
#include <string.h>

float ax, ay, az;
float max_ax, max_az, min_ax, min_az;
#define TP 50
float accel[3][TP];

void sample_gyro(void);
void sample_accel(void);

//Task task_sample_gyro(SAMPLING_DELAY, TASK_FOREVER, sample_gyro);
//Task task_evaluate_risk(0, 1, evaluate_risk);
//Scheduler runner;
File f;
HardwareSerial& mp3Serial = Serial1;

DFRobotDFPlayerMini mp3;

// worker_id hard-coded
danger_info_t info = { 1, D_NORMAL };

float initial_mean;
float initial_std_dev;
float gyro_data[SAMPLING_FREQ];
float gyro_buf[SAMPLING_FREQ];

void setup() {
  Serial.begin(9600);
  //while (!Serial);

 // initialize_sd(D7);
  initialize_mp3();
  initialize_imu();

  //initialize_data();

  //Serial.println("Initial mean: " + String(initial_mean));
  //Serial.println("Initial std deviation: " + String(initial_std_dev));

  //runner.init();
  //runner.addTask(task_sample_gyro);
  //runner.addTask(task_evaluate_risk);
  //task_sample_gyro.enable();
}

void loop() {
  //runner.execute();
  sample_accel();
}

float max_arr(float *arr, int size) {
  float max = arr[0];
  for (int i = 1; i < size; ++i) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

float min_arr(float *arr, int size) {
  float min = arr[0];
  for (int i = 1; i < size; ++i) {
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return min;
}

void sample_accel(void) {
  static int idx = 0;
  static int eval_first = 1;
  //float ax, ay, az;
  while (!IMU.accelerationAvailable());
  IMU.readAcceleration(accel[0][idx], accel[1][idx], accel[2][idx]);
  ++idx;
  if (idx == TP) {
    max_ax = max_arr(accel[0], TP);
    min_ax = min_arr(accel[0], TP);
    max_az = max_arr(accel[2], TP);
    min_az = min_arr(accel[2], TP);
    evaluate_risk();
    memset(accel, 0, sizeof(accel));
    idx = 0;
  }
}
/*
void sample_gyro(void) {
  static int eval_first = 1;
  static int sample_count = 0;
  float gx, gy, gz;

  while (!IMU.gyroscopeAvailable());
  IMU.readGyroscope(gx, gy, gz);
  float gyro_abs = sqrt(gx * gx + gy * gy + gz * gz);

  if (sample_count < SAMPLING_FREQ) {
    gyro_data[sample_count] = gyro_abs;
    //Serial.println("gyro_abs: " + String(gyro_abs));
    ++sample_count;
  }
  else {
    sample_count = 0;
    memcpy(gyro_buf, gyro_data, sizeof(gyro_data));
    if (eval_first) {
      task_evaluate_risk.enable();
      eval_first = 0;
    }
    else {
      task_evaluate_risk.restart();
    }
  }
}
*/