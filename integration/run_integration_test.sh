#!/bin/bash

# Fajlovi za logove
COMPILE_LOG="compile_output.txt"
TEST_LOG="integration_test_output.txt"
GCOV_LOG="gcov_output.txt"
COVERAGE_SUMMARY="coverage_summary.txt"

# CSV fajl za testiranje
CSV_FILE="test_data.csv"

# Čišćenje starih coverage fajlova i izvršnog
rm -f *.gcda *.gcno *.gcov integration *.txt

# Kompajliranje sa coverage opcijama
gcc -Wall -Wextra -fprofile-arcs -ftest-coverage \
    libcsv.c test_csv_integration.c -o integration \
    >> "$COMPILE_LOG" 2>&1

# Provera uspešnosti kompajliranja
if [ $? -ne 0 ]; then
    echo "Kompajliranje NIJE uspelo." >> "$COMPILE_LOG"
    echo "Greška pri kompajliranju. Pogledaj $COMPILE_LOG"
    exit 1
else
    echo "Kompajliranje uspešno završeno." >> "$COMPILE_LOG"
fi

# Pokretanje integracionog testa
echo "=== INTEGRATION TEST ===" > "$TEST_LOG"
./integration "$CSV_FILE" >> "$TEST_LOG" 2>&1

# Pokretanje gcov analize za libcsv.c
echo "=== GCOV ANALIZA libcsv.c ===" > "$GCOV_LOG"
gcov -b -c integration-libcsv.gcno >> "$GCOV_LOG" 2>&1

# Pokretanje gcov analize za test integracije
echo "=== GCOV ANALIZA test_csv_integration.c ===" >> "$GCOV_LOG"
gcov -b -c integration-test_csv_integration.gcno >> "$GCOV_LOG" 2>&1

# Izdvajanje procenta pokrivenosti linija u poseban fajl
grep "Lines executed" "$GCOV_LOG" > "$COVERAGE_SUMMARY"

echo "Skripta uspešno završena."
echo "Generisani fajlovi:"
echo "- $COMPILE_LOG"
echo "- $TEST_LOG"
echo "- $GCOV_LOG"
echo "- $COVERAGE_SUMMARY"
