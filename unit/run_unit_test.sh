#!/bin/bash

# Fajlovi za logove
COMPILE_LOG="compile_output.txt"
TEST_LOG="unit_test_output.txt"
GCOV_LOG="gcov_output.txt"
COVERAGE_SUMMARY="coverage_summary.txt"

# Čišćenje starih coverage fajlova
rm -f *.gcda *.gcno *.gcov unit *.txt

# Kompajliranje sa coverage opcijama
gcc -Wall -Wextra -fprofile-arcs -ftest-coverage \
    libcsv.c test_csv_unit.c -o unit \
    >> "$COMPILE_LOG" 2>&1

# Provera uspešnosti kompajliranja
if [ $? -ne 0 ]; then
    echo "Kompajliranje NIJE uspelo." >> "$COMPILE_LOG"
    echo "Greška pri kompajliranju. Pogledaj $COMPILE_LOG"
    exit 1
else
    echo "Kompajliranje uspešno završeno." >> "$COMPILE_LOG"
fi

# Pokretanje unit testa
echo "=== UNIT TEST ===" > "$TEST_LOG"

./unit >> "$TEST_LOG" 2>&1

# Pokretanje gcov analize za libcsv.c
echo "=== GCOV ANALIZA libcsv.c ===" > "$GCOV_LOG"
gcov -b -c unit-libcsv.gcno >> "$GCOV_LOG" 2>&1

# Pokretanje gcov analize za test integracije
echo "=== GCOV ANALIZA test_csv_integration.c ===" >> "$GCOV_LOG"
gcov -b -c unit-test_csv_unit.gcno >> "$GCOV_LOG" 2>&1

# Izdvajanje samo procenta pokrivenosti
grep "Lines executed" "$GCOV_LOG" > "$COVERAGE_SUMMARY"

echo "Skripta uspešno završena."
echo "Generisani fajlovi:"
echo "- $COMPILE_LOG"
echo "- $TEST_LOG"
echo "- $GCOV_LOG"
echo "- $COVERAGE_SUMMARY"
