#include "csv.h"      
#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>    

// Callback za svako polje CSV-a (ovde ništa ne radimo, samo stub)
void cb_field(void *field, size_t len, void *data) {
    // field -> pokazivač na podatke polja
    // len   -> dužina polja
    // data  -> dodatni parametar (može biti NULL)
}

// Callback za kraj reda (ovde ništa ne radimo, samo stub)
void cb_row(int c, void *data) {
    // c     -> broj polja u redu
    // data  -> dodatni parametar (može biti NULL)
}

int main(int argc, char **argv) {
    struct csv_parser parser;

    // ============================
    // Inicijalizacija parsera
    // ============================
    // 0 -> bez posebnih flagova
    csv_init(&parser, 0);

    // ============================
    // Učitavanje input fajla
    // ============================
    if (argc < 2) {
        // Ako nije prosleđen fajl kao argument
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");  // Otvaramo fajl u binarnom režimu
    if (!f) return 1;                // Proveravamo da li je otvoren

    // Odredimo veličinu fajla
    fseek(f, 0, SEEK_END);           // Pomeramo pokazivac na kraj fajla
    long size = ftell(f);             // Vraca broj bajtova od početka
    fseek(f, 0, SEEK_SET);           // Vracamo pokazivač na početak

    // Alociramo buffer u memoriji
    char *buffer = malloc(size);
    if (!buffer) {
        fclose(f);
        return 1;
    }

    // Čitamo sadržaj fajla u buffer
    fread(buffer, 1, size, f);
    fclose(f);  // Zatvaramo fajl odmah nakon čitanja

    // ============================
    // Pokretanje parsera na buffer-u
    // ============================
    csv_parse(&parser, buffer, size, cb_field, cb_row, NULL);

    // ============================
    // Oslobađanje resursa
    // ============================
    free(buffer);       // Oslobađamo alocirani buffer
    csv_free(&parser);  // Oslobađamo interne strukture parsera

    return 0;           
}
