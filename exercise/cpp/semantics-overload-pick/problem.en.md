This exercise reads no stdin and writes no stdout. Implement three overloads; a hidden harness runs the tests.

Implement:

- `const char* pick(int value)`: return `"int"`.
- `const char* pick(long value)`: return `"long"`.
- `const char* pick(double value)`: return `"double"`.

The harness passes arguments of several types and checks which overload C++ selects. Do not write `main`.
