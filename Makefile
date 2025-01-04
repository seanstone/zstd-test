all: zstd-compress zstd-decompress

zstd-compress: zstd-compress.cpp
	$(CXX) -o $@ $< $(shell pkg-config --cflags libzstd) $(CFLAGS) $(shell pkg-config --libs libzstd) $(LDFLAGS)

zstd-decompress: zstd-decompress.cpp
	$(CXX) -o $@ $< $(shell pkg-config --cflags libzstd) $(CFLAGS) $(shell pkg-config --libs libzstd) $(LDFLAGS)

clean:
	rm -f zstd-compress zstd-decompress