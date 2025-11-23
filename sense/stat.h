#ifndef STAT_H
# define STAT_H

# include <math.h>

float mean(const float *data, size_t n);
float variance(const float *data, size_t n);
float std_deviation(const float *data, size_t n);

#endif
