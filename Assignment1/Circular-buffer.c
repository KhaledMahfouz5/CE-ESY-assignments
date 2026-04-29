#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	unsigned char *data;
	int size;
	int head; // index of next byte to read
	int tail; // index of last written byte (-1 when empty)
} cbuf_t;

void cbuf_init(cbuf_t *b, unsigned char *storage, int size) {
	b->data = storage;
	b->size = size;
	b->head = 0;
	b->tail = -1;
}

static inline bool cbuf_is_empty(const cbuf_t *b) {
	return b->tail == -1;
}

static inline bool cbuf_is_full(const cbuf_t *b) {
	if (b->tail == -1) return false;
	int next = (b->tail + 1) % b->size;
	return next == b->head;
}

int cbuf_write_byte(cbuf_t *b, unsigned char byte) {
	if (cbuf_is_full(b)) return 0;
	b->tail = (b->tail + 1) % b->size;
	b->data[b->tail] = byte;
	return 1;
}

int cbuf_read_byte(cbuf_t *b, unsigned char *out) {
	if (cbuf_is_empty(b)) return 0;
	*out = b->data[b->head];
	if (b->head == b->tail) {
		b->head = 0;
		b->tail = -1;
	} else {
		b->head = (b->head + 1) % b->size;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	// stack-allocated storage for circular buffer
	unsigned char storage[64];
	cbuf_t buf;
	cbuf_init(&buf, storage, sizeof(storage));

	// name on the stack
	char name[64];

	if (argc > 1 && strlen(argv[1]) < sizeof(name) - 1) {
		strncpy(name, argv[1], sizeof(name));
		name[sizeof(name) - 1] = '\0';
	} else {
		strncpy(name, "Student ", sizeof(name));
		name[sizeof(name) - 1] = '\0';
	}

	// append "ESY-CE" safely
	strncat(name, "ESY-CE", sizeof(name) - strlen(name) - 1);

	// write name (including terminating '\0') into circular buffer byte-by-byte
	for (size_t i = 0; i <= strlen(name); ++i) {
		if (!cbuf_write_byte(&buf, (unsigned char)name[i])) {
			fprintf(stderr, "Buffer full, stopped writing at byte %zu\n", i);
			break;
		}
	}

	// print head/tail indices
	printf("head:%d , tail:%d\n", buf.head, buf.tail);

	// read back and print as characters until empty
	unsigned char ch;
	while (cbuf_read_byte(&buf, &ch)) {
		if (ch == '\0') break; // stop at string terminator for this demo
		putchar(ch);
	}
	putchar('\n');

	return 0;
}

