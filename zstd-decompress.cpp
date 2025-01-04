#include <stdio.h>
#include <stdlib.h>
#include <zstd.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 16384

void decompressFile(const std::string inputFile, const std::string outputFile) {
    int fin = open(inputFile.c_str(), O_RDONLY, 0644);
    int fout = open(outputFile.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fin < 0 || fout < 0) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    char inBuff[CHUNK_SIZE];
    char outBuff[CHUNK_SIZE];

    size_t n, decompressedSize;
    while ((n = read(fin, inBuff, CHUNK_SIZE)) > 0) {
        decompressedSize = ZSTD_decompress(outBuff, CHUNK_SIZE, inBuff, n);
        if (ZSTD_isError(decompressedSize)) {
            fprintf(stderr, "Decompression error: %s\n", ZSTD_getErrorName(decompressedSize));
            exit(EXIT_FAILURE);
        }
        write(fout, outBuff, decompressedSize);
    }

    close(fin);
    close(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    decompressFile(std::string(argv[1]), std::string(argv[2]));
    return EXIT_SUCCESS;
}