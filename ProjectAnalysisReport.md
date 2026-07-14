# Unit testovi

Za proveru funkcionalne ispravnosti biblioteke razvijen je skup jedinčnih testova implementiran u programu `test_csv_unit.c`. Testovi proveravaju ispravan rad parsera nad različitim tipovima ulaznih podataka, kao i ispravnost pojedinih funkcija koje biblioteka pruža.

Skup testova obuhvata sledeće scenarije:

- parsiranje standardnog CSV zapisa,
- obradu polja koja sadrže zarez unutar navodnika,
- obradu praznih polja,
- obradu escape navodnika,
- obradu CRLF završetaka redova,
- parsiranje jednog polja,
- obradu praznog ulaza,
- obradu razmaka oko polja,
- obradu praznih redova,
- obradu znaka novog reda unutar navodnika,
- obradu razmaka nakon zatvorenih navodnika,
- parsiranje reda sa velikim brojem kolona.

Pored osnovnih scenarija, implementirani su i dodatni testovi kojima se proveravaju:

- promena delimitera (`csv_set_delim`),
- promena karaktera za navodnike (`csv_set_quote`),
- obrada velikog polja,
- rad parsera u strogom režimu (`CSV_STRICT`),
- funkcija za upis CSV podataka (`csv_write`),
- getter funkcije (`csv_get_delim`, `csv_get_quote`, `csv_get_opts` i `csv_get_buffer_size`).

Nakon izvršavanja svih testova dobijen je rezultat da su svi testovi uspešno prošli, čime je potvrđena funkcionalna ispravnost implementiranih scenarija.

---

# GCov analiza

Nakon uspešnog izvršavanja jedinčnih testova korišćen je alat **GCov** za analizu pokrivenosti izvornog koda biblioteke `libcsv.c`.

Dobijeni rezultati pokrivenosti su:

| Metrika | Vrednost |
|---------|---------:|
| Pokrivenost linija (Lines executed) | **67.18%** |
| Izvršene grane (Branches executed) | **67.60%** |
| Grane izvršene bar jednom (Taken at least once) | **42.99%** |
| Izvršeni pozivi funkcija (Calls executed) | **30.77%** |

Rezultati pokazuju da jedinčni testovi pokrivaju više od dve trećine izvornog koda biblioteke, kao i značajan deo grananja parsera. Najveća pokrivenost ostvarena je u funkcijama koje učestvuju u obradi CSV ulaza, dok pojedine pomoćne funkcije i ređe izvršavane grane nisu obuhvaćene postojećim skupom testova.

Dobijena pokrivenost predstavlja dobru osnovu za dalju analizu kvaliteta biblioteke, koja je zatim proširena korišćenjem alata **Valgrind Memcheck**, **AddressSanitizer**, **Clang-Tidy**, **Lizard** i **AFL++**.

## Clang-Tidy analiza

Za statičku analizu izvornog koda korišćen je alat Clang-Tidy.
Analiza je izvršena nad fajlom `libcsv.c`, a rezultati su sačuvani u
fajlu `clang_tidy.txt`.

Clang-Tidy je prijavio ukupno 16 upozorenja. Sva upozorenja pripadaju istoj
kategoriji: `clang-analyzer-core.NULLDereference`

Upozorenja se odnose na moguće dereferenciranje pokazivača `entry_buf`
sa vrednošću `NULL` u okviru makroa `SUBMIT_FIELD` i `SUBMIT_CHAR`, koji se
koriste za upis podataka u interni bafer parsera.

Primer prijavljene lokacije:


`((p)->entry_buf[entry_pos]) = '\0';`

Ova upozorenja predstavljaju potencijalne putanje koje je statički analizator
identifikovao, ali nisu potvrđena tokom dinamičkog testiranja. Valgrind
Memcheck, AddressSanitizer i fuzz testiranje nisu pronašli greške vezane za
nevalidne pristupe memoriji, zbog čega se ova upozorenja tretiraju kao
potencijalni rizik, a ne kao potvrđena greška u implementaciji.


# Ciklomatska složenost

Alatom **lizard** analizirana je ciklomatska složenost biblioteke **libcsv** na osnovu njenog izvornog koda. 
Alat analizira strukturu programa i na osnovu broja grananja, petlji i drugih kontrolnih tokova određuje složenost pojedinačnih funkcija.

Ciklomatska složenost predstavlja meru broja nezavisnih putanja kroz programski kod. Veća vrednost ukazuje na veći broj mogućih tokova izvršavanja, što može otežati testiranje, održavanje i razumevanje koda.

Analiza je izvršena pokretanjem skripte `run_lizard.sh`, koja pokreće alat lizard i rezultat analize upisuje u izveštaj `lizard_report.txt`, dok se dodatno generiše HTML izveštaj koji omogućava pregledniji prikaz rezultata analize.

HTML izveštaj sadrži tabelarni prikaz analiziranih funkcija, njihove linije koda, broj parametara, broj tokena, kao i vrednosti ciklomatske složenosti.

Na osnovu generisanog izveštaja vidi se da biblioteka sadrži ukupno **23 funkcije**, sa prosečnom vrednošću ciklomatske složenosti **5.5**. 
Prosečan broj linija koda po funkciji iznosi **14.9**, dok je ukupan broj linija koda analiziranih funkcija **374**.

Najveću ciklomatsku složenost ima funkcija: `csv_parse` koja sadrži:
- NLOC: 132
- CCN: 49
- broj parametara: 4
- dužina funkcije: 149 linija

Ova funkcija predstavlja glavni deo biblioteke jer obavlja parsiranje CSV ulaza i obrađuje različite slučajeve kao što su razdvajanje polja, obrada navodnika, escape karaktera, promene stanja parsera i završetak redova. 
Zbog velikog broja mogućih stanja parsera i velikog broja grananja očekivano je da upravo ova funkcija ima najveću ciklomatsku složenost.

Pored funkcije `csv_parse`, ostale funkcije biblioteke imaju značajno manju složenost. 
Većina pomoćnih funkcija, kao što su funkcije za podešavanje parametara parsera (`csv_set_opts`, `csv_set_delim`, `csv_set_quote`) i funkcije za oslobađanje memorije (`csv_free`), imaju ciklomatsku složenost između 1 i 4, što ukazuje na jednostavniju strukturu.

Lizard je prijavio jednu funkciju koja prelazi zadati prag složenosti: **cyclomatic_complexity > 15**

To je funkcija `csv_parse` sa ciklomatskom složenošću 49. 
Iako je vrednost relativno visoka, ona je očekivana jer ova funkcija implementira kompletnu logiku CSV parsera i mora da podrži veliki broj različitih ulaznih slučajeva.

Dobijeni rezultati pokazuju da je najveći deo kompleksnosti biblioteke koncentrisan u jednoj centralnoj funkciji, dok su ostale funkcije jednostavne i lako održive.


## Valgrind Memcheck analiza

Alatom Valgrind Memcheck izvršena je analiza upravljanja memorijom tokom
izvršavanja unit testova biblioteke libcsv. Test je pokrenut sa opcijama
`--leak-check=full` i `--show-leak-kinds=all`, čime je omogućena detaljna
provera curenja memorije i nepravilnog korišćenja memorijskih resursa.

Tokom inicijalnog pokretanja pronađena je greška korišćenja neinicijalizovane
vrednosti u test funkciji `write_test`, gde izlazni bafer nije bio pravilno
terminisan nul-karakterom pre poziva `printf`. Nakon korekcije testa,
ponovljenom analizom nisu pronađene greške.

Konačni rezultat Valgrind analize je:

- 19 izvršenih alokacija i 19 oslobađanja memorije,
- 0 bajtova memorije ostalo zauzeto nakon završetka programa,
- 0 detektovanih grešaka.

Dobijeni rezultat potvrđuje da biblioteka tokom izvršavanja testiranih
funkcionalnosti nema uočene probleme sa curenjem memorije niti sa
neispravnim pristupom memorijskim lokacijama.


## AddressSanitizer analiza

Za proveru memorijske bezbednosti korišćen je alat AddressSanitizer (ASan).
Biblioteka `libcsv` je kompajlirana sa opcijom `-fsanitize=address`, nakon
čega je izvršen postojeći skup jedinčnih testova.

Tokom izvršavanja svih testova AddressSanitizer nije detektovao greške
vezane za upravljanje memorijom. Nisu pronađena prekoračenja bafera,
korišćenje oslobođene memorije niti nevalidni pristupi memoriji.

Rezultat analize:
`ASan nije pronašao memorijske greške.`

## AFL++ fuzz testiranje

Za dinamičko testiranje biblioteke korišćen je alat AFL++ (American Fuzzy Lop Plus Plus). Kao početni skup ulaza pripremljeno je više CSV fajlova koji obuhvataju različite karakteristične slučajeve, kao što su standardni CSV zapis, prazna polja, navodnici.

Test program učitava CSV fajl koji AFL++ prosledi kao ulaz, nakon čega njegov sadržaj prosleđuje funkciji `csv_parse`. AFL++ zatim automatski generiše veliki broj novih ulaza mutacijom početnih *seed* fajlova i izvršava parser nad svakim od njih, sa ciljem da istraži što veći broj putanja izvršavanja.

Rezultati fuzz testiranja nalaze se u direktorijumu `findings`, gde se posebno izdvajaju:
- `queue` – skup ulaza koje je AFL++ generisao i koji su doveli do novih ili zanimljivih putanja izvršavanja,
- `crashes` – ulazi koji izazivaju pad programa,
- `hangs` – ulazi koji dovode do zastoja ili prekoračenja vremenskog ograničenja.

Tokom sprovedenog fuzz testiranja nisu pronađeni ulazi koji izazivaju pad programa niti zastoj u izvršavanju, odnosno direktorijumi `crashes` i `hangs` ostali su prazni. Istovremeno, direktorijum `queue` sadrži veliki broj automatski generisanih ulaza, što pokazuje da je AFL++ uspešno pronašao nove putanje izvršavanja i povećao pokrivenost testiranja parsera.

# Zaključak analize

U okviru rada izvršena je sveobuhvatna analiza biblioteke **libcsv** primenom više metoda za proveru kvaliteta i pouzdanosti softvera.

Na osnovu sprovedenih analiza može se zaključiti da biblioteka **libcsv** predstavlja stabilnu i kvalitetno implementiranu biblioteku za obradu CSV podataka. Iako postoji prostor za povećanje pokrivenosti testovima i dodatno razmatranje upozorenja prijavljenih statičkom analizom, nijedan od primenjenih dinamičkih alata nije otkrio greške koje bi ukazivale na probleme u upravljanju memorijom ili stabilnosti izvršavanja.
