#include <stdio.h>
#include <stdlib.h>
#include <zstd.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 262144

void compressFile(const std::string inputFile, const std::string outputFile) {
    int fin = open(inputFile.c_str(), O_RDONLY, 0644);
    int fout = open(outputFile.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fin < 0 || fout < 0) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    ZSTD_CCtx* cctx = ZSTD_createCCtx();
    if (cctx == NULL) {
        fprintf(stderr, "ZSTD_createCCtx() error\n");
        exit(EXIT_FAILURE);
    }

    char inBuff[CHUNK_SIZE];
    char outBuff[ZSTD_compressBound(CHUNK_SIZE)];

    size_t n, toRead = CHUNK_SIZE;
    ZSTD_inBuffer input = { inBuff, 0, 0 };
    ZSTD_outBuffer output = { outBuff, sizeof(outBuff), 0 };

    while ((n = read(fin, inBuff, toRead)) > 0) {
        input.src = inBuff;
        input.size = n;
        input.pos = 0;

        while (input.pos < input.size) {
            output.dst = outBuff;
            output.size = sizeof(outBuff);
            output.pos = 0;

            size_t ret = ZSTD_compressStream(cctx, &output, &input);
            if (ZSTD_isError(ret)) {
                fprintf(stderr, "Compression error: %s\n", ZSTD_getErrorName(ret));
                exit(EXIT_FAILURE);
            }

            write(fout, outBuff, output.pos);
        }
    }

    output.dst = outBuff;
    output.size = sizeof(outBuff);
    output.pos = 0;

    size_t ret = ZSTD_endStream(cctx, &output);
    if (ZSTD_isError(ret)) {
        fprintf(stderr, "ZSTD_endStream() error: %s\n", ZSTD_getErrorName(ret));
        exit(EXIT_FAILURE);
    }

    write(fout, outBuff, output.pos);

    ZSTD_freeCCtx(cctx);
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