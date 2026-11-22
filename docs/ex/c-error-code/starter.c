#include <stddef.h>

#define ERR_OK 0
#define ERR_ARG_NULL (-1)
#define ERR_ARG_RANGE (-2)

#define SENSOR_COUNT 4
#define READING_MAX 1000

int record_reading(int sensor, int value, int *out);

int record_reading(int sensor, int value, int *out) {
    /* TODO: 先查 out，再查 sensor，最后查 value；
       不合格时返回对应的错误码，合格时写 *out 并返回 ERR_OK。 */
    (void)sensor;
    (void)value;
    (void)out;
    return ERR_ARG_NULL;
}
