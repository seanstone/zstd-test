all: zstd-compress zstd-decompress

zstd-compress: zstd-compress.c
	$(CC) -o $@ $< $(shell pkg-config --cflags libzstd) $(CFLAGS) $(shell pkg-config --libs libzstd) $(LDFLAGS)

zstd-decompress: zstd-decompress.c
	$(CC) -o $@ $< $(shell pkg-config --cflags libzstd) $(CFLAGS) $(shell pkg-config --libs libzstd) $(LDFLAGS)

clean:
	rm -f zstd-compress zstd-decompress