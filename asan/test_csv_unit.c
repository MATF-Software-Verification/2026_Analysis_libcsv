#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Globalna kontrola ispisa */
int verbose = 1;

/* Callback funkcija za svako CSV polje */
void cb_field(void *field, size_t len, void *data) {
    if (!verbose) return;
    printf("FIELD: %.*s\n", (int)len, (char *)field);
}

/* Callback funkcija za kraj reda */
void cb_row(int c, void *data) {
    if (!verbose) return;
    printf("ROW END\n");
}

int main() {

    struct csv_parser parser;

    if (csv_init(&parser, 0) != 0) {
        fprintf(stderr, "Greška pri inicijalizaciji parsera\n");
        return EXIT_FAILURE;
    }

    printf("\n===== libcsv ASan test =====\n");

    /* =========================
       1. Dugo polje (stress test realloc)
       ========================= */
    printf("\n--- Long field test ---\n");

    char *long_input = malloc(100000);
    if (!long_input) return EXIT_FAILURE;

    memset(long_input, 'A', 99998);
    long_input[99998] = '\n';
    long_input[99999] = '\0';

    csv_parse(&parser, long_input, strlen(long_input),
              cb_field, cb_row, NULL);
    free(long_input);

    /* =========================
       2. Nezatvoreni navodnici
       ========================= */
    printf("\n--- Unclosed quote test ---\n");

    const char *bad_quote = "\"aaa,bbb\n";
    csv_parse(&parser, bad_quote, strlen(bad_quote),
              cb_field, cb_row, NULL);

    /* =========================
       3. Escaped quotes
       ========================= */
    printf("\n--- Escaped quote test ---\n");

    const char *escaped = "\"aaa\"\"bbb\",ccc\n";
    csv_parse(&parser, escaped, strlen(escaped),
              cb_field, cb_row, NULL);

    /* =========================
       4. NULL bajt unutar inputa
       ========================= */
    printf("\n--- Embedded NULL test ---\n");

    char null_input[] = { 'a','b','\0','c',',','d','\n' };

    csv_parse(&parser, null_input, sizeof(null_input),
              cb_field, cb_row, NULL);

    /* =========================
       5. Random fuzz test (utisan)
       ========================= */
    printf("\n--- Fuzz test (silent) ---\n");

    verbose = 0;  // isključujemo ispis

    for (int i = 0; i < 10000; i++) {
        char fuzz[128];
        for (int j = 0; j < 128; j++)
            fuzz[j] = rand() % 256;

        csv_parse(&parser, fuzz, sizeof(fuzz),
                  cb_field, cb_row, NULL);
    }

    verbose = 1;  // vraćamo ispis

    csv_free(&parser);

    printf("\nTest završen.\n");
    return EXIT_SUCCESS;
}
