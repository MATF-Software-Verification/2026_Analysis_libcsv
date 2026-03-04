#!/bin/bash
# Skripta za pokretanje AFL++ fuzzing testa za libcsv
# Uključuje export opcije za izbegavanje problema sa core dumps i CPU scaling

# Folder sa početnim fajlovima (seedovima)
SEEDS_DIR="seeds"

# Folder gde će AFL++ čuvati rezultate
OUTPUT_DIR="findings"

# Postavljanje okruženja da ignoriše upozorenja o core dumps
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1

# Preskačemo proveru CPU scaling governor-a (može usporiti fuzzing ako nije performance)
export AFL_SKIP_CPUFREQ=1

# Kompajliranje instrumentisanog izvršnog fajla
afl-clang-fast -o test_afl test_csv_afl.c libcsv.c

# Pokretanje fuzzera
afl-fuzz -i "$SEEDS_DIR" -o "$OUTPUT_DIR" -- ./test_afl @@
