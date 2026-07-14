#!/bin/bash

LIBCSV_DIR="../libcsv"

# Kompajliranje unit testa
gcc -g ../unit/test_csv_unit.c "$LIBCSV_DIR/libcsv.c" -I"$LIBCSV_DIR" -o test_valgrind

# Pokretanje Valgrind-a:
# stdout programa se ignoriše, Valgrind ostaje vidljiv
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --error-exitcode=1 \
         ./test_valgrind > /dev/null 2> valgrind_report.txt
