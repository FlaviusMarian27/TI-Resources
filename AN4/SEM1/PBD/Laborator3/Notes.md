## Ce este normalizarea

- Tehnică de proiectare care ajută la **reducerea redundanței datelor**.
- **Redundanța** = stocarea nejustificată a aceleiași informații de mai multe ori în baza de date.
- Procesul identifică relațiile dintre tabele bazându-se pe:
    - **cheile primare** ale tabelelor
    - **dependențele funcționale** dintre atributele lor
- Normalizarea **împarte un tabel mare în tabele mai mici** și definește relațiile logice dintre ele, ca să crească claritatea în organizarea datelor.
- **Scopul principal**: reducerea redundanței și a caracteristicilor nedorite, numite **anomalii**.

## Cele 3 anomalii

Exemplul din lab, tabelul **Produse** (conține și date despre furnizori):

|IdP|NumeP|Qty|IdF|NumeF|AdresaF|
|---|---|---|---|---|---|
|101|Imprimanta laser|30|20|Xerox|Str. Daniel Danielopolu 4-6, Sector 1, București|
|105|Calculator PC|20|23|IBM|Bd. D.Cantemir nr.1, București|
|124|Copiator|10|20|Xerox|Str. Daniel Danielopolu 4-6, Sector 1, București|

|Anomalie|Ce înseamnă|Exemplul din lab|
|---|---|---|
|**De inserare**|Nu poți introduce un tuplu nou într-o relație din cauza lipsei de date|Nu poți insera datele unui furnizor (nume, adresă) decât dacă există în stoc un produs furnizat de el|
|**De ștergere**|Ștergerea unei înregistrări duce la pierderea unor detalii care nu sunt stocate în altă parte|Ștergi din tabel ultimul produs al unui furnizor și se pierd automat și datele furnizorului|
|**De modificare / actualizare**|La actualizarea unei informații redundante, unele apariții se modifică, iar altele rămân cu vechea valoare|Dacă un furnizor își modifică adresa, trebuie actualizată în **toate** înregistrările în care apare|

> [!NOTE] **Cauza anomaliilor** Anomaliile apar pentru că într-o aceeași tabelă au fost stocate date despre **două clase diferite de obiecte** (aici produse și furnizori).

## Noțiuni importante

|Noțiune|Definiție din lab|
|---|---|
|**Formă nenormalizată**|Tabel care conține unul sau mai multe **grupuri repetitive** (ca tabelul Produse de mai sus)|
|**Grup repetitiv**|Atribut sau set de atribute din cadrul tabelului, care apar cu **valori multiple** pentru o singură instanță a atributului cheie|
|**Descompunerea schemei de relație**|Procesul de descompunere a unui tabel cu structură incorectă în **două sau mai multe tabele**|
|**Forme normale**|Reguli de corectitudine folosite ca să detectezi relațiile care trebuie descompuse. Se bazează pe noțiunea de **dependență** (funcțională sau multivalorică)|

- Tehnica de normalizare presupune o serie de reguli, folosite pentru a testa relațiile individuale.
- Când o cerință **nu este îndeplinită**, relația care o "deranjează" trebuie descompusă în relații care satisfac individual cerințele normalizării.

## Prima formă normală (1NF)

**Definiție:** o relație este în 1NF dacă **toate atributele ei iau valori unice**. Mai concret:

- **fiecare celulă** din tabel trebuie să conțină **o singură valoare**
- **fiecare înregistrare** trebuie să fie **unică**

**Exemplul din lab:** relația STUDENT nu este în 1NF din cauza atributului cu mai multe valori `STUD_PHONE`.

Tabelul 1 (nu e în 1NF):

|STUD_NO|STUD_NAME|STUD_PHONE|STUD_STATE|STUD_COUNTRY|
|---|---|---|---|---|
|1|RAM|9716271721, 9871717178|HARYANA|INDIA|
|2|RAM|9898297281|PUNJAB|INDIA|
|3|SURESH||PUNJAB|INDIA|

**Conversia la prima formă normală:** fiecare număr de telefon ajunge pe un **rând separat** (studentul 1 apare pe două rânduri).

Tabelul 2 (în 1NF):

|STUD_NO|STUD_NAME|STUD_PHONE|STUD_STATE|STUD_COUNTRY|
|---|---|---|---|---|
|1|RAM|9716271721|HARYANA||
|1|RAM|9871717178|HARYANA|INDIA|
|2|RAM|9898297281|PUNJAB|INDIA|
|3|SURESH||PUNJAB|INDIA|

> [!NOTE] **Scăpare din PDF** La primul rând din Tabelul 2, `STUD_COUNTRY` e gol, deși în Tabelul 1 era INDIA. Pare o greșeală de redactare, nu o regulă.

## A doua formă normală (2NF)

**Definiție:** se bazează pe conceptul de **dependență funcțională totală**. Pentru ca o relație să fie în 2NF:

- trebuie să fie în **1NF**
- **toate atributele din cadrul entității trebuie să depindă numai de identificatorul unic al entității**, adică: fiecare atribut care **nu este cheie primară** este **total dependent funcțional** de cheia primară

**Dependența funcțională A → B:**

|Tip|Definiție|
|---|---|
|**Totală**|Eliminarea **oricărui** atribut din A duce la anularea dependenței|
|**Parțială**|Există un atribut care **poate fi eliminat** din A și totuși dependența se menține|

**Când se aplică:**

- 2NF se aplică relațiilor cu **chei compuse**, adică relațiilor la care cheia primară e compusă din două sau mai multe atribute.
- O relație a cărei cheie primară are **un singur atribut** se află **cel puțin în 2NF**.

### Exemplul din lab

Tabelul inițial (cheia candidată: `{STUD_NO, COURSE_NO}`):

|STUD_NO|COURSE_NO|COURSE_FEE|
|---|---|---|
|1|C1|1000|
|2|C2|1500|
|1|C4|2000|
|4|C3|1000|
|4|C1|1000|
|2|C5|2000|

Raționamentul din lab:

- Mai multe cursuri au aceeași taxă. `COURSE_FEE` **nu poate decide singur** valoarea lui `COURSE_NO` sau `STUD_NO`.
- `COURSE_FEE` împreună cu `STUD_NO` nu poate decide `COURSE_NO`, iar `COURSE_FEE` împreună cu `COURSE_NO` nu poate decide `STUD_NO`.
- Deci `COURSE_FEE` este un atribut **non-prim**: nu aparține singurei chei candidate `{STUD_NO, COURSE_NO}`.
- Există o **dependență parțială**: `COURSE_FEE` depinde de `COURSE_NO` (`COURSE_NO → COURSE_FEE`), care este un **subset adecvat** al cheii candidate.

**Conversia la 2NF:** tabelul se descompune în **2 tabele**.

Tabelul 1:

|STUD_NO|COURSE_NO|
|---|---|
|1|C1|
|2|C2|
|1|C4|
|4|C3|
|4|C1|
|2|C5|

Tabelul 2:

|COURSE_NO|COURSE_FEE|
|---|---|
|C1|1000|
|C2|1500|
|C3|1000|
|C4|2000|
|C5|2000|

## Capcane de examen

> [!WARNING] **1NF cere două lucruri** O singură valoare în fiecare celulă **și** înregistrări unice. Una singură nu ajunge.

> [!WARNING] **2NF se verifică doar la chei compuse** O relație cu cheia primară dintr-un singur atribut e automat cel puțin în 2NF.

> [!WARNING] **Totală vs parțială** Totală = eliminarea oricărui atribut din A anulează dependența. Parțială = poți elimina un atribut din A și dependența rămâne. Nu le inversa.

> [!WARNING] **Atribut non-prim** `COURSE_FEE` nu face parte din cheia candidată `{STUD_NO, COURSE_NO}`, deci e non-prim. Depinde de un **subset** al cheii (`COURSE_NO`), deci dependența e **parțială**.

> [!WARNING] **Cauza anomaliilor** Într-o aceeași tabelă sunt stocate date despre **două clase diferite de obiecte**.

> [!WARNING] **Cele trei anomalii** Inserare = nu poți adăuga fără alte date. Ștergere = pierzi date care nu sunt stocate în altă parte. Modificare = actualizezi unele apariții și altele rămân cu valoarea veche.

## De reținut pe scurt

- Normalizare = reducerea **redundanței** și a **anomaliilor**, prin împărțirea tabelelor mari.
- **1NF**: o valoare pe celulă, rânduri unice. Rezolvare: o valoare pe rând.
- **2NF**: în 1NF + fără dependențe parțiale de cheia primară. Rezolvare: descompunere în 2 tabele.
- Lab-ul se oprește la **2NF**.