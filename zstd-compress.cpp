#include <stdio.h>
#include <stdlib.h>
#include <zstd.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 16384

void compressFile(const std::string inputFile, const std::string outputFile) {
    int fin = open(inputFile.c_str(), O_RDONLY, 0644);
    int fout = open(outputFile.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fin < 0 || fout < 0) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    char inBuff[CHUNK_SIZE];
    char outBuff[ZSTD_compressBound(CHUNK_SIZE)];

    size_t n, compressedSize;
    while ((n = read(fin, inBuff, CHUNK_SIZE)) > 0) {
        compressedSize = ZSTD_compress(outBuff, ZSTD_compressBound(CHUNK_SIZE), inBuff, n, 1);
        if (ZSTD_isError(compressedSize)) {
            fprintf(stderr, "Compression error: %s\n", ZSTD_getErrorName(compressedSize));
            exit(EXIT_FAILURE);
        }
        write(fout, outBuff, compressedSize);
    }

    close(fin);
    close(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    compressFile(std::string(argv[1]), std::string(argv[2]));
    return EXIT_SUCCESS;
}