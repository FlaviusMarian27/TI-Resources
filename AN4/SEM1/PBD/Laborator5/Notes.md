## 1. Structura SGBD Oracle

Componentele de bază:

|Componentă|Rol|
|---|---|
|**Oracle SQL Server**|Execută **toate comenzile SQL**. E **singura** componentă cu acces la informațiile din baza de date, asigură **integritatea și securitatea** datelor|
|**SQL*Plus**|Mediu de dezvoltare, **interfața cu utilizatorii**: are comenzi de editare, comunică cu serverul și lansează programe PL/SQL|
|**PL/SQL**|**Limbaj procedural** pentru dezvoltare de aplicații|

- Pentru **fiecare utilizator conectat** se deschide o **sesiune separată**.
- Fiecare utilizator are parola lui, dar **mai multe sesiuni** se pot lansa pe **același user**, de la stații diferite sau de pe aceeași stație.
- Pe o stație se pot lansa mai multe sesiuni prin SQL*Plus, chiar pe servere diferite.

### Conectare (SQL*Plus)

- **User Name:** `SCOTT` (user existent, cu drepturi limitate)
- **Password:** `TIGER`
- **Host string:** numele bazei de date existente pe server (ex. `B623server`). Pentru sala B529 se dă adresa IP a serverului Oracle, în formatul din PDF `192.168.113.200:1522/orcl` sau `192.16.21.100:1521/orcl2`.

## 2. Comenzi SQL de bază

### Creare tabelă

```sql
CREATE TABLE Stud (Nume Char(20), Adresa Varchar2(15), Bursa Number(7), Datan Date);
```

- `Varchar2(20)` = șir de caractere de **lungime variabilă, maxim 20**.

### Ștergere tabelă

```sql
DROP TABLE stud1;   -- nu salvează înregistrările în Rollback
```

### Inserare

```sql
INSERT INTO stud1 (nume, adresa, bursa, datan)   -- lista de câmpuri
       VALUES ('Radu', 'Lugoj', 1200, '15-jan-77'); -- lista de valori
```

### Ștergere înregistrări

```sql
DELETE FROM stud WHERE Adresa LIKE 'Lugoj%';
```

- `%` = **un număr oarecare** de caractere.
- `_` = **un singur caracter**, cu orice valoare.

### Rollback, Commit, Truncate

|Comandă|Ce face|
|---|---|
|`DELETE FROM stud;`|Șterge **toate** înregistrările, **salvându-le în Rollback**|
|`ROLLBACK;`|Reface toate înregistrările șterse sau modificate **după ultimul COMMIT**|
|`COMMIT;`|**Șterge fișierul Rollback**, iar modificările devin **definitive**|
|`TRUNCATE TABLE Stud;`|Șterge **numai înregistrările**, tabela rămâne **goală**|

### Modificare (UPDATE)

```sql
UPDATE stud SET bursa = 1200 WHERE Nume = 'Radu';
UPDATE stud SET bursa = bursa * 1.2;      -- mărește TOATE bursele cu 20%
```

### Afișarea tabelelor

```sql
SELECT * FROM TAB;      -- numele tabelelor utilizatorului curent
SELECT * FROM CAT;      -- catalogul utilizatorului
SELECT Owner, table_name FROM All_tables;                    -- toate datele despre tabele (sute)
SELECT Owner, table_name FROM All_tables WHERE owner='SCOTT'; -- doar tabelele lui SCOTT
DESCR All_tables        -- structura tabelei All_tables (peste 30 de coloane)
```

## 3. Selecturi imbricate (SELECT în SELECT)

Salariații cu **aceeași funcție ca SCOTT**:

```sql
SELECT Ename Nume, Deptno, Job Functie, Sal Salar FROM Emp
WHERE job = ANY (SELECT job FROM Emp WHERE Ename = 'SCOTT');
```

Rezultat: FORD și SCOTT (ambii ANALYST, 3000).

Dacă subselectul întoarce **mai mult de o înregistrare**, se folosește `ANY` sau `ALL`:

- **ANY**: condiția e îndeplinită dacă **cel puțin o** valoare selectată o îndeplinește.
- **ALL**: condiția e îndeplinită dacă **toate** valorile selectate o îndeplinesc.

Salariu **mai mare decât salariul mediu din toate departamentele** (ALL):

```sql
SELECT Ename, Deptno, sal Salar FROM Emp
WHERE sal > ALL (SELECT avg(sal) FROM Emp GROUP BY deptno);
```

Rezultat: JONES (2975), SCOTT (3000), KING (5000), FORD (3000).

Cu **ANY** în loc de ALL: salariu mai mare decât salariul mediu din **cel puțin un** departament:

```sql
SELECT Ename, Deptno, sal Salar FROM Emp
WHERE sal > ANY (SELECT avg(sal) FROM Emp GROUP BY deptno);
```

Rezultat: 8 salariați (ALLEN, JONES, BLAKE, CLARK, SCOTT, KING, TURNER, FORD).

## 4. Funcții agregat și GROUP BY

Funcții agregat: `SUM`, `COUNT`, `AVG`, `MIN`, `MAX`. Fără `GROUP BY`, se calculează pentru **tot fișierul**, iar în lista de afișare se admit **doar funcții agregat**, nicio coloană a tabelei.

```sql
SELECT avg(sal) Salar_mediu, Sum(sal) Suma_sal,
       Max(Sal) Sal_max, Min(sal) Min_sal, Count(*) Nr FROM EMP;
```

Rezultat în PDF: 1864.0625 | 29825 | 5000 | 200 | 16.

### GROUP BY

- Se folosește pentru date despre un **grup de înregistrări** cu aceeași valoare într-una sau mai multe coloane (facultate, an, funcție, oraș etc.).
- Tabela se **sortează întâi** după coloana de grupare, apoi se calculează funcțiile agregat pentru **fiecare grup**.
- **Numai coloanele după care se face gruparea** pot apărea în lista de afișare, alături de funcțiile agregat.

Salariul mediu și numărul de salariați pe departament:

```sql
SELECT E.deptno Nr_dept, dname Nume_Dept, loc Adresa, avg(sal) Sal_med,
       COUNT(Ename) Nr_salariati
FROM Emp E, Dept D
WHERE d.deptno = e.deptno
GROUP BY E.deptno, dname, loc;
```

|NR_DEPT|NUME_DEPT|ADRESA|SAL_MED|NR_SALARIATI|
|---|---|---|---|---|
|10|ACCOUNTING|NEW YORK|2916.67|3|
|20|RESEARCH|DALLAS|1845.83|6|
|30|SALES|CHICAGO|1428.57|7|

### HAVING

Verifică **condiția de grup**: se afișează doar grupurile care o îndeplinesc.

```sql
SELECT Job, avg(sal) Salar_mediu, Sum(sal) Suma_sal,
       Max(Sal) Sal_max, Min(sal) Min_sal, Count(*) Nr
FROM EMP GROUP BY job
HAVING Avg(sal) > 1000;
```

Rezultat: ANALYST, CLERK, MANAGER, PRESIDENT, SALESMAN.

## 5. Inner, Self și Outer JOIN

### INNER JOIN

Join-ul clasic: preia din cele două tabele **numai înregistrările care au corespondent**. Salariații fără `Deptno` (conducere, întreținere) **nu apar**.

```sql
-- scriere clasică
SELECT E.Ename Nume, E.Sal Salar, D.Dname Nume_dept
FROM EMP E, DEPT D WHERE E.Deptno = D.Deptno AND sal < 1000;

-- scriere standard
SELECT E.Ename Nume, E.Sal Salar, D.Dname Nume_dept
FROM EMP E INNER JOIN DEPT D ON E.Deptno = D.Deptno AND sal < 1000;
```

Rezultat: SMITH (800, RESEARCH), JAMES (950, SALES).

### LEFT OUTER JOIN

Ia **toate înregistrările din prima tabelă** și:

- completează coloanele din a doua tabelă **dacă au corespondent** conform condiției de join;
- le lasă **goale** dacă nu există corespondent.

```sql
-- scriere clasică, cu (+)
SELECT E.Ename Nume, E.Sal Salar, E.deptno, D.Dname Nume_dept
FROM EMP E, DEPT D WHERE E.Deptno = D.Deptno(+) AND sal < 1000;

-- scriere standard
SELECT E.Ename Nume, E.Sal Salar, D.Dname Nume_dept
FROM EMP E LEFT OUTER JOIN DEPT D ON E.Deptno = D.Deptno AND sal < 1000;
```

Rezultat: SMITH, JAMES, plus **popa** (200) și **Popescu** (600), cu departamentul gol (nu au `Deptno`).

- **Semnul (+)** se pune **după coloana din condiția de join care corespunde tabelei incomplete**.

### RIGHT OUTER JOIN

Lucrează asemănător, dar ia **toate înregistrările din a doua tabelă**.

Exemplu din PDF (salariații fără telefon au `TEL` și `Impuls` necompletate):

```sql
SELECT P.Nume, P.Adresa, P.Tel, T.Impuls FROM Pers P, Telefon T
WHERE P.Tel = T.Tel(+);
```

Scriere standard (din PDF):

```sql
SELECT E.Ename Nume, E.Sal Salar, D.Dname Nume_dept
FROM EMP E RIGHT OUTER JOIN DEPT D ON E.Deptno = D.Deptno AND sal < 1000;
```

### SELF JOIN

- Join-ul în cadrul **aceleiași tabele**: un câmp din tabelă referă alt câmp din aceeași tabelă.
- Se realizează **deschizând tabela de 2 ori cu alias-uri diferite**.
- Exemplu: fiecare salariat are codul șefului (`Mgr`), iar șeful e tot un salariat cu codul `Empno`. Condiția de join: `E.Mgr = S.Empno`.

```sql
-- scriere clasică
SELECT E.Ename Nume, E.Sal Salar, S.Ename Nume_sef, S.Sal Salar_sef
FROM EMP E, EMP S WHERE E.Mgr = S.Empno;

-- scriere standard (SELF INNER JOIN)
SELECT E.Ename Nume, E.Sal Salar, S.Ename Nume_sef, S.Sal Salar_sef
FROM EMP E INNER JOIN EMP S ON E.Mgr = S.Empno;
```

- **KING nu apare** în rezultat: e președinte și **nu are șef**.
- Cu **Left Outer Join** apar **toți** salariații, iar KING are datele șefului **goale**:

```sql
SELECT E.Ename Nume, E.Sal Salar, S.Ename Nume_sef, S.Sal Salar_sef
FROM EMP E, EMP S WHERE E.Mgr = S.Empno(+);
```

## 6. Vederi (VIEW)

- O vedere definește o **machetă** prin care se văd **numai anumite coloane** din tabelele bazei de date.
- Se definește printr-un `SELECT`, care poate conține unul sau mai multe `JOIN`-uri.
- Vederile sunt **tabele virtuale**, pentru care **nu se alocă spațiu**.
- `SELECT`-ul vederii se ține într-un rând din tabela `All_Views`, în coloana `Text` de tip `LONG`.

**La ce folosesc:**

- simplifică scrierea interogărilor complexe;
- permit **acordarea drepturilor de acces pe vedere**, nu pe tabele;
- permit definirea de **tabele virtuale** pentru aplicații.

```sql
CREATE VIEW nume_view (lista_coloane) AS
SELECT lista_col FROM tab1, tab2 WHERE cond [WITH READ ONLY];

CREATE VIEW Vemp (Nume, Salar, Departament) AS
SELECT E.Ename Nume, E.Sal Salar, D.Dname Nume_dept
FROM EMP E, DEPT D WHERE E.Deptno = D.Deptno;

SELECT * FROM Vemp WHERE salar > 1200;   -- afișează tabela virtuală, executând JOIN
```

### Afișarea vederilor existente

```sql
DESCR All_Views                       -- structura (OWNER, VIEW_NAME, TEXT_LENGTH, TEXT, ...)
SET LONG 20000                        -- lungimea afișată pentru coloane de tip LONG
SELECT Owner, View_Name, Text FROM All_Views WHERE View_Name = 'VEMP';
```

### Vederi simple vs complexe

||Vedere **simplă**|Vedere **complexă**|
|---|---|---|
|Definită pe|**o singură tabelă**|**JOIN** pe două sau mai multe tabele sau **operații de grup**|
|Funcții agregat / GROUP BY|**Nu**|Poate avea|
|Operații permise|**DML** (modificări și inserări) + interogări|**Doar interogări**|

- Adăugările și modificările făcute pe vedere se fac **fizic în tabela corespunzătoare**.

### Drepturi de acces pe vederi (ideile din PDF)

- Drepturile pentru grupuri de utilizatori se **recomandă pe vederi, nu pe tabele**.
- La proiectarea BD se **evită fragmentarea tabelelor** din motive de „proprietate" sau locație (prelucrările globale se complică). Există **o singură tabelă pentru fiecare entitate**.
- **Exemplul cu stocurile:** o singură tabelă `Stocuri`, câte o vedere `Magi` pentru fiecare magazie (acces nelimitat doar pentru gestionarul `Gesti`), iar contabilitatea primește o vedere `Cont_Stocuri`, **numai la citire**.
- **Exemplul cu studenții:** o singură tabelă `Studenți`, vederi pe facultăți sau secții. Secretara are drepturi complete pe secția ei, conducerea facultății citește din `Decan_Studenti`, rectoratul din `R_Studenti`, iar câmpurile publice (nume, adresă, tel, e-mail, data nașterii) se citesc din `D_Studenti`.

```sql
CREATE VIEW Stud_AC AS SELECT * FROM stud WHERE cods LIKE 'AC%';
GRANT SELECT, INSERT, UPDATE, DELETE ON Stud_AC TO secr_ac;
GRANT SELECT(Nume, Adresa, Tel) ON Stud TO Public;

SELECT * FROM stud_ac WHERE cods LIKE 'AC4%';   -- studenții din anul 4 AC
```

- Secretara de la AC are drepturi complete pe studenții de la AC.
- `Nume`, `Adresa`, `Tel` pot fi citite de oricine, dar **nu modificate**.

## Scăpări în PDF

> ℹ️ **Titlul greșit la RIGHT OUTER JOIN:** Sub exemplul cu `RIGHT OUTER JOIN`, PDF-ul scrie „Scrierea standard a unui **LEFT** OUTER JOIN", dar codul de dedesubt folosește `RIGHT OUTER JOIN`. Cel corect e RIGHT.

> ℹ️ **Exemplul cu Pers și Telefon:** Textul îl pune la RIGHT OUTER JOIN, dar interogarea are `(+)` pe `T.Tel`, adică păstrează toți din `Pers` (prima tabelă), deci se comportă ca un **LEFT** OUTER JOIN. Reține regula cu `(+)`, nu eticheta.

> ℹ️ **Ghilimele și formatări:** Ghilimelele din PDF (`‘Lugoj%’`, `’VEMP’`) sunt tipografice. În SQL se scriu ghilimele simple drepte `'...'`.

## Capcane de examen

> ⚠️ **DELETE vs TRUNCATE vs DROP:** `DELETE` șterge înregistrările și le **salvează în Rollback** (se poate face `ROLLBACK`). `TRUNCATE` șterge doar înregistrările, tabela rămâne goală. `DROP` șterge tabela și **nu salvează înregistrările în Rollback**.

> ⚠️ **COMMIT vs ROLLBACK:** `ROLLBACK` reface totul **după ultimul COMMIT**. După `COMMIT`, fișierul Rollback se șterge și modificările sunt **definitive**.

> ⚠️ **ANY vs ALL:** `ANY` = cel puțin o valoare îndeplinește condiția, `ALL` = toate. La „salariu > medie": cu `ALL` obții 4 salariați (JONES, SCOTT, KING, FORD), cu `ANY` obții 8.

> ⚠️ **Wildcard-uri LIKE:** `%` = orice număr de caractere, `_` = exact un caracter.

> ⚠️ **GROUP BY:** În lista de afișare apar **numai coloanele de grupare** și funcțiile agregat. Fără `GROUP BY`, funcțiile agregat se calculează pe **tot fișierul** și nu poți afișa nicio coloană simplă.

> ⚠️ **HAVING vs WHERE:** `HAVING` verifică **condiția de grup** (ex. `Avg(sal) > 1000`), nu condiția pe înregistrări individuale.

> ⚠️ **INNER vs LEFT OUTER:** `INNER JOIN` pierde salariații fără `Deptno` (popa, Popescu). `LEFT OUTER JOIN` îi păstrează, cu coloanele din a doua tabelă goale.

> ⚠️ **Semnul (+):** Se pune pe partea **tabelei incomplete**. `E.Deptno = D.Deptno(+)` păstrează toți salariații (LEFT OUTER), cu `D` incompletă.

> ⚠️ **SELF JOIN:** Aceeași tabelă de 2 ori, cu **alias-uri diferite** (`E` și `S`), condiția `E.Mgr = S.Empno`. KING (președinte, fără șef) dispare la INNER JOIN și apare cu date goale la LEFT OUTER JOIN.

> ⚠️ **Vedere simplă vs complexă:** Simplă = o tabelă, fără agregate sau GROUP BY, permite DML. Complexă (JOIN sau grup) = **doar interogări**.

## De reținut pe scurt

- **Oracle:** SQL Server (execută comenzile, singurul cu acces la date), SQL*Plus (interfața), PL/SQL (limbaj procedural). Conectare cu `SCOTT` / `TIGER`.
- **Comenzi:** `CREATE`, `DROP`, `INSERT`, `DELETE`, `UPDATE`, `TRUNCATE`, `COMMIT`, `ROLLBACK`.
- **Subselecturi:** `ANY` (cel puțin una) și `ALL` (toate).
- **Grupare:** `SUM`, `COUNT`, `AVG`, `MIN`, `MAX`, `GROUP BY`, `HAVING`.
- **Join-uri:** INNER (doar cu corespondent), LEFT OUTER (toate din prima), RIGHT OUTER (toate din a doua), SELF JOIN (aceeași tabelă cu 2 alias-uri), semnul `(+)`.
- **Vederi:** tabele virtuale, simplifică interogările, drepturi pe vedere nu pe tabelă, simple (DML) vs complexe (doar interogări).