#!/bin/bash
# Skripta za kompajliranje i pokretanje CSV testa sa Valgrind Memcheck

# Putanja do libcsv submodula (relativno od foldera valgrind)
LIBCSV_DIR="../libcsv"

# Kompajliranje
gcc -g test_csv_valgrind.c "$LIBCSV_DIR/libcsv.c" -I"$LIBCSV_DIR" -o test_valgrind

# Pokretanje sa Valgrind Memcheck
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./test_valgrind
