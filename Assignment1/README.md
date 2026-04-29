## Circular Buffer Demo

Short demo of a simple circular buffer in C that writes a name into a byte-wise ring buffer and reads it back.

### Files
- ` Circular-buffer.c` — the C source (circular buffer implementation + demo `main`).
- `README.md` — this file.

### Build
Compile with a POSIX C compiler:
```
cc -Wall -Wextra -O2 -o cbuf_test " Circular-buffer.c"
```
(Use the exact filename shown; tab-completion is recommended if your shell shows special glyphs.)

### Run
- No arguments: uses default name "Student ESY-CE"
```
./cbuf_test
```
- With a name argument (must be < 63 characters to avoid truncation):
```
./cbuf_test Alice
```

### Behavior
- The program appends "ESY-CE" to the provided name (or the default) and writes the resulting string (including terminating '\0') into a circular buffer, byte-by-byte.
- It prints the buffer `head` and `tail` indices, then reads and prints characters until it reaches the terminating `\0`.
- If the buffer fills while writing, the program prints an error to stderr indicating where writing stopped.

### Notes & Safety
- The circular buffer uses stack-allocated storage (64 bytes). Long input strings may be truncated; the program uses `strncpy`/`strncat` to avoid buffer overflows on the name buffer.
- The buffer implementation represents an empty buffer with `tail == -1`.
- To check for memory issues, run the compiled program under Valgrind if available:
```
valgrind --leak-check=full ./cbuf_test Alice
```

### License
Public domain / use as you wish.
