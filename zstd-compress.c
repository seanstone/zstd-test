#include <stdio.h>
#include <stdlib.h>
#include <zstd.h>

#define CHUNK_SIZE 16384

void compressFile(const char *inputFile, const char *outputFile) {
    FILE *fin = fopen(inputFile, "rb");
    FILE *fout = fopen(outputFile, "wb");
    if (!fin || !fout) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    void *inBuff = malloc(CHUNK_SIZE);
    void *outBuff = malloc(ZSTD_compressBound(CHUNK_SIZE));
    if (!inBuff || !outBuff) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    size_t read, compressedSize;
    while ((read = fread(inBuff, 1, CHUNK_SIZE, fin)) > 0) {
        compressedSize = ZSTD_compress(outBuff, ZSTD_compressBound(CHUNK_SIZE), inBuff, read, 1);
        if (ZSTD_isError(compressedSize)) {
            fprintf(stderr, "Compression error: %s\n", ZSTD_getErrorName(compressedSize));
            exit(EXIT_FAILURE);
        }
        fwrite(outBuff, 1, compressedSize, fout);
    }

    free(inBuff);
    free(outBuff);
    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    compressFile(argv[1], argv[2]);
    return EXIT_SUCCESS;
}