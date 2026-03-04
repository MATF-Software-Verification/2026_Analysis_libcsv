#!/bin/bash

# Folder submodula libcsv (relativno od foldera asan)
LIBCSV_DIR="../libcsv"

# Naziv izvršnog fajla
OUT=test

# Fajlovi izvora
SRC="test_csv_unit.c $LIBCSV_DIR/libcsv.c"

# Fajl za čuvanje izveštaja
REPORT="csv_test_report.txt"

# Kompajliranje sa ASan i debug simbolima
gcc -fsanitize=address -g $SRC -I"$LIBCSV_DIR" -o $OUT

# Provera da li je kompajliranje uspelo
if [ $? -ne 0 ]; then
    echo "Greška pri kompajliranju!"
    exit 1
fi

# Pokretanje testa i čuvanje izlaza u fajl
echo "=== Pokretanje testa ==="
./$OUT | tee $REPORT

echo "=== Izveštaj sačuvan u $REPORT ==="
