# 2026_Analysis_libcsv

**Autor:** Jelena Komazec
**Broj indeksa:** 1046/2025

## libcsv

Projekat je rađen nad bibliotekom **libcsv**, koja služi za parsiranje i generisanje CSV podataka u programskom jeziku C.

**Osnovne informacije:**

* *Repozitorijum:* https://github.com/rgamble/libcsv
* *Jezik:* C
* *Verzija:* 3.0.3
* *Licenca:* GNU LGPL
* *Analizirana grana:* master
* *Heš komita:* b1d5212

---

## Korišćeni alati i tehnike

| # | Alat             | Kategorija       | Opis                                         |
| - | ---------------- | ---------------- | -------------------------------------------- |
| 1 | clang-tidy       | Statička analiza | Detekcija potencijalnih problema             |
| 2 | Unit testovi     | Testiranje       | Funkcionalno testiranje biblioteke           |
| 3 | GCOV             | Pokrivenost      | Praćenje pokrivenosti koda                   |
| 4 | Valgrind         | Memorija         | Provera memorije i curenja                   |
| 5 | Flawfinder       | Bezbednost       | Detekcija potencijalno rizičnih konstrukcija |
| 6 | Lizard           | Kompleksnost     | Analiza kompleksnosti koda                   |
| 7 | AFL++            | Fuzzing          | Automatsko generisanje i testiranje ulaza    |

## Zaključak

Analizom **libcsv** biblioteke različitim alatima provereni su funkcionalnost, pokrivenost koda, upravljanje memorijom, bezbednost, kompleksnost i ponašanje programa na različitim ulazima. Unit testovi su uspešno prošli, dok Valgrind, Flawfinder i AFL++ tokom sprovedenih analiza nisu detektovali memorijske probleme, potencijalne sigurnosne probleme, crash-eve ili hang-ove. Clang-Tidy je prijavio određena upozorenja, a Lizard je ukazao na povećanu kompleksnost funkcije `csv_parse()`. Na osnovu sprovedenih analiza, biblioteka se na korišćenom skupu testova pokazala kao stabilna, uz nekoliko mesta koja zahtevaju dodatnu pažnju.
