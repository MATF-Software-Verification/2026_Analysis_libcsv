# AFL++ (Fuzzing)

## 1. Cilj analize:
- Automatski generisati veliki broj različitih CSV inputa.
- Pronaći potencijalne bugove, crash-eve ili segfault-ove parsera.
- Testirati robusnost parsera u edge-case situacijama, uključujući: duge stringove, prazna polja, specijalne karaktere i escape sekvence
- Potvrditi stabilnost i sigurnost biblioteke libcsv.

## 2. Kompilacija i pokretanje AFL++

`afl-clang-fast -o test_afl test_csv_afl.c libcsv.c`

| Opcija | Opis |
|--------|------|
| `afl-clang-fast` | LLVM-based instrumentirani kompajler za AFL++ koji omogućava praćenje izvršavanja programa i generisanje fuzz inputa |

`export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1`

Opis | Onemogućava probleme sa core dump podešavanjima sistema, tako da AFL++ ne promaši kratke procese ili crash-eve |

`export AFL_SKIP_CPUFREQ=1`

Opis | Preskače proveru CPU frequency scaling-a i moguće performansne upozorenja |

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
