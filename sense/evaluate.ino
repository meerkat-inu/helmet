#include "evaluate.h"

static void evaluate_risk_gyro(float gyro_mean) {
  // change later by experiments
  static float sigma_level1 = 2.0;
  static float sigma_level2 = 3.0;
  static float sigma_level3 = 4.0;

  // no abs
  float diff = gyro_mean - initial_mean;
  if (diff >= sigma_level3 * initial_std_dev) {
    info.level = D_CRITICAL;
  }
  else if (diff >= sigma_level2 * initial_std_dev) {
    info.level = D_DANGER;
  }
  else if (diff >= sigma_level1 * initial_std_dev) {
    info.level = D_CAUTION;
  }
  else {
    info.level = D_NORMAL;
  }
}

//float old_ax, old_ay, old_az;
// float ax, ay, az;
static void evaluate_risk_accl(void) {
  float diff_az = max_az - min_az;
  float diff_ax = max_ax - min_ax;
  //float abs_az = abs(az), abs_ax = abs(ax);
  if (diff_az > 3.0 || diff_ax > 1.2) {
    Serial.println("2단계:");
    Serial.println("az max: " + String(max_az) + " az min: " + String(min_az));
    Serial.println("ax max: " + String(max_ax) + " ax min: " + String(min_ax));
    Serial.println("diff_az: " + String(diff_az));
    Serial.println("diff_ax: " + String(diff_ax));
    playSoundByLevel(2);
  }
  else if (diff_az > 1.0 || diff_ax > 0.8) {
    Serial.println("1단계:");
    Serial.println("az max: " + String(max_az) + " az min: " + String(min_az));
    Serial.println("ax max: " + String(max_ax) + " ax min: " + String(min_ax));
    Serial.println("diff_az: " + String(diff_az));
    Serial.println("diff_ax: " + String(diff_ax));
    playSoundByLevel(1);
  }
  else {
    Serial.println("az max: " + String(max_az) + " az min: " + String(min_az));
    Serial.println("ax max: " + String(max_ax) + " ax min: " + String(min_ax));
  }
}

void evaluate_risk(void) {
  //float gyro_mean = mean(gyro_buf, sizeof(gyro_buf) / sizeof(float));
  //evaluate_risk_gyro(gyro_mean);
  evaluate_risk_accl();
}
