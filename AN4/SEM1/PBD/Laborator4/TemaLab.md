## Cerință

Normalizați baza de date a unei companii de E-Commerce (Produse, Categorii, Producători, Comenzi, Detalii Comandă, Clienți). **Identificați dependențele funcționale și eliminați-le în mod corespunzător**, astfel încât schemele rezultate să respecte **3NF** și **4NF**.

## Regulile folosite (din laboratoarele 2 și 3)

- **1NF**: fiecare celulă are **o singură valoare** și fiecare rând e **unic**.
- **2NF**: în 1NF și **fiecare atribut non-cheie depinde total de cheia primară**. Se verifică **doar la chei compuse**.
- **3NF**: în 1NF și 2NF și **fără dependență tranzitivă** (A → B și B → C, cu B și C non-cheie). Se elimină mutând atributele dependente tranzitiv într-o relație nouă, **cu o copie a determinantului**.
- **4NF**: în 1NF, 2NF, 3NF și **fără dependențe multivalorice** (A ↠ B și A ↠ C, cu B și C **independente**). Se elimină punând atributele în relații separate, **cu o copie a determinantului**.

> ℹ️ Datele din tabele sunt **inventate**, ca exemple (5 înregistrări pe tabel).

## 1. Produse (`id_produs`, `nume_produs`, `descriere`, `pret_unitar`, `categorie_id`, `producator_id`)

|id_produs|nume_produs|descriere|pret_unitar|categorie_id|producator_id|
|---|---|---|---|---|---|
|1|Laptop X1|15 inch, 16 GB RAM|3500|1|1|
|2|Telefon S5|128 GB|2000|2|2|
|3|Mouse M2|Wireless|80|1|3|
|4|Căști B7|Bluetooth|250|2|4|
|5|Tastatură K3|Mecanică|300|1|3|

- **Dependențe funcționale**: `id_produs` → `nume_produs`, `descriere`, `pret_unitar`, `categorie_id`, `producator_id`.
- **1NF**: ok, o valoare pe celulă, rânduri unice.
- **2NF**: ok, cheia are **un singur atribut**, deci nu pot exista dependențe parțiale.
- **3NF**: ok. `categorie_id` și `producator_id` sunt chei străine; numele categoriei și al producătorului stau în **tabelele lor**, deci nu există lanțuri de tipul `id_produs → categorie_id → nume_categorie` în acest tabel.
- **4NF**: ok, nu există atribute multivalorice.

## 2. Categorii (`id_categorie`, `nume_categorie`, `descriere_categorie`)

|id_categorie|nume_categorie|descriere_categorie|
|---|---|---|
|1|Calculatoare|Laptopuri și periferice|
|2|Telefoane|Telefoane și accesorii audio|
|3|Electrocasnice|Aparate pentru casă|
|4|Cărți|Cărți tipărite|
|5|Jocuri|Jocuri video și de societate|

- **Dependențe funcționale**: `id_categorie` → `nume_categorie`, `descriere_categorie`.
- **1NF**: ok.
- **2NF**: ok, cheie simplă.
- **3NF**: ok, `descriere_categorie` nu depinde de `nume_categorie`, ci direct de `id_categorie`.
- **4NF**: ok.

## 3. Producători (`id_producator`, `nume_producator`, `tara_origine`)

|id_producator|nume_producator|tara_origine|
|---|---|---|
|1|TechCorp|Germania|
|2|MobiTech|Coreea de Sud|
|3|PeriPlus|Taiwan|
|4|SoundMax|Japonia|
|5|HomeStar|Italia|

- **Dependențe funcționale**: `id_producator` → `nume_producator`, `tara_origine`.
- **1NF**: ok.
- **2NF**: ok, cheie simplă.
- **3NF**: ok. Țara de origine e a producătorului, deci depinde direct de `id_producator`.
- **4NF**: ok.

## 4. Comenzi (`id_comanda`, `data_comenzii`, `client_id`)

|id_comanda|data_comenzii|client_id|
|---|---|---|
|100|2026-10-01|1|
|101|2026-10-03|2|
|102|2026-10-05|1|
|103|2026-10-10|3|
|104|2026-10-15|4|

- **Dependențe funcționale**: `id_comanda` → `data_comenzii`, `client_id`.
- **1NF**: ok.
- **2NF**: ok, cheie simplă.
- **3NF**: ok. Datele clientului (nume, adresă etc.) stau în Clienți, nu în Comenzi, deci nu există `id_comanda → client_id → nume_client` în același tabel.
- **4NF**: ok.

## 5. Detalii Comandă (`id_detaliu_comanda`, `id_comanda`, `id_produs`, `cantitate`, `pret_total`)

|id_detaliu_comanda|id_comanda|id_produs|cantitate|pret_total|
|---|---|---|---|---|
|1|100|3|2|160|
|2|100|5|1|300|
|3|101|4|1|250|
|4|102|3|2|120|
|5|103|1|1|3500|

- **Dependențe funcționale**: `id_detaliu_comanda` → `id_comanda`, `id_produs`, `cantitate`, `pret_total`.
- **1NF**: ok.
- **2NF**: ok, cheie simplă.
- **3NF**: ok, **`pret_total` rămâne**. Prima impresie: `pret_total = cantitate × pret_unitar`, iar `pret_unitar` e în Produse, deci ar fi dependență tranzitivă. Dar prețul din comandă e **prețul de la momentul comenzii** (ofertă). Uite rândurile 1 și 4: același `id_produs` = 3 și aceeași `cantitate` = 2, dar `pret_total` diferă (160 față de 120, comanda 102 a prins oferta). Deci `{id_produs, cantitate}` **nu determină** `pret_total`, nu avem lanț A → B → C și nu avem dependență tranzitivă (așa a zis și profu).
- **4NF**: ok.

> ⚠️ **Nu șterge `pret_total`:** L-ai pierde pe cel real plătit la acea comandă. Păstrat, nu încalcă 3NF, pentru că nu se poate deduce din Produse.

## 6. Clienți (`id_client`, `nume_client`, `adresa`, `email`, `telefon`)

> ℹ️ **Presupunere:** un client are **o singură adresă**, dar poate avea **mai multe e-mailuri și mai multe numere de telefon**. Dacă profu spune că are câte unul din fiecare, Clienți e deja în 1NF-4NF și nu mai faci pașii de mai jos.

**Tabelul inițial (nenormalizat):**

|id_client|nume_client|adresa|email|telefon|
|---|---|---|---|---|
|1|Ana Pop|Str. Libertății 12|ana@mail.com; ana@work.ro|0722111111; 0733222222|
|2|Dan Rus|Str. Școlii 3|dan@mail.com|0744333333|
|3|Eva Man|Str. Fabricii 25|eva@mail.com|0755444444; 0766555555|
|4|Radu Ene|Bd. Revoluției 5|radu@mail.com; radu@work.ro|0777666666|
|5|Ioana Tudor|Str. Mihai Viteazu 8|ioana@mail.com|0788777777|

- **Dependențe funcționale**: `id_client` → `nume_client`, `adresa`; `email` și `telefon` sunt multivalorice.

### 1NF: nu e în 1NF

La clienții 1, 3 și 4, `email` și/sau `telefon` au **mai multe valori într-o celulă** (ca `STUD_PHONE` din laboratorul 2). Punem câte o valoare pe rând, pentru toate combinațiile:

|id_client|nume_client|adresa|email|telefon|
|---|---|---|---|---|
|1|Ana Pop|Str. Libertății 12|ana@mail.com|0722111111|
|1|Ana Pop|Str. Libertății 12|ana@mail.com|0733222222|
|1|Ana Pop|Str. Libertății 12|ana@work.ro|0722111111|
|1|Ana Pop|Str. Libertății 12|ana@work.ro|0733222222|
|2|Dan Rus|Str. Școlii 3|dan@mail.com|0744333333|
|3|Eva Man|Str. Fabricii 25|eva@mail.com|0755444444|
|3|Eva Man|Str. Fabricii 25|eva@mail.com|0766555555|
|4|Radu Ene|Bd. Revoluției 5|radu@mail.com|0777666666|
|4|Radu Ene|Bd. Revoluției 5|radu@work.ro|0777666666|
|5|Ioana Tudor|Str. Mihai Viteazu 8|ioana@mail.com|0788777777|

`id_client` nu mai e unic, deci **cheia primară devine compusă**: `{id_client, email, telefon}`.

### 2NF: nu e în 2NF

`nume_client` și `adresa` depind doar de `id_client`, adică de **o parte din cheie**: **dependențe parțiale**, ca `COURSE_NO → COURSE_FEE` din lab. Semnul vizibil: „Ana Pop" și adresa ei apar de 4 ori. Se descompune în 2 tabele:

_Clienți_ (cheie: `id_client`)

|id_client|nume_client|adresa|
|---|---|---|
|1|Ana Pop|Str. Libertății 12|
|2|Dan Rus|Str. Școlii 3|
|3|Eva Man|Str. Fabricii 25|
|4|Radu Ene|Bd. Revoluției 5|
|5|Ioana Tudor|Str. Mihai Viteazu 8|

_Contact_Client_ (cheie: `{id_client, email, telefon}`): cele 10 rânduri din tabelul 1NF, **fără** `nume_client` și `adresa` (rămân doar `id_client`, `email`, `telefon`).

### 3NF: ok

- **Clienți**: `nume_client` și `adresa` depind direct de `id_client`, fără lanț între ele.
- **Contact_Client**: toate atributele fac parte din cheie, deci nu există atribute non-cheie și nici dependențe tranzitive.

### 4NF: nu e în 4NF

În **Contact_Client** avem două atribute multivalorice **independente** pentru același client:

- `id_client ↠ email`
- `id_client ↠ telefon`

E-mailurile nu depind de telefoane, de aceea s-au repetat **toate combinațiile** (4 rânduri pentru Ana, 2 pentru Radu), ca la exemplul cu pizza. Tabelul trece de 1NF-3NF (nu are atribute non-cheie), dar are redundanță. Soluția: **câte o relație pentru fiecare**, cu copia determinantului (`id_client`):

_Emailuri_Client_ (cheie: `{id_client, email}`)

|id_client|email|
|---|---|
|1|ana@mail.com|
|1|ana@work.ro|
|2|dan@mail.com|
|3|eva@mail.com|
|4|radu@mail.com|
|4|radu@work.ro|
|5|ioana@mail.com|

_Telefoane_Client_ (cheie: `{id_client, telefon}`)

|id_client|telefon|
|---|---|
|1|0722111111|
|1|0733222222|
|2|0744333333|
|3|0755444444|
|3|0766555555|
|4|0777666666|
|5|0788777777|

> ℹ️ **Scurtătură:** poți separa direct la 1NF în Clienți + Emailuri_Client + Telefoane_Client, fără pasul intermediar Contact_Client. Rezultatul final e același.

## Schema finală (3NF și 4NF)

|Tabel|Cheie primară|Alte atribute / chei străine|
|---|---|---|
|**Produse**|`id_produs`|`nume_produs`, `descriere`, `pret_unitar`, `categorie_id` (FK), `producator_id` (FK)|
|**Categorii**|`id_categorie`|`nume_categorie`, `descriere_categorie`|
|**Producători**|`id_producator`|`nume_producator`, `tara_origine`|
|**Comenzi**|`id_comanda`|`data_comenzii`, `client_id` (FK)|
|**Detalii_Comandă**|`id_detaliu_comanda`|`id_comanda` (FK), `id_produs` (FK), `cantitate`, `pret_total`|
|**Clienți**|`id_client`|`nume_client`, `adresa`|
|**Emailuri_Client**|`{id_client, email}`|`id_client` (FK)|
|**Telefoane_Client**|`{id_client, telefon}`|`id_client` (FK)|

## Ce spui la predare

- **Produse, Categorii, Producători, Comenzi, Detalii Comandă**: erau deja în 1NF-4NF (cheie simplă, o valoare pe celulă, fără lanțuri tranzitive sau multivalorice).
- **`pret_total`** rămâne, pentru că reține prețul de la momentul comenzii (ofertă) și nu se poate deduce din `pret_unitar`.
- **Clienți**: a picat la **1NF** (e-mailuri și telefoane multiple), la **2NF** (dependențe parțiale după ce cheia a devenit compusă) și la **4NF** (e-mailurile și telefoanele sunt multivalorice și independente). S-a descompus în Clienți, Emailuri_Client și Telefoane_Client.

## Capcane

> ⚠️ **Ordinea formelor:** 1NF → 2NF → 3NF → 4NF. 2NF apare abia după ce cheia a devenit compusă (la Clienți, după 1NF).

> ⚠️ **Tranzitivă înseamnă lanț prin non-chei:** A → B → C cu B și C non-cheie. Chei străine ca `categorie_id` nu creează dependențe tranzitive în Produse.

> ⚠️ **4NF cere două sau mai multe atribute multivalorice independente:** Un singur atribut multivaloric e doar o problemă de 1NF.

> ⚠️ **Copia determinantului:** Fiecare tabel nou de la 4NF păstrează `id_client`, altfel nu mai poți lega datele de client.