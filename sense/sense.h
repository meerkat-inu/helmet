#ifndef SENSE_H
# define SENSE_H

# include "stat.h"

# define SAMPLING_FREQ 100
# define SAMPLING_DELAY 10 // 1000ms / 100
# define DATAS_PER_MIN 6000

enum danger_level {
	D_NORMAL = 0,
	D_CAUTION,
	D_DANGER,
	D_CRITICAL,
};

typedef struct danger_info {
	int worker_id;
	enum danger_level level;
} danger_info_t;

void initialize_data(void); // must be called only once.

#endif
