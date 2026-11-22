Oh, my dear friend, there you are at last. Do sit down—make yourself at home, please.

The thing is this: our old timer spits out but one number—the seconds since midnight. But the audience, they want a respectable `H:MM:SS`, you understand? No leading zero on the hours, even when they get frighteningly big; the minutes and the seconds must be exactly two digits—not one less, by God, not one.

And please, do not write your own `main`.

**In plain words**: implement `std::string format_duration(long long total_seconds)` returning `H:MM:SS`; no padding for hours, exactly two digits for minutes and seconds; `total_seconds` is non-negative.

### Example

- `0` gives `0:00:00`
- `3599` gives `0:59:59`
- `3600` gives `1:00:00`
- `86399` gives `23:59:59`
- `360000` gives `100:00:00`

### Constraints

- `H` may exceed 99 and has no leading zero.
- `MM` and `SS` must be exactly two digits, zero-padded.
- Do not write `main`.
