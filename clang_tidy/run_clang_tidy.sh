#!/bin/bash

clang-tidy ../libcsv/libcsv.c -- -I../libcsv > clang_tidy.txt 2>&1

# Izdvajanje upozorenja sa okolnim linijama
grep -C 2 "warning" clang_tidy.txt > clang_warnings_summary.txt

echo "Clang-Tidy analiza završena."
echo "Kompletan izveštaj: clang_tidy.txt"
echo "Sažetak upozorenja: clang_warnings_summary.txt"
