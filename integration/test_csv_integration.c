#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

// Callback za svako polje
void field_cb(void *field, size_t len, void *data) {
    (void)data;
    char *str = (char*)field;
    if (str) {
        printf("FIELD: %.*s\n", (int)len, str);
    } else {
        printf("FIELD: NULL\n");
    }
}

// Callback za kraj reda
void row_cb(int c, void *data) {
    (void)c;
    int *row_count = (int*)data;

    printf("ROW END\n"); // ROW END za sve redove

    if (*row_count == -1) {
        // Header završen, resetujemo brojač za prave podatke
        *row_count = 0;
    } else {
        (*row_count)++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    struct csv_parser parser;
    if (csv_init(&parser, CSV_APPEND_NULL) != 0) {
        fprintf(stderr, "Failed to initialize parser\n");
        fclose(fp);
        return 1;
    }

    char buf[1024];
    size_t bytes_read;
    int row_count = -1; // -1 znači da je header

    while ((bytes_read = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (csv_parse(&parser, buf, bytes_read, field_cb, row_cb, &row_count) != bytes_read) {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&parser)));
            csv_free(&parser);
            fclose(fp);
            return 1;
        }
    }

    csv_fini(&parser, field_cb, row_cb, &row_count);
    csv_free(&parser);
    fclose(fp);

    printf("Parsed %d data rows (excluding header)\n", row_count);

    if (row_count == 3) {
        printf("Integration test passed!\n");
    } else {
        printf("Integration test failed: expected 3 rows, got %d\n", row_count);
    }

    return 0;
}
