## Cerință

- Să se creeze tabelele bazei de date pornind de la schema logică obținută (schema corespunzătoare modelului relațional al bazei de date) pentru problema primită în săptămâna 2.
- Se vor adăuga minim 5 înregistrări în fiecare tabelă.
- Se vor crea minim 2 interogări.
- Se va crea o vizualizare (view).

## Problema din săptămâna 2

Structura ierarhică a unei organizații:

- Un **grup** are un id unic și un nume. Un grup are mai multe companii, iar o companie aparține unui singur grup.
- O **companie** are un id unic, un nume și numele fondatorului (unul sau mai mulți). Fiecare companie are mai multe fabrici, iar o fabrică aparține unei singure companii.
- O **fabrică** are un id, un nume și o adresă (strada, numărul, codul poștal). O fabrică produce mai multe articole, iar un articol poate fi produs de mai multe fabrici.
- Un **articol** are un id unic, un nume, data de fabricare, termen de valabilitate și data expirării.

[Diagrama E-R a organizației](https://claude.ai/chat/er-tema-organizatie.svg)

## Schema relațională (pe entități)

|Tabel|Cheie primară|Atribute|
|---|---|---|
|**Grup**|`id_grup`|`nume`|
|**Companie**|`id_companie`|`nume_companie`, `nume_fondator`|
|**Fabrica**|`id_fabrica`|`nume_fabrica`, `adresa`|
|**Articol**|`id_articol`|`nume_articol`, `data_fabricare`, `data_expirare`|

- `adresa` rămâne într-o singură coloană.
- `termen_valabilitate` (atribut compus) apare ca `data_fabricare` și `data_expirare`.
- `nume_fondator` e într-o singură coloană. Fondatorii multipli sunt scriși în aceeași celulă, separați prin virgulă.
- Legăturile 1:N din diagramă (Grup → Companie și Companie → Fabrica) se adaugă la pasul 4, prin coloanele `id_grup` și `id_companie`, ca să putem face `JOIN`.

## 1. Crearea tabelelor

```sql
CREATE TABLE Grup (
    id_grup  NUMBER(5) PRIMARY KEY,
    nume     VARCHAR2(100) NOT NULL
);

CREATE TABLE Companie (
    id_companie   NUMBER(5) PRIMARY KEY,
    nume_companie VARCHAR2(100) NOT NULL,
    nume_fondator VARCHAR2(200)
);

CREATE TABLE Fabrica (
    id_fabrica   NUMBER(5) PRIMARY KEY,
    nume_fabrica VARCHAR2(100) NOT NULL,
    adresa       VARCHAR2(200)
);

CREATE TABLE Articol (
    id_articol     NUMBER(5) PRIMARY KEY,
    nume_articol   VARCHAR2(100) NOT NULL,
    data_fabricare DATE,
    data_expirare  DATE
);
```

## 2. Inserarea înregistrărilor (câte 5 în fiecare tabel)

```sql
INSERT INTO Grup (id_grup, nume) VALUES
    (1, 'Alpha Group'),
    (2, 'Beta Holding'),
    (3, 'Gamma Industries'),
    (4, 'Delta Corp'),
    (5, 'Omega Group');

INSERT INTO Companie (id_companie, nume_companie, nume_fondator) VALUES
    (10, 'AlphaFood', 'Ion Popescu, Maria Ionescu'),
    (11, 'AlphaCosmetics', 'Andrei Radu'),
    (12, 'BetaFoods', 'Elena Vasile, Dan Marin'),
    (13, 'GammaConserve', 'Mihai Stan'),
    (14, 'DeltaPack', 'Ana Pop');

INSERT INTO Fabrica (id_fabrica, nume_fabrica, adresa) VALUES
    (20, 'Fabrica Timisoara', 'Str. Libertatii 12, 300001'),
    (21, 'Fabrica Arad', 'Bd. Revolutiei 5, 310001'),
    (22, 'Fabrica Deva', 'Str. Mihai Viteazu 8, 330001'),
    (23, 'Fabrica Cluj', 'Str. Fabricii 25, 400001'),
    (24, 'Fabrica Sibiu', 'Str. Scolii 3, 550001');

INSERT INTO Articol (id_articol, nume_articol, data_fabricare, data_expirare) VALUES
    (500, 'Lapte', DATE '2026-10-01', DATE '2026-10-15'),
    (501, 'Iaurt', DATE '2026-10-05', DATE '2026-10-26'),
    (502, 'Paine', DATE '2026-10-16', DATE '2026-10-20'),
    (503, 'Conserva ton', DATE '2026-06-01', DATE '2028-06-01'),
    (504, 'Sampon', DATE '2026-03-10', DATE '2029-03-10');

COMMIT;
```

> ℹ️ Sintaxa cu mai multe rânduri după `VALUES` merge în Oracle 23ai. În versiunile mai vechi se folosește `INSERT ALL ... SELECT * FROM dual` sau câte un `INSERT` pe rând.

## 3. Interogări

### Interogarea 1

**Cerință:** să se afișeze articolele care expiră după 20.10.2026, ordonate după data expirării.

```sql
SELECT nume_articol Articol, data_fabricare Fabricat, data_expirare Expira
FROM Articol
WHERE data_expirare > DATE '2026-10-20'
ORDER BY data_expirare;
```

|ARTICOL|FABRICAT|EXPIRA|
|---|---|---|
|Iaurt|2026-10-05|2026-10-26|
|Conserva ton|2026-06-01|2028-06-01|
|Sampon|2026-03-10|2029-03-10|

### Interogarea 2

**Cerință:** să se afișeze companiile care aparțin grupului Alpha Group.

Pentru `JOIN` avem nevoie de legătura dintre Companie și Grup (relația 1:N din diagramă). O adăugăm cu `ALTER TABLE`:

```sql
ALTER TABLE Companie ADD id_grup NUMBER(5);

UPDATE Companie
SET id_grup = CASE id_companie
                WHEN 10 THEN 1
                WHEN 11 THEN 1
                WHEN 12 THEN 2
                WHEN 13 THEN 3
                WHEN 14 THEN 4
              END;

COMMIT;
```

```sql
SELECT C.nume_companie Companie, G.nume Grup
FROM Companie C INNER JOIN Grup G ON C.id_grup = G.id_grup
WHERE G.nume = 'Alpha Group';
```

|COMPANIE|GRUP|
|---|---|
|AlphaFood|Alpha Group|
|AlphaCosmetics|Alpha Group|

## 4. Vizualizări (view)

> ℹ️ **Ce e o vedere:** o interogare salvată în baza de date, cu nume, care se folosește ca un **tabel virtual** (nu stochează date, `SELECT`-ul din spate se execută de fiecare dată). Simplifică interogările complexe și permite acordarea drepturilor pe vedere, nu pe tabele.

### Vedere complexă (JOIN pe două tabele, doar interogări)

```sql
CREATE VIEW Vcompanii (Companie, Grup) AS
SELECT C.nume_companie, G.nume
FROM Companie C INNER JOIN Grup G ON C.id_grup = G.id_grup;

SELECT * FROM Vcompanii;

SELECT * FROM Vcompanii WHERE Grup = 'Alpha Group';
```

|COMPANIE|GRUP|
|---|---|
|AlphaFood|Alpha Group|
|AlphaCosmetics|Alpha Group|
|BetaFoods|Beta Holding|
|GammaConserve|Gamma Industries|
|DeltaPack|Delta Corp|

### Vedere simplă (o singură tabelă, fără agregate și GROUP BY, permite DML)

```sql
CREATE VIEW Varticole (Articol, Fabricat, Expirare) AS
SELECT nume_articol, data_fabricare, data_expirare
FROM Articol;

SELECT * FROM Varticole;

SELECT * FROM Varticole WHERE Expirare > DATE '2026-10-20';
```

Ultima interogare întoarce **Iaurt, Conserva ton, Sampon**.

### Vedere simplă vs complexă

| | Vedere simplă | Vedere complexă |
| :--- | :--- | :--- |
| **Exemplu** | `Varticole` | `Vcompanii` |
| **Definită pe** | o singură tabelă | `JOIN` pe două sau mai multe tabele |
| **Operații permise** | interogări și DML (`INSERT`, `UPDATE`, `DELETE`) | doar interogări |

## Capcane

> ⚠️ **Alias de tabel:** Dacă folosești `C.nume_companie`, trebuie să scrii `FROM Companie C`. Fără alias în `FROM` apare `ORA-00904: invalid identifier`.

> ⚠️ **ALTER înainte de JOIN:** Interogarea 2 și vederea `Vcompanii` merg doar după ce Companie are coloana `id_grup` completată.

> ⚠️ **Rulare comandă cu comandă:** Dacă editorul rulează o singură comandă o dată, apare `ORA-03405`. Rulează fiecare comandă separat sau folosește „Run Script".