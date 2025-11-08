Avem 3 circuite cu capacități diferite și adrese diferite:

|Circuit|Adresă de start|Capacitate|
|---|---|---|
|C1|00000H|64K|
|C2|40000H|32K|
|C3|60000H|16K|

Să se determine maparea adreselor circuitelor și expresiile de selectare (SEL).

---

## 2️⃣ Rezolvare

Pentru a afla câți biți sunt necesari pentru adresare:

`64K  = 2^16 → A0...A15`
`32K  = 2^15 → A0...A14`

Adresarea completă a sistemului merge până la A19 (deoarece avem 1 MB → 2^20 adrese).

---

## 3️⃣ Determinarea zonelor de adrese

|Circuit|Adresă start|Adresă stop|
|---|---|---|
|C1|00000H|0FFFFH|
|C2|40000H|47FFFH|
|C3|70000H|77FFFH|

Pentru fiecare zonă luăm biții A19...A16, deoarece fiecare cifră hex reprezintă **4 biți (1 nibble)**.  
→ 5 cifre hex * 4 biți = **20 biți de adresă totală (A0...A19)**

---

## 4️⃣ Tabelul de mapare (A19...A16)

| Circuit | A19 | A18 | A17 | A16 |
| ------- | --- | --- | --- | --- |
| C1      | 0   | 0   | 0   | 0   |
| C2      | 0   | 1   | 0   | 0   |
| C3      | 0   | 1   | 1   | 1   |

---

## 5️⃣ Ecuațiile de selecție

Dacă semnalul de activare (CE) este **activ-high**, ecuațiile sunt:

`SEL_C1 = ¬A19 · ¬A18 · ¬A17 · ¬A16 
`SEL_C2 = ¬A19 ·  A18 · ¬A17 · ¬A16`
`SEL_C3 = ¬A19 ·  A18 ·  A17 ·  A16`

Dacă semnalul CE este **activ-low**, se inversează toate ecuațiile de mai sus.

![[Decouder.png]]