#include "Arduino_BMI270_BMM150.h"
#include <math.h>

#define MINUTE_DATA 6000
#define SAMPLING_FREQ 100
#define SAMPLING_TIME 10 // 10ms

void initialize_data(void); // must be called only once.
float mean(const float *data, size_t n);
float std_deviation(const float *data, size_t n);

float w[MINUTE_DATA];
float initial_mean;
float initial_std_dev;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (42);
  }

  Serial.println("IMU initialized");
  Serial.println("Gyroscope sample rate = " + String(IMU.gyroscopeSampleRate()) + "Hz");

  initialize_data();

  Serial.println(initial_mean);
  Serial.println(initial_std_dev);
}

void loop() {
}

void initialize_data(void) {
  for (int i = 0; i < MINUTE_DATA; ++i) {
    while (!IMU.gyroscopeAvailable());
    float gx, gy, gz;
    IMU.readGyroscope(gx, gy, gz);
    w[i] = sqrt(gx * gx + gy * gy + gz * gz);
    Serial.println(w[i]);
    delay(SAMPLING_TIME);
  }
  initial_mean = mean(w, sizeof(w) / sizeof(float));
  initial_std_dev = std_deviation(w, sizeof(w) / sizeof(float));
}

float mean(const float *data, size_t n) {
	float m = 0;
	const float *p = data;
	while (p < data + n) {
		m += *p;
		++p;
	}
	m /= n;
	return m;
}

float variance(const float *data, size_t n) {
	float var = 0;
	float m = mean(data, n);
	const float *p = data;
	while (p < data + n) {
		var += (*p - m) * (*p - m);
		++p;
	}
	var /= n - 1; // sample variance
	return var;
}

float std_deviation(const float *data, size_t n) {
	return sqrt(variance(data, n));
}
