## Recapitulare

- **Normalizarea** = tehnică de proiectare prin care se urmărește **reducerea redundanței datelor** și eliminarea caracteristicilor nedorite: **anomaliile de inserare, de ștergere și de actualizare**.
- Relațiile în 2NF au o redundanță mai mică decât cele în 1NF, dar pot fi în continuare **vulnerabile la anomaliile de modificare/actualizare**.
- De aceea se aplică reguli noi, ca să aduci relațiile într-o **formă normală superioară**.

## A treia formă normală (3NF)

**Problema:** dacă vrei să actualizezi doar un tuplu de date și nu pe celelalte, baza de date poate ajunge într-o **stare inconsistentă**. Această anomalie de actualizare e cauzată de o **dependență tranzitivă**. Pentru a reduce și mai mult redundanța, astfel de dependențe trebuie **eliminate**, trecând la 3NF.

**Definiție:** o relație este în 3NF dacă:

- **îndeplinește condițiile pentru 1NF și 2NF**
- **nu există dependență tranzitivă pentru atributele non-prime** (niciun atribut care nu este cheie primară nu e dependent tranzitiv de cheia primară).

### Dependența tranzitivă

- Dacă A, B, C sunt atribute ale relației R și există dependențele **A → B** și **B → C**, se spune că **C este dependent tranzitiv de A prin intermediul lui B** (cu condiția ca A să nu fie dependent funcțional de B sau C).
- Pe scurt: o dependență tranzitivă apare atunci când **schimbarea unui atribut non-cheie poate provoca modificarea oricărui alt atribut non-cheie**.

### Cum se elimină (2NF → 3NF)

Se **elimină din relație atributele dependente tranzitiv** și se plasează într-o **relație nouă, împreună cu o copie a determinantului lor**.

### Exemplul din lab

Tabelul STUDENT:

|STUD_NO|STUD_NAME|STUD_STATE|STUD_COUNTRY|STUD_AGE|
|---|---|---|---|---|
|1|RAM|HARYANA|INDIA|20|
|2|RAM|PUNJAB|INDIA|19|
|3|SURESH|PUNJAB|INDIA|21|

- `STUD_NO → STUD_STATE` și `STUD_STATE → STUD_COUNTRY` sunt adevărate: `STUD_STATE` depinde de `STUD_NO`, iar `STUD_COUNTRY` depinde de `STUD_STATE`. Ultimele două atribute **nu sunt atribute cheie**.
- Deci `STUD_COUNTRY` este **dependent tranzitiv** de `STUD_NO`, și se încalcă regulile pentru 3NF.
- **Descompunerea** relației STUDENT(`STUD_NO`, `STUD_NAME`, `STUD_STATE`, `STUD_COUNTRY`, `STUD_AGE`) în alte 2 relații:
    - **STUDENT** (`STUD_NO`, `STUD_NAME`, `STUD_STATE`, `STUD_AGE`)
    - **STATE_COUNTRY** (`STATE`, `COUNTRY`)

> ℹ️ **Scăpare din PDF:** în descompunere, relația STUDENT apare cu `STUD_PHONE`, deși acest atribut nu era în relația inițială. Pare o greșeală de redactare, rămâne ca în lab doar `STUD_NO`, `STUD_NAME`, `STUD_STATE`, `STUD_AGE`.

## A patra formă normală (4NF)

**Problema:** formele 1NF, 2NF și 3NF elimină anomaliile datorate **dependențelor funcționale**, dar mai există un tip de dependență care poate cauza redundanță: **dependența multivalorică**.

Acest tip de dependență poate apărea din cauza primei forme normale, care nu permite unui atribut dintr-o înregistrare să aibă mai mult de 1 valoare. De exemplu, dacă într-o relație există **două atribute multivalorice**, trebuie să repeți **fiecare valoare a unuia dintre atribute împreună cu fiecare a celuilalt**, ceea ce duce la **redundanța datelor**.

**Definiție:** o relație este în 4NF dacă:

- se află în **1NF, 2NF și 3NF**, și
- **nu conține dependențe multivalorice**.

### Dependența multivalorică

- Există o dependență multivalorică între atributele A, B, C ale unei relații dacă, pentru **fiecare valoare a lui A**, există o **mulțime de valori ale lui B** și o **mulțime de valori ale lui C**, dar mulțimile B și C sunt **independente** una de cealaltă.
- Se notează **A ↠ B**.

### Cum se elimină

Se **plasează atributul (atributele) într-o nouă relație, împreună cu o copie a determinantului**.

### Exemplul din lab (restaurante și pizza)

Fiecare restaurant livrează un tip de pizza într-o arie de distribuție. Cheia primară e formată din (`Restaurant`, `Tip de pizza`, `Aria de distribuție`) și nu există atribute non-cheie, deci **nu se încalcă nici 1NF, nici 2NF, nici 3NF**.

Dar varietățile de pizza oferite de un restaurant sunt **independente** de ariile de distribuție, deci apare redundanță:

- pentru Restaurant **Jerry's** se menționează de **3 ori** că oferă tipul „Pufos";
- dacă vrei să adaugi tipul „Subțire" la Jerry's, trebuie să adaugi **3 înregistrări**, câte una pentru fiecare arie de distribuție.

**Tabelul inițial:**

|Restaurant|Tip de pizza|Aria de distribuție|
|---|---|---|
|Pizza Hut|Clasic|Dr. Taberei|
|Pizza Hut|Clasic|Militari|
|Pizza Hut|Pufos|Dr. Taberei|
|Pizza Hut|Pufos|Militari|
|Springtime|Pufos|Domenii|
|Springtime|Clasic|Domenii|
|Jerry's|Clasic|Dr. Taberei|
|Jerry's|Clasic|Militari|
|Jerry's|Clasic|Crangasi|
|Jerry's|Pufos|Dr. Taberei|
|Jerry's|Pufos|Militari|
|Jerry's|Pufos|Crangasi|

**Soluția:** descompunerea în **două tabele**, separând dependențele `<Restaurant - Tip de pizza>` și `<Restaurant - Aria de distribuție>`.

_Restaurant - Tip de pizza_

|Restaurant|Tip de pizza|
|---|---|
|Pizza Hut|Clasic|
|Pizza Hut|Pufos|
|Springtime|Pufos|
|Springtime|Clasic|
|Jerry's|Clasic|
|Jerry's|Pufos|

_Restaurant - Aria de distribuție_

|Restaurant|Aria de distribuție|
|---|---|
|Pizza Hut|Dr. Taberei|
|Pizza Hut|Militari|
|Springtime|Domenii|
|Jerry's|Dr. Taberei|
|Jerry's|Militari|
|Jerry's|Crangasi|

## Capcane de examen

> ⚠️ **Dependența tranzitivă:** E lanțul A → B → C, unde B și C sunt atribute **non-cheie**. Nu e tranzitivă o dependență direct de cheie.

> ⚠️ **3NF vine după 2NF:** O relație în 3NF trebuie să fie și în 1NF și 2NF. Nu sari peste pași.

> ⚠️ **Copia determinantului:** La 3NF și 4NF, atributul scos merge într-o relație nouă **împreună cu o copie a determinantului** (`STATE` în STATE_COUNTRY, `Restaurant` în ambele tabele de pizza). Fără copie, nu mai poți reface legătura.

> ⚠️ **4NF prinde ce nu prind 1NF-3NF:** Tabelul cu pizza nu are atribute non-cheie, deci trece de 1NF, 2NF și 3NF, dar tot are redundanță. Doar 4NF o prinde.

> ⚠️ **Independența mulțimilor:** Dependența multivalorică cere ca mulțimile B și C să fie **independente**. Dacă una depinde de cealaltă, nu e dependență multivalorică.

> ⚠️ **Formele normale se bazează pe dependențe:** 3NF elimină dependențele **tranzitive** (funcționale), 4NF elimină dependențele **multivalorice**.

## De reținut pe scurt

- **3NF**: în 1NF + 2NF și fără dependențe tranzitive. Rezolvare: atributul dependent tranzitiv merge într-o relație nouă, cu copia determinantului.
- **4NF**: în 1NF + 2NF + 3NF și fără dependențe multivalorice. Rezolvare: separi atributele multivalorice independente în tabele distincte, fiecare cu copia determinantului.
- Exemplele din lab: **STUDENT / STATE_COUNTRY** pentru 3NF, **restaurant / pizza / arie de distribuție** pentru 4NF.