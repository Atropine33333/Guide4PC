THE ORDER OF ERROR CODES

Article 1 Purpose
There are four sensors, numbered `0` to `3`. Each registration writes one reading into the place the caller supplies, and reports the reason when it cannot.

Article 2 Constants

```c
#define ERR_OK 0
#define ERR_ARG_NULL (-1)
#define ERR_ARG_RANGE (-2)

#define SENSOR_COUNT 4
#define READING_MAX 1000
```

Article 3 Interface

```c
int record_reading(int sensor, int value, int *out);
```

Article 4 Order of inspection
Look at `out` first: when it is null, return `ERR_ARG_NULL`. Then look at `sensor`: when it falls outside `0` to `SENSOR_COUNT - 1`, return `ERR_ARG_RANGE`. Finally look at `value`: when it falls outside `0` to `READING_MAX`, return `ERR_ARG_RANGE`.

Article 5 Registration
When all three pass, write `value` into `*out` and return `ERR_OK`.

Article 6 Refusal
A refusal never touches `*out`; the value held by the caller stays as it was.

Article 7 How it is called
`record_reading` is called directly, so implement the function itself and do not write `main`.

**In plain words**: check the three conditions in the order of Article 4. When they pass, write `*out` and return `0`; when they fail, return `-1` or `-2` and leave `*out` alone.

**Interface**: see Article 3.

### Example

```c
int v = -1;
record_reading(0, 25, &v);     // 0, and v is 25
record_reading(0, -1, &v);     // -2, and v is still 25
record_reading(4, 10, &v);     // -2, the number is out of range
record_reading(1, 10, 0);      // -1, the output pointer is null
```

### Contract

- Return the error code on failure and print nothing.
- Check `out` first, so `*out` is never written or dereferenced by mistake.
- Use the standard library only; no dynamic allocation, and do not write `main`.
