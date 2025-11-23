#include "Arduino_BMI270_BMM150.h"
#include "TaskScheduler.h"
#include "sense.h"
#include "evaluate.h"
#include <string.h>

void sample_gyro(void);

Task task_sample_gyro(SAMPLING_DELAY, TASK_FOREVER, sample_gyro);
Task task_evaluate_risk(0, 1, evaluate_risk);
Scheduler runner;

// worker_id hard-coded
danger_into_t info = { 1, D_NORMAL };

float initial_mean;
float initial_std_dev;
float gyro_data[SAMPLING_FREQ];
float gyro_buf[SAMPLING_FREQ];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (1);
  }

  Serial.println("IMU initialized");
  Serial.println("Gyroscope sample rate = " + String(IMU.gyroscopeSampleRate()) + "Hz");

  initialize_data();

  Serial.println("Initial mean: " + String(initial_mean));
  Serial.println("Initial std deviation: " + String(initial_std_dev));

  runner.init();
  runner.addTask(task_sample_gyro);
  runner.addTask(task_evaluate_risk);
  task_sample_gyro.enable();
}

void loop() {
}

void sample_gyro(void) {
  static int eval_first = 1;
  static int sample_count = 0;
  float gx, gy, gz;

  while (!IMU.gyroscopeAvailable());
  IMU.readGyroscope(gx, gy, gz);
  float gyro_abs = sqrt(gx * gx + gy * gy + gz * gz);

  if (sample_count < SAMPLING_FREQ) {
    gyro_data[sample_count] = gyro_abs;
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
