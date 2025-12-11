#include "Arduino_BMI270_BMM150.h"
#include "TaskScheduler.h"
#include "sense.h"
#include "evaluate.h"
#include <string.h>

float max_ax, max_az, min_ax, min_az;
float max_gx, max_gz, min_gx, min_gz;

#define TP 50
float accel[3][TP];
float gyro[3][TP];

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
  //initialize_mp3();
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
  static int idx = 0;
  sample_accel(idx);
  sample_gyro(idx);
  ++idx;
  if (idx == TP) {
    max_ax = max_arr(accel[0], TP);
    min_ax = min_arr(accel[0], TP);
    max_az = max_arr(accel[2], TP);
    min_az = min_arr(accel[2], TP);

    max_gx = max_arr(gyro[0], TP);
    min_gx = min_arr(gyro[0], TP);
    max_gz = max_arr(gyro[2], TP);
    min_gz = min_arr(gyro[2], TP);

/*
    Serial.print(200*(max_ax - min_ax));
    Serial.print("\t");
    Serial.print(200*(max_az - min_az));
    Serial.print("\t");
    Serial.print(max_gx - min_gx);
    Serial.print("\t");
    Serial.println(max_gz - min_gz);
*/
    evaluate_risk();
    idx = 0;
  }
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

void sample_accel(int idx) {
  while (!IMU.accelerationAvailable());
  IMU.readAcceleration(accel[0][idx], accel[1][idx], accel[2][idx]);
  //Serial.println(String(accel[0][idx]) + "\t" + String(accel[2][idx]));
}

void sample_gyro(int idx) {
  while (!IMU.gyroscopeAvailable());
  IMU.readGyroscope(gyro[0][idx], gyro[1][idx], gyro[2][idx]);
  //Serial.println(String(200) + "\t" + String(gyro[0][idx]) + "\t" + String(gyro[1][idx]) + "\t" + String(gyro[2][idx]) + "\t" + String(-200));
}
