#!/bin/bash

TEST_LOG="unit_test_output.txt"
GCOV_LOG="gcov_output.txt"

rm -f *.gcda *.gcno *.gcov unit *.txt

gcc -Wall -Wextra -O0 -g \
    -fprofile-arcs -ftest-coverage \
    -I../libcsv \
    ../libcsv/libcsv.c \
    test_csv_unit.c \
    -o unit 

if [ $? -ne 0 ]; then
    echo "Kompajliranje NIJE uspelo." >> "$COMPILE_LOG"
    echo "Greška pri kompajliranju. Pogledaj $COMPILE_LOG"
    exit 1
fi

echo "Kompajliranje uspešno završeno."
echo " UNIT TEST " > "$TEST_LOG"

./unit >> "$TEST_LOG" 2>&1

echo " GCOV ANALIZA libcsv " > "$GCOV_LOG"

gcov -b -c unit-libcsv.gcno >> "$GCOV_LOG" 2>&1

echo "Skripta uspešno završena."
echo "Generisani fajlovi:"
echo "- $TEST_LOG"
echo "- $GCOV_LOG"
