// 参考实现：c-error-code
#include <stddef.h>

#define ERR_OK 0
#define ERR_ARG_NULL (-1)
#define ERR_ARG_RANGE (-2)

#define SENSOR_COUNT 4
#define READING_MAX 1000

int record_reading(int sensor, int value, int *out);

int record_reading(int sensor, int value, int *out) {
    if (out == NULL) return ERR_ARG_NULL;
    if (sensor < 0 || sensor >= SENSOR_COUNT) return ERR_ARG_RANGE;
    if (value < 0 || value > READING_MAX) return ERR_ARG_RANGE;
    *out = value;
    return ERR_OK;
}
