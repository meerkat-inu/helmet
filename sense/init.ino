#include "sense.h"

// gyroscope == 각속도 데이터
void initialize_data(void) {
  float initial_gyro_data[DATAS_PER_MIN];
  for (int i = 0; i < DATAS_PER_MIN; ++i) {
    while (!IMU.gyroscopeAvailable());
    float gx, gy, gz;
    IMU.readGyroscope(gx, gy, gz);
    initial_gyro_data[i] = sqrt(gx * gx + gy * gy + gz * gz);
    f.println(String(gx) + "," + String(gy) + "," + String(gz));
    f.flush();
    delay(SAMPLING_DELAY);
  }
  initial_mean = mean(initial_gyro_data, sizeof(initial_gyro_data) / sizeof(float));
  initial_std_dev = std_deviation(initial_gyro_data, sizeof(initial_gyro_data) / sizeof(float));
}

void initialize_sd(int pin_no) {
  if (!SD.begin(pin_no)) {
    Serial.println("Failed to begin SD card on pin " + String(pin_no));
    while (1);
  }
  Serial.println("SD card successfully began on pin " + String(pin_no));

  f = SD.open("data.csv", FILE_WRITE);
  if (!f) {
    Serial.println("Failed to open a file");
    while (1);
  }
}

void initialize_imu(void) {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (1);
  }

  Serial.println("IMU initialized");
  Serial.println("Gyroscope sample rate = " + String(IMU.gyroscopeSampleRate()) + "Hz");
}

void playSoundByLevel(int level) {
  mp3.playMp3Folder(level);
  delay(3000);
}

// RX, TX 핀에 제대로 연결.
// mp3 rx -> nano tx
// mp3 tx -> nano rx
void initialize_mp3(void) {
  mp3Serial.begin(9600);

  while (!mp3.begin(mp3Serial));

  mp3.volume(15);
  //playSoundByLevel(2);
}
