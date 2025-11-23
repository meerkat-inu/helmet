#include "stat.h"

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
