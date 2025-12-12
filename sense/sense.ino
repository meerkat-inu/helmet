#include "Arduino_BMI270_BMM150.h"
#include "TaskScheduler.h"
#include "sense.h"
#include "evaluate.h"
#include <string.h>

#include <ArduinoBLE.h>

float max_ax, max_az, min_ax, min_az;
float max_gx, max_gz, min_gx, min_gz;
//int scanned = 0;
int scanned_time = millis();

#define TP 50
float accel[3][TP];
float gyro[3][TP];

void sample_gyro(void);
void sample_accel(void);

File f;
HardwareSerial& mp3Serial = Serial1;

DFRobotDFPlayerMini mp3;

//// worker_id hard-coded
//danger_info_t info = { 1, D_NORMAL };

float initial_mean;
float initial_std_dev;
float gyro_data[SAMPLING_FREQ];
float gyro_buf[SAMPLING_FREQ];

// 여기부터 수정
float txPower = -60.0;
float filteredRSSI = -60;
const float alpha = 0.5;
float distance = -1;

float warning_range = 1.5;
float danger_range	= 1.0;

void bleble();
// 여기까지

void setup() {
  initialize_imu();
  initialize_mp3();

  BLE.begin();
  BLE.scan(true);
}

void loop() {
  if (millis() - scanned_time >= 60000) {
    bleble();
  }
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
}

void sample_gyro(int idx) {
  while (!IMU.gyroscopeAvailable());
  IMU.readGyroscope(gyro[0][idx], gyro[1][idx], gyro[2][idx]);
}

void bleble() { // 안전일때 0, 1/2단계일때 1/2반환

  BLEDevice peripheral;
  String targetMac = "ec:62:60:80:fc:16";
  while (!(peripheral = BLE.available()) || (peripheral.address() != targetMac));
  scanned_time = millis();
}
