#include "csv.h"
#include <stdio.h>
#include <stdlib.h>

void cb_field(void *field, size_t len, void *data) {
    (void)field;
    (void)len;
    (void)data;
}

void cb_row(int c, void *data) {
    (void)c;
    (void)data;
}

int main(int argc, char **argv) {

    if (argc < 2)
        return 1;

    struct csv_parser parser;

    if (csv_init(&parser, 0) != 0)
        return 1;

    FILE *f = fopen(argv[1], "rb");

    if (!f) {
        csv_free(&parser);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size < 0) {
        fclose(f);
        csv_free(&parser);
        return 1;
    }

    char *buffer = malloc(size + 1);

    if (!buffer) {
        fclose(f);
        csv_free(&parser);
        return 1;
    }

    fread(buffer, 1, size, f);
    buffer[size] = '\0';

    fclose(f);

    csv_parse(
        &parser,
        buffer,
        size,
        cb_field,
        cb_row,
        NULL
    );

    free(buffer);
    csv_free(&parser);

    return 0;
}
