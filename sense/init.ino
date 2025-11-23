#include "sense.h"

// gyroscope == 각속도 데이터
void initialize_data(void) {
  float initial_gyro_data[DATAS_PER_MIN];
  for (int i = 0; i < DATAS_PER_MIN; ++i) {
    while (!IMU.gyroscopeAvailable());
    float gx, gy, gz;
    IMU.readGyroscope(gx, gy, gz);
    gyro_abs[i] = sqrt(gx * gx + gy * gy + gz * gz);
    Serial.println(gyro_abs[i]);
    delay(SAMPLING_DELAY);
  }
  initial_mean = mean(w, sizeof(w) / sizeof(float));
  initial_std_dev = std_deviation(w, sizeof(w) / sizeof(float));
}
