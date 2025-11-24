#include "stat.h"
#include "SD.h"

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

void evaluate_risk(void) {
    float gyro_mean = mean(gyro_buf, sizeof(gyro_buf) / sizeof(float));
    evaluate_risk_gyro(gyro_mean);

    // temporary code
    /*
    if (info.level != D_NORMAL) {
      f.println(String(info.worker_id) + "," + String(info.level));
      f.flush();
    }
    */
    
    if (info.level != D_NORMAL) {
      Serial.println("[결과] worker: " + String(info.worker_id) + " | 위험도: " + String(info.level));
      Serial.println("  gyro_mean: " + String(gyro_mean));
    }
    
}
