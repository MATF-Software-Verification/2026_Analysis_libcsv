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
| `-Wall` | Aktivira upozorenja za sve uobičajene probleme u kodu. |
| `-Wextra` | Aktivira dodatna upozorenja koja nisu obuhvaćena sa `-Wall`. |
| `-fprofile-arcs` | Omogućava instrumentaciju za GCOV – beleži tok izvođenja programa. |
| `-ftest-coverage` | Generiše dodatne fajlove (`.gcno`, `.gcda`) za pokrivenost koda. |

### 2.2 GCOV opcije

| Opcija | Opis |
|--------|------|
| `-b` | Prikazuje branch coverage tj. pokrivenost logičkih grana (`if`, `switch`). |
| `-c` | Prikazuje call coverage tj. koliko je funkcijskih poziva izvršeno. |

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

- Parser je uspešno obradio 3 reda CSV podataka.
- Ispravno su prepoznata obična polja, numerička polja i quoted polja sa zarezom.
- Callback funkcije za polja i kraj reda su pozvane odgovarajući broj puta.
- Program je završio bez greške.

- Zaključak: osnovna funkcionalnost parsera radi ispravno.
    
---

## 5. GCOV analiza pokrivenosti

Pokretanje gcov alata:

`gcov -b -c unit-test_csv_unit.gcno`

### 5.1 Rezultati za test_csv_unit.c:

Lines executed:86.36% of 22
Branches executed:100.00% of 6
Taken at least once:66.67% of 6
Calls executed:81.82% of 11

### 5.2 Analiza pokrivenosti

Line coverage – 86.36%
Većina linija je izvršena. Neizvršene linije odnose se na obradu grešaka (NULL polja, neuspeh inicijalizacije parsera).

Branch coverage – 100% analizirano, 66.67% izvršeno
Sve logičke grane su analizirane, ali nisu sve izvršene u pozitivnim test slučajevima.

Call coverage – 81.82%
Većina funkcijskih poziva je izvršena, osim onih u granama za obradu grešaka.

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

- CSV parser funkcioniše ispravno za tipične ulazne podatke.
- Ostvarena je visoka pokrivenost linija (86.36%).
- Sve grane su analizirane, ali nisu sve izvršene tokom testova.
- Testovi pokrivaju glavni tok izvršavanja programa, ali za potpunu validaciju bi bilo potrebno dodati testove koji simuliraju greške i specijalne slučajeve.

---

## 8. Preporuka

- Dodati testove za negativne slučajeve i greške pri inicijalizaciji parsera.

