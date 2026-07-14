#!/bin/bash

# Fajlovi za logove
COMPILE_LOG="compile_output.txt"
TEST_LOG="unit_test_output.txt"
GCOV_LOG="gcov_output.txt"

# Čišćenje starih coverage fajlova
rm -f *.gcda *.gcno *.gcov unit *.txt

# Kompajliranje sa coverage opcijama
gcc -Wall -Wextra -O0 -g \
    -fprofile-arcs -ftest-coverage \
    -I../libcsv \
    ../libcsv/libcsv.c \
    test_csv_unit.c \
    -o unit \
    >> "$COMPILE_LOG" 2>&1

# Provera uspešnosti kompajliranja
if [ $? -ne 0 ]; then
    echo "Kompajliranje NIJE uspelo." >> "$COMPILE_LOG"
    echo "Greška pri kompajliranju. Pogledaj $COMPILE_LOG"
    exit 1
fi

echo "Kompajliranje uspešno završeno." >> "$COMPILE_LOG"

# Pokretanje jedinicnog testa
echo " UNIT TEST " > "$TEST_LOG"

./unit >> "$TEST_LOG" 2>&1

# GCOV analiza biblioteke
echo " GCOV ANALIZA libcsv " > "$GCOV_LOG"

gcov -b -c unit-libcsv.gcno >> "$GCOV_LOG" 2>&1

echo "Skripta uspešno završena."
echo "Generisani fajlovi:"
echo "- $COMPILE_LOG"
echo "- $TEST_LOG"
echo "- $GCOV_LOG"
