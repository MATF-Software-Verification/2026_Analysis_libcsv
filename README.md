# Seminarski rad – Analiza projekta otvoftenog koda

## Autor
**Jelena Komazec, 1046/2025** 
Email: [mi251046@alas.matf.bg.ac.rs] 

---

## Opis projekta

Naziv projekta: **libcsv**
Autor: Robert Gamble
Verzija: 3.0.3
Licenca: GNU LGPL

**Opis:**

libcsv je mala i brza biblioteka za parsiranje i generisanje CSV podataka u C-u. Omogućava:
- Parsiranje CSV fajlova sa navodnicima, escape sekvencama i praznim poljima
- Pisanje CSV podataka u fajl ili memorijski buffer
- Konfigurisanje delimiter-a, navodnika i opcija parsera (strogi režim, null-terminatori, tretiranje praznih polja kao NULL)
- Praćenje grešaka i upravljanje memorijom


- Grana koja je analizirana: `master` 
- Commit heš: `b1d5212`
- Link ka repozitorijumu: [https://github.com/rgamble/libcsv](https://github.com/rgamble/libcsv) 

Cilj analize bio je da se ispita ponašanje biblioteke pri različitim ulazima i da se otkriju potencijalni problemi sa memorijom, greške u parsiranju i stabilnost programa.

---

## Korišćeni alati i uputstva za reprodukciju

### 1. Unit test i integracioni test

- Testovi pokrivaju funkcionalnosti parsiranja polja i redova, kao i interakciju više funkcija. 
- Skripte koje pokreću sve funkcionalne testove
[unit] ./run_unit_test.sh
[integration] ./run_integration_test.sh

### 2. Clang-tidy

- Analiza i preporuke za refaktor koda.
- Skripta za pokretanje: ./run_clang_tidy.sh

### 3. AdressSanitizer (ASan)

- Detekcija buffer overflow, use-after-free, neinicijalizovane memorije
- Skripta za pokretanje: ./runa_asan_test.sh

### 4. Valgrind (Memcheck)

- Praćenje memorijskih alokacija i provera curenja memorije
- Skripta za pokretanje: ./run_valgrind_test.sh

### 5. AFL++ (Fuzzing)

- Instrumentacija binarnog fajla i automatsko generisanje ulaza
- Skripta za pokretanje: ./run_afl++_test.sh

### Napomena: 
- Za reprodukciju rezultata, dovoljno je pokrenuti odgovarajuću skriptu u terminalu, pri čemu skripte automatski uključuju sve potrebne opcije i parametre. Rezultati se generišu u folderima definisanim unutar svake skripte

---

## Zaključak

- Biblioteka libcsv je stabilna i ne pokazuje curenje memorije pri standardnim ulazima.
- Unit i integracioni testovi potvrđuju ispravno parsiranje polja i redova.
- Clang-Tidy nisu otkrili kritične probleme, ali daju preporuke za stil i robustnost.
- ASan i Valgrind pokazuju da su sve alokacije i dealokacije memorije ispravne.
- AFL++ fuzzing omogućava otkrivanje edge-case ulaza i potencijalnih padova.

-Sve testirane funkcionalnosti i alati potvrđuju da je biblioteka pogodna za dalje korišćenje i integraciju u projekte koji zahtevaju rad sa CSV podacima
