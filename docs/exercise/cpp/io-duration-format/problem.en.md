This exercise reads no stdin and writes no stdout. Implement a duration formatter; a hidden harness runs the tests.

Implement:

- `std::string format_duration(long long total_seconds)`: format the number of seconds as `H:MM:SS`.

Rules:

- `H` is the hour count without leading zeros and may exceed 99;
- `MM` and `SS` must each be exactly two digits, zero-padded;
- `total_seconds` is guaranteed to be non-negative.

For example, `0` becomes `0:00:00`, `3599` becomes `0:59:59`, and `3600` becomes `1:00:00`. Do not write `main`.
