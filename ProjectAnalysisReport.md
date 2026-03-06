# Analiza unit testiranja i pokrivenosti koda

## 1. Cilj analize

Cilj ove analize je:

- Verifikacija funkcionalnosti CSV parsera korišćenjem unit testova  
- Provera ispravnosti rada callback mehanizma  
- Merenje pokrivenosti koda pomoću `gcov` alata  
- Procena kvaliteta testova na osnovu ostvarenog procenta pokrivenosti  

---

## 2. Objašnjenje korišćenih opcija

### 2.1 GCC opcije za kompajliranje sa pokrivenošću koda

| Opcija | Opis |
|--------|------|
| `-Wall` | Aktivira upozorenja za sve uobičajene probleme u kodu |
| `-Wextra` | Aktivira dodatna upozorenja koja nisu obuhvaćena sa `-Wall` |
| `-fprofile-arcs` | Omogućava instrumentaciju za GCOV – beleži tok izvođenja programa |
| `-ftest-coverage` | Generiše dodatne fajlove (`.gcno`, `.gcda`) za pokrivenost koda |

### 2.2 GCOV opcije

| Opcija | Opis |
|--------|------|
| `-b` | Prikazuje branch coverage tj. pokrivenost logičkih grana (`if`, `switch`) |
| `-c` | Prikazuje call coverage tj. koliko je funkcijskih poziva izvršeno |

---

## 3. Pokretanje unit testa

Program je kompajliran sa uključenim opcijama za pokrivenost:

`gcc -Wall -Wextra -fprofile-arcs -ftest-coverage libcsv.c test_csv_unit.c -o unit`

---

## 4. Dobijeni izlaz

FIELD: a
FIELD: b
FIELD: c
ROW END: 

FIELD: 1
FIELD: 2
FIELD: 3
ROW END: 

FIELD: quoted,field
FIELD: 4
FIELD: 5
ROW END: EOF
Parsed 30 characters
All unit tests OK

### 4.1 Tumačenje rezultata:

- Parser je uspešno obradio 3 reda CSV podataka
- Ispravno su prepoznata obična polja, numerička polja i quoted polja sa zarezom
- Callback funkcije za polja i kraj reda su pozvane odgovarajući broj puta
- Program je završio bez greške

- Zaključak: osnovna funkcionalnost parsera radi ispravno
    
---

## 5. GCOV analiza pokrivenosti

Pokretanje gcov alata:

`gcov -b -c unit-test_csv_unit.gcno`

### 5.1 Rezultati za test_csv_unit.c:

Lines executed:86.36% of 22
Branches executed:100.00% of 6
Taken at least once:66.67% of 6
Calls executed:81.82% of 11

---

## 6. Procena kvaliteta testova

Testovi pokrivaju:

- Osnovno parsiranje CSV podataka
- Obradu quoted polja
- Obradu više redova
- Obradu završetka fajla (EOF)
- Rad callback funkcija

Testovi ne pokrivaju:

- Grešku pri inicijalizaciji parsera
- NULL vrednosti polja
- Ekstremne ili neispravne ulaze

---

## 7. Zaključak

- CSV parser funkcioniše ispravno za tipične ulazne podatke
- Ostvarena je visoka pokrivenost linija (86.36%)
- Sve grane su analizirane, ali nisu sve izvršene tokom testova
- Testovi pokrivaju glavni tok izvršavanja programa, ali za potpunu validaciju bi bilo potrebno dodati testove koji simuliraju greške i specijalne slučajeve

---

## 8. Preporuka

- Dodati testove za negativne slučajeve i greške pri inicijalizaciji parsera.

# Analiza integracionog testiranja i pokrivenosti koda

## 1. Cilj analize

Cilj ove analize je:

- Proveriti funkcionalnost CSV parsera kroz integracione testove 
- Validacija obrade različitih tipova polja i redova 
- Merenje pokrivenosti koda pomoću `gcov` alata 
- Procena kvaliteta integracionih testova na osnovu procenta pokrivenosti 

---

## 2. Objašnjenje korišćenih opcija

### 2.1 GCC opcije za kompajliranje sa pokrivenošću koda

| Opcija | Opis |
|--------|------|
| `-Wall` | Aktivira upozorenja za sve uobičajene probleme u kodu. |
| `-Wextra` | Aktivira dodatna upozorenja koja nisu obuhvaćena sa `-Wall`. |
| `-fprofile-arcs` | Omogućava instrumentaciju za GCOV – beleži tok izvršenja programa. |
| `-ftest-coverage` | Generiše fajlove (`.gcno`, `.gcda`) za analizu pokrivenosti koda. |

### 2.2 GCOV opcije

| Opcija | Opis |
|--------|------|
| `-b` | Prikazuje branch coverage tj. pokrivenost logičkih grana (`if`, `switch`). |
| `-c` | Prikazuje call coverage tj. koliko je funkcijskih poziva izvršeno. |

---

## 3. Pokretanje integracionog testa

Kompajliranje i pokretanje:

`gcc -Wall -Wextra -fprofile-arcs -ftest-coverage libcsv.c test_csv_integration.c -o integration`

---

## 4. Dobijeni izlaz

FIELD: ime
FIELD: prezime
FIELD: godine
ROW END
FIELD: Ana
FIELD: Petrović
FIELD: 25
ROW END
FIELD: Ivan, Jr.
FIELD: Marković
FIELD: 30
ROW END
Parsed 3 data rows (excluding header)
Integration test passed!

### 4.1 Tumačenje rezultata:

- Parser je uspešno obradio 3 reda CSV podataka (uključujući quoted polja sa zarezom)
- Callback funkcije za polja i kraj reda su pozvane odgovarajući broj puta
- Program je završio bez greške, što potvrđuje da osnovna integracija funkcioniše

---

## 5. GCOV analiza pokrivenosti

Pokretanje gcov alata:

`gcov -b -c integration-libcsv.gcno`

### 5.1 Rezultati za libcsv.c:

Lines executed:38.93% of 262
Branches executed:39.56% of 321
Taken at least once:22.43% of 321
Calls executed:17.95% of 39

Zaključak: 
- Integracioni test uspešno pokriva osnovni tok CSV parsiranja, uključujući multiple redove i quoted polja, ali za potpunu pokrivenost potrebno je dodati testove za negativne i ekstremne ulaze.

---

## 6. Procena kvaliteta testova

Integracioni testovi pokrivaju:

- Parsiranje CSV podataka sa zaglavljem i više redova
- Obradu quoted polja i polja sa zarezom
- Funkcionalnost callback mehanizma
- Kraj fajla (EOF)

Integracioni testovi ne pokrivaju:

- Greške pri inicijalizaciji parsera
- NULL vrednosti polja
- Ekstremne ili neispravne ulaze

---

## 7. Zaključak

- CSV parser funkcioniše ispravno za tipične ulazne podatke.
- Ostvarena je visoka pokrivenost linija i funkcija, ali nisu sve grane izvršene.
- Testovi pokrivaju glavni tok izvršavanja, ali nisu obuhvatili negativne scenarije i edge case-ove.

---

## 8. Preporuka

- Dodati integracione testove za negativne slučajeve i greške pri inicijalizaciji parsera.

# Analiza projekta sa Clang-Tidy

## 1. Cilj analize

Clang-Tidy je statički analizator koda za C/C++ projekte koji detektuje moguće runtime greške, probleme sa pokazivačima, potencijalne null dereference, kao i druge sigurnosne i stilističke probleme. Cilj korišćenja Clang-Tidy u ovom projektu bio je da se:

- Otkrivaju runtime problemi i potencijalne null pointer dereference pre izvršavanja koda
- Poboljša stabilnost i sigurnost biblioteke `libcsv`
- Identifikuju problematični delovi koda vezani za upravljanje memorijom i kontrolu toka programa
- Olakša refaktorisanje i održavanje koda

---

## 2. Objašnjenje korišćenih opcija

| Opcija | Opis |
|--------|------|
| `clang-tidy libcsv.c csv.h` | Pokreće analizu na fajlovima `libcsv.c` i `csv.h` |
| `--` | Separator između Clang-Tidy opcija i opcija kompajlera |
| `-I.` | Uključuje trenutni direktorijum kao putanju za header fajlove (include path) |
| `> clang_tidy_results/clang_tidy.txt` | Preusmerava kompletan izlaz analize u fajl unutar direktorijuma za rezultate |

---

## 3. Rezultati analize

- Ukupan broj upozorenja: 32  
  - `libcsv.c`: 16 upozorenja  
  - `csv.h`: 16 upozorenja  

- Tipovi uočenih problema:
  - Null pointer dereference (`clang-analyzer-core.NullDereference`)
  - Pretpostavke o validnosti pokazivača korišćenjem `assert` i `if` provera
  - Problemi u kontroli toka programa kroz `if`, `switch` i `while`
  - Potencijalna neinicijalizovana polja i pristupi memoriji pre alokacije

- Izlaz analize je sačuvan u `clang_tidy_results/clang_tidy.txt`.

---

## 4. Zaključak

Analiza sa Clang-Tidy pokazuje da kod projekta:

- Sadrži upozorenja koja ukazuju na potencijalne null pointer dereference, što može izazvati runtime greške 
- Većina upozorenja se odnosi na neophodnost dodatnih provera validnosti pokazivača i sigurnije upravljanje memorijom 
- Refaktorisanje i implementacija dodatnih provera može značajno poboljšati stabilnost biblioteke `libcsv` 
- Preporučuje se kombinovanje statičke analize sa unit testovima kako bi se osigurala potpuna pokrivenost i kvalitet koda  

Rezultati analize su sačuvani u tekstualnom fajlu i mogu se dalje procesirati za generisanje PDF ili HTML izveštaja

# Dinamička analiza – AddressSanitizer (ASan)

## 1. Podešavanje okruženja i kompilacija

Biblioteka **libcsv** testirana je korišćenjem AddressSanitizer alata ugrađenog u GCC kompajler.

Kompilacija je izvršena sledećom komandom:

`gcc -fsanitize=address -g test_csv_unit.c libcsv.c -o test`

| Opcija | Opis |
|--------|------|
| `-fsanitize=address` | Aktivira AddressSanitizer (ASan) za detekciju memorijskih grešaka tokom izvršavanja programa (heap/stack overflow, use-after-free, double free, invalid read/write) |
| `-g` | Dodaje debug simbole u izvršni fajl, omogućavajući precizne izveštaje sa linijama koda kada ASan detektuje grešku |
| `test_csv_unit.c` | Test program koji pokreće različite test scenarije biblioteke libcsv |
| `libcsv.c` | Izvorni kod biblioteke koja se analizira |
| `-o test` | Naziv generisanog izvršnog fajla |

## Test 1 - Dugačko polje (realloc stres test)

Ulaz je sadržao veoma dugo polje (više hiljada karaktera)

Cilj:
- Proveriti stabilnost dinamičkog proširenja bafera
- Detektovati potencijalni buffer overflow

Rezultat:
- Parser je uspešno obradio ulaz
- Nije došlo do rušenja programa
- AdressSanitizer nije prijavio memorijske greške

Zaključak:
- Dinamičko upravljanje memorijom (realloc) funkcioniše ispravno u testiranom scenariju

## Test 2 - Nezatvoreni navodnici

Ulaz:
`"abc,def\n`

Cilj: 
- Testirati ponašanje parsera pri sintaksnoj grešci

Rezultat:
- Parser nije emitovao završno polje
- Nije došlo do crash-a
- ASan nije prijavio greške

Zaključak:
- Biblioteka stavilno obrađuje nedovršene quoted sekvence bez memorijskih problema

## Test 3 - Escaped navodnici

Ulaz:

`"aaa""bbb",ccc\n`

Cilja:
- Testirrati obradu escaped navodnika ("")
- Proveriti stabilnost pri obradi quoted polja

Rezultati:
- Polje je pravilno parsirano
- Nije detektovan buffer overflow
- ASan nije prijavio greške

Zaključak:
- Implementacija quoted polja i escape maehanizma funkcioniše ispravno u testiranom okruženju

## Test 4 - Ugrađeni NULL bajt

Ulaz je sadržao `\0` unutar podataka

Cilj:
- Proveriti da li biblioteka zavisi od `strlen`
- Testirati otpornost na binarne podatke

Rezultat:
- Nije došlo do rušenja programa
- Nije prijavljen invalid read/write operacija
- ASan nije detektovao greške

Zaključak:
- Biblioteka može da obradi podatke koji sadrže NULL bajt bez memorijske korupcije

## Test 5 - Jednostavan fuzz test

Generisan je niz nasumičnih bajtova i prosleđen parseru

Cilj:
- Izazvati neočekivana stanja parsera
- Testirati robusnost implementacije

Rezultat:
- Program se nije srušio
- Nije detektovan heap overflow
- Nije detektovan use-after-free
- Nije prijavljen double free

Zaključak:
- Parser pokazuje dobru stabilnost na slučajno generisane ulaze u okviru sprovedenog testiranja

---

## Opšti zaključak:
Na osnovu sprovedenih testova može se zaključiti:

- Nisu detektovane memorijske greške tipa heap overflow, stack overflow, use-after-free ili double free
- Dinamičko upravljanje memorijom funkcioniše stabilno u testiranim scenarijima
- Biblioteka pokazuje otpornost na granične i nekorektne ulaze

# Valgrind Memcheck test

Ovaj izveštaj dokumentuje testiranje `libcsv` parsera korišćenjem **Valgrind Memcheck** alata.

Cilj:
- Otkrivanje curenja memorije - da li postoji memorija koja je alocirana, ali nije oslobođena
- Detekcija neinicijalizovanih promenljivih koje se koriste prilikom izvršavanja programa
- Pronalaženje pristupa van dodeljenog opsega memorije
- Provera ispravnog oslobađanja memorije kod dinamički alociranih resursa u programu

---

## 1. Kompajliranje

`gcc -g test_csv_valgrind.c libcsv.c -o test_valgrind`

### 1.1 Opis opcija

 Opcija | Opis |
|--------|------|
| `-g` | Dodaje debug simbole u izvršni fajl, omogućavajući precizne izveštaje sa linijama koda kada ASan detektuje grešku |



## 2. Pokretanje testa sa Valgrind

`valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./test_valgrind`

### 2.1 Opis opcija

 Opcija | Opis |
|--------|------|
| `--leak-check=full` | Detektuje sve curenje memorije i prikazuje detaljne informacije o alokacijama koje nisu oslobođene |
| `--show-leak-kinds=all` | Prikazuje sve tipove curenja memorije: definite, indirect i possible leaks |
| `--error-exitcode=1` | Postavlja izlazni kod programa na 1 ako Valgrind detektuje bilo kakvu grešku u memoriji |


## 3. Modifikacije u testu za Valgrind

Za Valgrind je test modifikovan da:
- Ne ispisuje ogromna polja (long field test) kako izlaz ne bi bio nečitljiv
- Fuzz test radi u utišanom režimu (verbose = 0)
- Ostale test sekcije ostaju iste (nezatvoreni navodnici, escaped quotes, embedded NULL)

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

# AFL++ (Fuzzing)

## 1. Cilj analize:
- Automatski generisati veliki broj različitih CSV inputa
- Pronaći potencijalne bugove, crash-eve ili segfault-ove parsera
- Testirati robusnost parsera u edge-case situacijama, uključujući: duge stringove, prazna polja, specijalne karaktere i escape sekvence
- Potvrditi stabilnost i sigurnost biblioteke libcsv

## 2. Kompilacija i pokretanje AFL++

`afl-clang-fast -o test_afl test_csv_afl.c libcsv.c`

| Opcija | Opis |
|--------|------|
| `afl-clang-fast` | LLVM-based instrumentirani kompajler za AFL++ koji omogućava praćenje izvršavanja programa i generisanje fuzz inputa |

| `export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1` | Onemogućava probleme sa core dump podešavanjima sistema, tako da AFL++ ne promaši kratke procese ili crash-eve |

| `export AFL_SKIP_CPUFREQ=1` | Preskače proveru CPU frequency scaling-a i moguće performansne upozorenja |


`afl-fuzz -i seeds -o findings -- ./test_afl @@`

| Opcija | Opis |
|--------|------|
| `afl-fuzz` | Glavni AFL++ fuzzer alat koji izvršava testirani program sa generisanim inputom |
| `-i seeds` | Folder sa početnim seed fajlovima (ulazni primeri CSV-a) |
| `-o findings` | Folder u koji se čuvaju rezultati fuzzinga: queue/, crashes/, hangs/ |
| `-- ./test_afl @@` | Komanda koja se izvršava za svaki fuzz input; @@ se zamenjuje sa fajlom generisanim od strane AFL++ |

## 3. Seed fajlovi korišćeni za fuzzing:

**test1.csv**
ime,prezime,godine
Ana,Petrović,25
Marko,Jovanović,30
"Ivan, Jr.",Ivić,22

**test2.csv**
ime,prezime,godine
Ana,,25
,Marko,30
"Ivan, Jr.",,22
"Milica; M.",Popović,31

**test3.csv**
ime,prezime,godine
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA,Petrović,25
Marko,BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB,30
Ivan,Ivić,CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC,22

**test4.csv**
ime,prezime,godine
Ana,"Petrović\nPetrov",25
Marko,"Jovanović\tJovo",30
"Ivan, Jr.","Ivić\"Iv",22
Jovana,Đorđević,27

## 4. Rezultati

Folder **findings** sadrži:
- crashes/ - potencijalni crash fajlovi (trenutno prazno)
- hangs/ - inputi koji izazivaju dugotrajno čekanje (trenutno prazno)
- queue/ - inputi koje AFL++ koristi za dalje fuzzing iteracije
Ostali fajlovi: 
- cmdline - komanda kojom je pokrenut fuzzer
- fuzzer_setup - info o konfiguraciji fuzzera na startu
- fuzzer_stats - koliko inputa je testirano, koliko chrash-eva, koliko hangs
- plot_data - podaci koji se koriste za generisanje grafika napretka fuzz testa
- fuzz_bitmap - interni bitmap koji AFL++ koristi da prati korivene grane koda

## 5. Zaključak
- Trenutni parser ne pokazuje crash-eve ni hang-ove sa početnim seed fajlovima. 
- Fuzzing je pokrenut i može se ostaviti da radi duže za potencijalno otkrivanje ranjivosti
