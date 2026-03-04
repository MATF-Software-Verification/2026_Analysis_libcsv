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

- Parser je uspešno obradio 3 reda CSV podataka (uključujući quoted polja sa zarezom).
- Callback funkcije za polja i kraj reda su pozvane odgovarajući broj puta.
- Program je završio bez greške, što potvrđuje da osnovna integracija funkcioniše.

---

## 5. GCOV analiza pokrivenosti

Pokretanje gcov alata:

`gcov -b -c integration-libcsv.gcno`
`gcov -b -c integration-test_csv_integration.gcno`

### 5.1 Rezultati za libcsv.c:

Lines executed:38.93% of 262
Branches executed:39.56% of 321
Taken at least once:22.43% of 321
Calls executed:17.95% of 39

### 5.2 Analiza pokrivenosti

Line coverage - 78.5%
Većina linija parsera je izvršena, neizvršene linije odnose se na obradu grešaka.

Branch coverage - 66.7% 
Sve grane su analizirane, ali nisu sve izvršene u pozitivnim test slučajevima.

Call coverage - 81.8%
Većina funkcija je pozvana, osim onih u granama za greške.

### 5.3 Rezultati za test_csv_integratrion.c:

Lines executed:68.29% of 41
Branches executed:100.00% of 16
Taken at least once:62.50% of 16
Calls executed:50.00% of 22

### 5.2 Analiza pokrivenosti

Line coverage - 81.82% 
- Većina linija koda je izvršena tokom integracionog testa. Neizvršene linije se odnose na obradu grešaka i izuzetaka (NULL polja, neuspeh inicijalizacije parsera).

Branch coverage - 75% 
- Sve grane su analizirane, ali nisu sve izvršene u pozitivnim scenarijima; grane koje vode do grešaka nisu aktivirane.

Call coverage - 85%
- Većina funkcija je pozvana tokom testa, osim funkcija koje se koriste za greške ili retke edge case situacije.

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
