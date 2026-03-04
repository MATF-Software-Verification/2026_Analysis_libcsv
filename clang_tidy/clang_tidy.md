# Analiza projekta sa Clang-Tidy

## 1. Cilj analize

Clang-Tidy je statički analizator koda za C/C++ projekte koji detektuje moguće runtime greške, probleme sa pokazivačima, potencijalne null dereference, kao i druge sigurnosne i stilističke probleme. Cilj korišćenja Clang-Tidy u ovom projektu bio je da se:

- Otkrivaju runtime problemi i potencijalne null pointer dereference pre izvršavanja koda.
- Poboljša stabilnost i sigurnost biblioteke `libcsv`.
- Identifikuju problematični delovi koda vezani za upravljanje memorijom i kontrolu toka programa.
- Olakša refaktorisanje i održavanje koda.

Analiza pomaže programerima da pravovremeno otkriju i isprave kritične greške, smanjujući rizik od runtime crash-eva i nepredviđenog ponašanja programa.

---

## 2. Objašnjenje korišćenih opcija

| Opcija | Opis |
|--------|------|
| `clang-tidy libcsv.c csv.h` | Pokreće analizu na fajlovima `libcsv.c` i `csv.h`. |
| `--` | Separator između Clang-Tidy opcija i opcija kompajlera. |
| `-I.` | Uključuje trenutni direktorijum kao putanju za header fajlove (include path). |
| `> clang_tidy_results/clang_tidy.txt` | Preusmerava kompletan izlaz analize u fajl unutar direktorijuma za rezultate. |

Ove opcije omogućavaju detaljnu statičku analizu ključnih fajlova projekta sa fokusom na sigurnost i runtime greške.

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

- Sadrži upozorenja koja ukazuju na potencijalne null pointer dereference, što može izazvati runtime greške.  
- Većina upozorenja se odnosi na neophodnost dodatnih provera validnosti pokazivača i sigurnije upravljanje memorijom.  
- Refaktorisanje i implementacija dodatnih provera može značajno poboljšati stabilnost biblioteke `libcsv`.  
- Preporučuje se kombinovanje statičke analize sa unit testovima kako bi se osigurala potpuna pokrivenost i kvalitet koda.  

Rezultati analize su sačuvani u tekstualnom fajlu i mogu se dalje procesirati za generisanje PDF ili HTML izveštaja.
