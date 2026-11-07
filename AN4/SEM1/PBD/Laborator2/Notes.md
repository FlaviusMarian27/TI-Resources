## Ce este modelarea E-R

- Tip de modelare **intuitivă, de nivel înalt**.
- Corespunde **proiectării de tip conceptual** (etapă din procesul de proiectare al bazelor de date).
- Înseamnă reprezentarea **vizuală** a structurii bazei de date printr-o **Diagramă Entitate-Relație (ER Diagram)**.
- Rolul diagramei: definește **datele ce vor fi stocate** și **interdependențele dintre ele**. E ușor de înțeles.

## Componentele de bază

|Componentă|Ce este|Reprezentare|
|---|---|---|
|**Entitate** (Entity)|Set de date modelat ca obiect independent: persoană, loc, eveniment, obiect reprezentativ pentru un sistem|**Dreptunghi**|
|**Atribut** (Attribute)|Proprietățile / caracteristicile unei entități|**Oval**|
|**Relație** (Relationship)|Conexiunea dintre două sau mai multe entități|**Romb**|

## Componentele derivate

|Componentă|Definiție|Reprezentare|Exemplu din lab|
|---|---|---|---|
|**Entitate slabă** (Weak Entity)|Nu se poate identifica unic prin atribute proprii, se bazează pe relația cu alte entități|**Dreptunghi dublu**|Cont bancar (nu poate fi identificat fără să știi banca de care aparține)|
|**Atribut cheie** (Key Attribute)|Definește în mod unic o entitate|Text **subliniat**|Nr. matricol (Student)|
|**Atribut compus** (Composite)|Combinație de mai multe alte atribute|-|Adresa (țara, strada, numărul etc.)|
|**Atribut cu valoare multiplă** (Multivalued)|Poate conține mai multe valori|**Oval dublu**|Nr. telefon (un student poate avea mai multe numere)|
|**Atribut derivat** (Derived)|Are valoare dinamică, derivă dintr-un alt atribut|**Oval punctat**|Vârsta (derivată din data nașterii, își modifică valoarea în timp)|

## Tipuri de relații

|Tip|Când se folosește|Exemplu din lab|Cardinalitate pe diagramă|
|---|---|---|---|
|**One to One** (1:1)|O instanță a unei entități e asociată cu o singură instanță a altei entități|O persoană poate avea un singur pașaport|Persoană **1** - are - **1** Pașaport|
|**One to Many** (1:N)|O instanță a unei entități e asociată cu mai multe instanțe ale altei entități|Un client poate plasa mai multe comenzi|Client **1** - plasează - **N** Comandă|
|**Many to One** (N:1)|Inversul lui One to Many: mai multe instanțe ale unei entități sunt asociate cu o singură instanță a altei entități|Mai mulți studenți pot fi înrolați la un singur curs|Student **N** - înrolat - **1** Curs|
|**Many to Many** (N:N)|Mai multe instanțe ale unei entități sunt asociate cu mai multe instanțe ale altei entități|Un angajat poate fi asociat mai multor proiecte, iar un proiect mai multor angajați|Angajat **N** - e asociat - **N** Proiect|

## Reprezentare grafică

[Componentele diagramei E-R](https://claude.ai/chat/er-componente.svg)

## Capcane de examen

> [!WARNING] **Entitate slabă vs atribut multivaloare** Ambele se desenează "dublu", dar sunt lucruri diferite: entitatea slabă = **dreptunghi dublu**, atributul cu valoare multiplă = **oval dublu**.

> [!WARNING] **Dublu, punctat, subliniat**
> 
> - Multivaloare: oval **dublu**
> - Derivat: oval **punctat**
> - Cheie: **subliniat**

> [!WARNING] **Ce tip de atribut e fiecare exemplu**
> 
> - Vârsta = **derivat** (din data nașterii), nu atribut simplu.
> - Nr. telefon = **multivaloare**.
> - Adresa = **compus**.
> - Nr. matricol = **cheie**.

> [!WARNING] **Entitatea slabă** Se identifică **doar prin relația cu altă entitate** (Cont bancar depinde de Bancă).

> [!WARNING] **N:1 este inversul lui 1:N** Exemplele din lab sunt diferite: **Client-Comandă** pentru 1:N, **Student-Curs** pentru N:1. Nu le amesteca.

> [!WARNING] **Reprezentare vs semnificație** Entitate = dreptunghi, atribut = oval, relație = romb. Cardinalitatea (1 / N) se scrie pe linia dintre entitate și romb.

## De reținut pe scurt

- Modelarea E-R = proiectare **conceptuală**, rezultatul e **diagrama ER**.
- 3 componente de bază: **entitate, atribut, relație**.
- Componente derivate: entitate slabă, atribut cheie, compus, multivaloare, derivat și cele 4 tipuri de relații (1:1, 1:N, N:1, N:N).
- Învață fiecare componentă împreună cu **forma ei grafică** și **exemplul ei** din lab.

---

# Plus (nu e în PDF)

> [!NOTE] Tot ce e mai jos e din **cunoștințe generale**, nu din PDF-ul de lab. Verifică definițiile cu ce a zis profu la curs.

[Entitate tare și capcanele de modelare](https://claude.ai/chat/er-plus.svg)

## Entitate tare (strong entity)

- Opusul entității slabe: se **identifică singură**, prin **atribut cheie propriu** (ex. `id_banca` pentru Bancă).
- Se desenează cu **dreptunghi simplu**.
- Entitatea slabă (dreptunghi dublu) are nevoie de o entitate tare ca să fie identificată (Cont bancar → Bancă).

## Capcane de modelare (connection traps)

Apar când diagrama sugerează o legătură între două entități, dar **nu poți afla** corect legătura dintre instanțe.

### Capcana evantai (fan trap)

- **Când apare**: o entitate are **două relații 1:N** care pleacă din ea, ca un evantai (Grup → Companie și Grup → Angajat).
- **Problema**: între cele două entități de la capete (Companie și Angajat) **nu există cale**. Știi grupul angajatului, dar nu și compania lui.
- **Cum o repari**: pui entitățile **în lanț**: Grup 1:N Companie 1:N Angajat. Grupul îl afli prin companie.

### Capcana prăpastie (chasm trap)

- **Când apare**: calea între entități există în desen, dar este **întreruptă pentru unele instanțe**, din cauza **participării opționale** într-o relație.
- **Exemplu**: Companie 1:N Angajat, Angajat 1:N Fabrică (gestionează). O fabrică **fără angajat responsabil** nu are cale spre companie.
- **Cum o repari**: adaugi o **relație directă** între capete (Companie 1:N Fabrică).

> [!WARNING] **Capcane de examen**
> 
> - **Evantai**: cele două relații pleacă din **aceeași entitate** și calea între capete **lipsește complet**.
> - **Prăpastie**: calea **există**, dar nu e completă pentru toate instanțele (participare opțională).
> - La ambele, soluția e să **restructurezi** diagrama (lanț sau relație directă).
> - Dacă profu le numește „capcana abisului" sau „încrucișată", verifică la ce se referă: eu le-am pus ca **chasm trap** și **fan trap**.