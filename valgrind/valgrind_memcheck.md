# Libcsv Valgrind Memcheck test

Ovaj izveštaj dokumentuje testiranje `libcsv` parsera korišćenjem **Valgrind Memcheck** alata.

Cilj:
- Otkrivanje curenja memorije (memory leaks) – da li postoji memorija koja je alocirana, ali nije oslobođena
- Detekcija neinicijalizovanih promenljivih koje se koriste prilikom izvršavanja programa
- Pronalaženje pristupa van dodeljenog opsega memorije (out-of-bounds read/write).
- Provera ispravnog oslobađanja memorije kod dinamički alociranih resursa u programu.

---

## 1. Kompajliranje

`gcc -g test_csv_valgrind.c libcsv.c -o test_valgrind`

### 1.1 Opis opcija

 Opcija | Opis |
|--------|------|
| `-g` | Dodaje debug simbole u izvršni fajl, omogućavajući precizne izveštaje sa linijama koda kada ASan detektuje grešku. |



## 2. Pokretanje testa sa Valgrind

`valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./test_valgrind`

### 2.1 Opis opcija

 Opcija | Opis |
|--------|------|
| `--leak-check=full` | Detektuje sve curenje memorije i prikazuje detaljne informacije o alokacijama koje nisu oslobođene. |
| `--show-leak-kinds=all` | Prikazuje sve tipove curenja memorije: definite, indirect i possible leaks. |
| `--error-exitcode=1` | Postavlja izlazni kod programa na 1 ako Valgrind detektuje bilo kakvu grešku u memoriji. |


## 3. Modifikacije u testu za Valgrind

Za Valgrind je test modifikovan da:
- Ne ispisuje ogromna polja (long field test) kako izlaz ne bi bio nečitljiv.
- Fuzz test radi u utišanom režimu (verbose = 0).
- Ostale test sekcije ostaju iste (nezatvoreni navodnici, escaped quotes, embedded NULL).

## 4. Primer izlaza

==978761== Memcheck, a memory error detector
==978761== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==978761== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==978761== Command: ./test_valgrind
==978761== 
==978761== HEAP SUMMARY:
==978761==     in use at exit: 0 bytes in 0 blocks
==978761==   total heap usage: 783 allocs, 783 frees, 39,287,584 bytes allocated
==978761== 
==978761== All heap blocks were freed -- no leaks are possible
==978761== 
==978761== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

## 5. Zaključak:

- Test nije detektovao curenje memorije
- Sve alokacije su ispravno oslobođene
- Test je validan i ne zahteva dodatnu proveru pokrivenosti koda (gcov/lcov) za osnovnu verifikaciju
