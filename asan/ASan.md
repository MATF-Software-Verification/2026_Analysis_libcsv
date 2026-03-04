# Dinamička analiza – AddressSanitizer (ASan)

## 1. Podešavanje okruženja i kompilacija

Biblioteka **libcsv** testirana je korišćenjem AddressSanitizer alata ugrađenog u GCC kompajler.

Kompilacija je izvršena sledećom komandom:

`gcc -fsanitize=address -g test_csv_unit.c libcsv.c -o test`

| Opcija | Opis |
|--------|------|
| `-fsanitize=address` | Aktivira AddressSanitizer (ASan) za detekciju memorijskih grešaka tokom izvršavanja programa (heap/stack overflow, use-after-free, double free, invalid read/write). |
| `-g` | Dodaje debug simbole u izvršni fajl, omogućavajući precizne izveštaje sa linijama koda kada ASan detektuje grešku. |
| `test_csv_unit.c` | Test program koji pokreće različite test scenarije biblioteke libcsv. |
| `libcsv.c` | Izvorni kod biblioteke koja se analizira. |
| `-o test` | Naziv generisanog izvršnog fajla. |

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

