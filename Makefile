zstd-test: zstd-test.c
	$(CC) -o $@ $< $(shell pkg-config --cflags libzstd) $(CFLAGS) $(shell pkg-config --libs libzstd) $(LDFLAGS)

clean:
	rm -f zstd-test