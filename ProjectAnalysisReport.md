# Analiza biblioteke libcsv

## Clang-Tidy

Za statičku analizu izvornog koda korišćen je **Clang-Tidy**. Analiza je prijavila ukupno **16 upozorenja**, uglavnom tipa `clang-analyzer-core.NullDereference`.

Upozorenja se odnose na pristup polju `entry_buf` preko makroa `SUBMIT_FIELD` i `SUBMIT_CHAR`. Clang-Tidy na određenim analiziranim putanjama zaključuje da `entry_buf` može imati vrednost `NULL` u trenutku kada se nad njim vrši indeksiranje.

Ova upozorenja predstavljaju potencijalne probleme detektovane statičkom analizom i zahtevaju dodatnu proveru u kontekstu stvarnog toka programa.

---

## Unit testovi i GCOV

Za proveru funkcionalnosti biblioteke napravljeno je **18 unit testova** koji pokrivaju različite funkcionalnosti CSV parsera, uključujući standardne CSV slučajeve, prazna polja, quoted vrednosti, custom delimiter, velike field-ove, strict mode, pisanje CSV-a i getter funkcije.

Svi testovi su uspešno prošli:

**18/18 testova uspešno.**

Nakon toga je pomoću alata **GCOV** analizirana pokrivenost koda. Testovi su ostvarili:

* **67.18% line coverage**
* **67.60% branch coverage**

Rezultati pokazuju da su testovi pokrili značajan deo implementacije, ali da određeni delovi koda i grane ostaju nepokriveni i mogu zahtevati dodatne testove.

Preostali nepokriveni delovi uglavnom predstavljaju alternativne funkcionalnosti i error paths, kao što su `NULL` argumenti, neuspešna alokacija memorije, custom memory callback funkcije, `csv_fwrite` i pojedine grane parsera.

Dodavanje dodatnih standardnih CSV primera nije značajno povećavalo coverage, jer su takvi testovi prolazili kroz već pokrivene grane.

---

## Valgrind

Unit test program kompajliran je sa debug informacijama i pokrenut pod **Valgrind Memcheck** alatom. Uključena je detaljna provera curenja memorije pomoću opcija `--leak-check=full` i `--show-leak-kinds=all`.

Tokom izvršavanja 18 unit testova izvršeno je:

* **19 alokacija memorije**
* **19 oslobađanja memorije**
* **0 bajtova** zauzete memorije na kraju programa
* **0 detektovanih grešaka**

Valgrind je takođe prijavio da su svi alocirani heap blokovi oslobođeni, odnosno da **nije pronađeno curenje memorije**.

Rezultat pokazuje da tokom izvršavanja korišćenih testova nisu detektovane memorijske greške niti curenja memorije.

---

## Flawfinder

Za statičku analizu potencijalnih sigurnosnih problema korišćen je **Flawfinder**.

Analiziran je fajl `libcsv/libcsv.c`, koji sadrži **545 linija**, odnosno **405 SLOC** (*Source Lines of Code*). Flawfinder koristi skup od **222 pravila** za prepoznavanje potencijalno rizičnih konstrukcija u C/C++ kodu.

Analizom nije pronađen nijedan potencijalni sigurnosni problem:

**Hits = 0**

Dakle, za glavnu implementaciju `libcsv.c` nisu prijavljeni potencijalno rizični konstrukti ni na jednom nivou rizika.

---

## Lizard

Za analizu kompleksnosti izvornog koda korišćen je alat **Lizard**. Lizard je statički analizator koji meri kompleksnost i strukturu funkcija bez izvršavanja programa.

Analizom su obuhvaćene funkcije iz fajla `libcsv/libcsv.c`. Ukupno je analizirano **23 funkcije**, sa **374 NLOC** (*Non-Comment Lines of Code*). Prosečna funkcija ima **14.9 NLOC**, dok prosečna ciklomatska kompleksnost iznosi **5.5**.

Lizard je prijavio jedno upozorenje, za funkciju `csv_parse()`:

| Funkcija      | NLOC |    CCN | PARAM | Length |
| ------------- | ---: | -----: | ----: | -----: |
| `csv_parse()` |  132 | **49** |     4 |    149 |

Funkcija `csv_parse()` je jedina funkcija koja je prešla neki od podrazumevanih pragova za upozorenje. Konkretno, njen **CCN iznosi 49**, dok je prag za upozorenje `CCN > 15`.

Visoka ciklomatska kompleksnost ukazuje na veliki broj grananja i mogućih puteva izvršavanja, zbog čega je funkcija složenija za razumevanje, testiranje i održavanje. Samo upozorenje ne znači da funkcija sadrži grešku.

Za analizu je generisan i **HTML izveštaj**, koji omogućava pregled rezultata u preglednijem grafičkom obliku.

---

## AFL++

Za dinamičko **fuzz testiranje** korišćen je alat **AFL++**. Cilj fuzz testiranja je automatsko generisanje i mutiranje velikog broja ulaza kako bi se pronašli ulazi koji mogu izazvati neočekivano ponašanje, rušenje ili zaglavljivanje programa.

Za potrebe fuzzinga napravljen je test harness `test_csv_afl.c`, koji učitava sadržaj ulaznog fajla i prosleđuje ga funkciji `csv_parse()` iz biblioteke `libcsv`.

AFL++ je kompajlirao program pomoću `afl-clang-fast`, čime je omogućena instrumentacija programa i praćenje izvršnih putanja. Kao početni ulazi korišćeni su CSV primeri iz direktorijuma `seeds`, dok je AFL++ rezultate čuvao u direktorijumu `findings`.

Fuzz test je pokrenut naredbom:

```bash
afl-fuzz -V 60 -i seeds -o findings -- ./test_afl @@
```


Tokom testiranja program je izvršen **565.024 puta**, prosečnom brzinom od približno **10.925 izvršavanja u sekundi**.

AFL++ je formirao corpus od **151 zanimljivog ulaza**, od kojih je **144 pronađeno tokom samog fuzziranja**. Ostvarena je **100% stabilnost** izvršavanja.

Pronađeno je **83 od ukupno 315 praćenih edges**, uz bitmap coverage od **26.35%**.

Najvažniji rezultat je da tokom testiranja:

* **crashes = 0**
* **hangs = 0**

Odnosno, AFL++ nije pronašao ulaz koji bi izazvao rušenje ili zaglavljivanje programa.

Dobijeni rezultati ne predstavljaju dokaz da je biblioteka potpuno bez grešaka, već pokazuju da u okviru korišćenih seedova, vremena testiranja i generisanih ulaza nisu detektovani crash-evi niti hang-ovi.

## Zaključak

Na osnovu svih rezultata može se zaključiti da biblioteka libcsv na korišćenom skupu testova pokazuje stabilno ponašanje i nema detektovanih memorijskih ili sigurnosnih problema, ali postoje delovi koda koji zahtevaju dodatnu pažnju, posebno sa aspekta kompleksnosti i upozorenja dobijenih statičkom analizom. Dobijeni rezultati ne predstavljaju dokaz da je biblioteka potpuno bez grešaka, već daju pregled njenog ponašanja i potencijalnih problematičnih mesta u okviru sprovedenih analiza.
