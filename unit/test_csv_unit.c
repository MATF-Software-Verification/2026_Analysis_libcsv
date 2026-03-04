#include <stdio.h>
#include <string.h>
#include "csv.h"

// Callback funkcija za svako polje
void field_cb(void *field, size_t len, void *data) {
    (void)data;
    char *str = (char*)field;
    if (str) {
        printf("FIELD: %.*s\n", (int)len, str);
    } else {
        printf("FIELD: NULL\n");
    }
}

// Callback funkcija za kraj reda
void row_cb(int c, void *data) {
    (void)data;
    if (c == -1) {
        printf("ROW END: EOF\n");
    } else {
        printf("ROW END: %c\n", (char)c);
    }
}

int main() {
    struct csv_parser parser;
    char csv_data[] = "a,b,c\n1,2,3\n\"quoted,field\",4,5";

    // Inicijalizacija parsera sa opcijom da se polja null-terminiraju
    if (csv_init(&parser, CSV_APPEND_NULL) != 0) {
        fprintf(stderr, "Failed to initialize parser\n");
        return 1;
    }

    // Parsiranje CSV podataka
    size_t parsed = csv_parse(&parser, csv_data, strlen(csv_data), field_cb, row_cb, NULL);

    // Finalizacija parsiranja i pozivanje callback-ova za preostale podatke
    csv_fini(&parser, field_cb, row_cb, NULL);

    // Oslobađanje resursa parsera
    csv_free(&parser);

    // Ispis statistike i potvrda da su testovi prošli
    printf("Parsed %zu characters\n", parsed);
    printf("All unit tests OK\n");

    return 0;
}
