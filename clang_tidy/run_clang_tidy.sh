#!/bin/bash

# Folder za rezultate
RESULT_DIR="./clang_tidy_results"

# Kreiranje foldera ako ne postoji
mkdir -p "$RESULT_DIR"

# Pokretanje Clang-Tidy analize
clang-tidy ../libcsv/libcsv.c -- -I../libcsv > "$RESULT_DIR/clang_tidy.txt"

echo "Clang-Tidy analiza završena. Rezultati su u $RESULT_DIR/clang_tidy.txt"
