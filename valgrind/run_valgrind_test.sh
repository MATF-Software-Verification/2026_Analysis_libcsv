#!/bin/bash

gcc -g ../unit/test_csv_unit.c ../libcsv/libcsv.c -I../libcsv -o test_valgrind

valgrind --leak-check=full \
         --show-leak-kinds=all \
         ./test_valgrind > /dev/null 2> valgrind_report.txt
