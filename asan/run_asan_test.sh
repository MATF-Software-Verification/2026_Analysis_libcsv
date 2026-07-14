#!/bin/bash

LIBCSV_DIR="../libcsv"

gcc -fsanitize=address -g \
    ../unit/test_csv_unit.c \
    "$LIBCSV_DIR/libcsv.c" \
    -I"$LIBCSV_DIR" \
    -o test_asan

if [ $? -ne 0 ]; then
    echo "Greška pri kompajliranju!"
    exit 1
fi

./test_asan > /dev/null 2> asan_report.txt


if [ -s asan_report.txt ]; then
    echo "ASan je pronašao probleme."
else
    echo "ASan nije pronašao memorijske greške." > asan_report.txt
fi


echo "ASan analiza završena. Rezultati su u asan_report.txt"
