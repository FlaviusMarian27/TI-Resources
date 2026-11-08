
## Cerință

Normalizați baza de date aferentă diagramei ER realizate la primul laborator, folosind tehnicile **1NF** și **2NF**.

![Diagrama E-R de la primul laborator](/Laborator2/er-tema-organizatie.svg)

## Regulile folosite (din laboratorul 2)

- **1NF**: **fiecare celulă** conține **o singură valoare**, iar **fiecare înregistrare** este **unică**. Un atribut cu mai multe valori se rezolvă punând fiecare valoare pe **rând separat**.
- **2NF**: relația e în **1NF** și **fiecare atribut care nu e cheie primară depinde total de cheia primară**. Se verifică **doar la chei compuse**. O dependență **parțială** se elimină **descompunând tabelul în două**.

> ℹ️ Datele din tabele sunt **inventate**, ca exemple (minim 5 înregistrări pe tabel).

## Tabelele (câte una pentru fiecare entitate)

### 1. Grup (`id_grup`, `nume`)

|id_grup|nume|
|---|---|
|1|Alpha Group|
|2|Beta Holding|
|3|Gamma Industries|
|4|Delta Corp|
|5|Omega Group|

- **1NF**: ok, o singură valoare în fiecare celulă, rânduri unice.
- **2NF**: cheia primară are **un singur atribut** (`id_grup`), deci tabelul e automat cel puțin în 2NF.

### 2. Companie (`id_companie`, `nume_companie`, `nume_fondator`)

**Tabelul inițial (nenormalizat):**

|id_companie|nume_companie|nume_fondator|
|---|---|---|
|10|AlphaFood|Ion Popescu, Maria Ionescu|
|11|AlphaCosmetics|Andrei Radu|
|12|BetaFoods|Elena Vasile, Dan Marin|
|13|GammaConserve|Mihai Stan|
|14|DeltaPack|Ana Pop|

**Verificare 1NF: nu e în 1NF.** `nume_fondator` e atribut cu **mai multe valori** (o companie are unul sau mai mulți fondatori), exact ca `STUD_PHONE` din lab. Celulele de la companiile 10 și 12 conțin două valori.

**Aducere în 1NF** (câte un fondator pe rând):

|id_companie|nume_companie|nume_fondator|
|---|---|---|
|10|AlphaFood|Ion Popescu|
|10|AlphaFood|Maria Ionescu|
|11|AlphaCosmetics|Andrei Radu|
|12|BetaFoods|Elena Vasile|
|12|BetaFoods|Dan Marin|
|13|GammaConserve|Mihai Stan|
|14|DeltaPack|Ana Pop|

Acum `id_companie` nu mai e unic, deci **cheia primară devine compusă**: `{id_companie, nume_fondator}`.

**Verificare 2NF: nu e în 2NF.** `nume_companie` depinde doar de `id_companie`, adică de **o parte din cheie** (`id_companie → nume_companie`). Asta e o **dependență parțială**, ca `COURSE_NO → COURSE_FEE` din lab. Semnul vizibil: „AlphaFood" și „BetaFoods" apar de mai multe ori.

**Aducere în 2NF** (descompunere în 2 tabele):

_Companie_ (cheie: `id_companie`)

|id_companie|nume_companie|
|---|---|
|10|AlphaFood|
|11|AlphaCosmetics|
|12|BetaFoods|
|13|GammaConserve|
|14|DeltaPack|

_Fondator_Companie_ (cheie: `{id_companie, nume_fondator}`)

|id_companie|nume_fondator|
|---|---|
|10|Ion Popescu|
|10|Maria Ionescu|
|11|Andrei Radu|
|12|Elena Vasile|
|12|Dan Marin|
|13|Mihai Stan|
|14|Ana Pop|

Acum `nume_companie` e stocat o singură dată, iar în _Fondator_Companie_ nu mai există atribute în afara cheii, deci ambele sunt în 2NF.

### 3. Fabrică (`id_fabrica`, `nume_fabrica`, `adresa`)

|id_fabrica|nume_fabrica|adresa|
|---|---|---|
|20|Fabrica Timișoara|Str. Libertății 12, 300001|
|21|Fabrica Arad|Bd. Revoluției 5, 310001|
|22|Fabrica Deva|Str. Mihai Viteazu 8, 330001|
|23|Fabrica Cluj|Str. Fabricii 25, 400001|
|24|Fabrica Sibiu|Str. Școlii 3, 550001|

- **1NF**: ok. `adresa` rămâne **într-o singură coloană**, tratată ca o valoare, cum a spus profu la laborator (nu se mai descompune).
- **2NF**: cheia primară are **un singur atribut** (`id_fabrica`), deci e automat cel puțin în 2NF.

### 4. Articol (`id_articol`, `nume_articol`, `termen_valabilitate`)

**Tabelul inițial** (termenul de valabilitate, atribut compus, pus într-o singură celulă):

|id_articol|nume_articol|termen_valabilitate|
|---|---|---|
|500|Lapte|2026-10-01 - 2026-10-15|
|501|Iaurt|2026-10-05 - 2026-10-26|
|502|Pâine|2026-10-16 - 2026-10-20|
|503|Conservă ton|2026-06-01 - 2028-06-01|
|504|Șampon|2026-03-10 - 2029-03-10|

**Verificare 1NF: nu e în 1NF.** `termen_valabilitate` conține **două valori** într-o celulă (data fabricării și data expirării).

**Aducere în 1NF** (atributul compus se desparte în componentele lui):

|id_articol|nume_articol|data_fabricare|data_expirare|
|---|---|---|---|
|500|Lapte|2026-10-01|2026-10-15|
|501|Iaurt|2026-10-05|2026-10-26|
|502|Pâine|2026-10-16|2026-10-20|
|503|Conservă ton|2026-06-01|2028-06-01|
|504|Șampon|2026-03-10|2029-03-10|

- **2NF**: cheia primară are **un singur atribut** (`id_articol`), deci e automat cel puțin în 2NF. Nu mai e nimic de descompus.

## Schema finală (în 1NF și 2NF)

| Tabel                 | Cheie primară                  | Atribute                                          |
| --------------------- | ------------------------------ | ------------------------------------------------- |
| **Grup**              | `id_grup`                      | `nume`                                            |
| **Companie**          | `id_companie`                  | `nume_companie`                                   |
| **Fondator_Companie** | `{id_companie, nume_fondator}` | -                                                 |
| **Fabrică**           | `id_fabrica`                   | `nume_fabrica`, `adresa`                          |
| **Articol**           | `id_articol`                   | `nume_articol`, `data_fabricare`, `data_expirare` |