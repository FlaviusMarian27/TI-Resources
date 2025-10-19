

# 🧠 Registers

### Registre generale
Vom lucra cu registrele de bază ale microprocesorului (AX, BX, CX, DX, etc.).

![AX Register](image/ax_register.png)


---

# 💾 Variables

### 🔹 Cum declarăm o variabilă?

```asm
nume_variabila DB valoare_initiala
nume_variabila DW valoare_initiala
nume_variabila DD valoare_initiala
nume_variabila DP valoare_initiala
```

### 🔹 Dimensiuni aferente pentru variabile

| Directive | Dimensiune | Tip de date |
| --------- | ---------- | ----------- |
| `DB`      | 8 biți     | Byte        |
| `DW`      | 16 biți    | Word        |
| `DD`      | 32 biți    | Double Word |
| `DP`      | 64 biți    | Quad Word   |
## 📘 Exemple de declarații în Assembly

### 🔹 Variabile simple

```asm
A  DB  7      ; variabilă pe 8 biți
A1 DB  3      ; tot 8 biți
```


### 🔹 Vector

```asm
A2 DW 7, 6, 4, 5 ; 
```

-  fiecare element din vector este pe 16 biti(2 octeti/ 2 bytes)
-  in memorie, ele sunt stocate secvențial pe câte 2 octeți fiecare
-  Reprezentare grafică a lui A2 în memorie

| Adresă | Conținut (DW) | Valoare |
| ------ | ------------- | ------- |
| 1000h  | A2[0]         | 7       |
| 1002h  | A2[1]         | 5       |
| 1004h  | A2[2]         | 6       |
| 1006h  | A2[3]         | 4       |


---
#  🔗 Constante

###### 💡 **Sintaxă generală**

```asm
nume_constanta EQU valoare
```

###### 📘 **Exemple**

```asm
PI       EQU 3.14
MAX_VAL  EQU 255
PORT_A   EQU 80H
```

###### ⚠️ **Diferență față de variabile:**

| Tip       | Declarație | Se alocă memorie? | Poate fi modificată? |
| --------- | ---------- | ----------------- | -------------------- |
| Variabilă | `A DB 7`   | ✅ Da              | ✅ Da                 |
| Constantă | `A EQU 7`  | ❌ Nu              | ❌ Nu                 |

---

# ⚙️ Instrucțiuni Assembly

## 🔹 Structura generală a unei instrucțiuni

	etichetă(opțional) nume_instrucțiune operanzi ; comentariu (opțional)

###### 📘 **Exemplu**

```asm
START: MOV AX, BX   ; copiază conținutul lui BX în AX
```


## 🧩 Instrucțiunea `MOV`

### 📘 Descriere

Instrucțiunea `MOV` copiază conținutul dintr-un operand sursă (**source**) într-un operand destinație (**destination**).

### 🔹 Sintaxă

```asm
MOV OP1, OP2
```

- **OP1** = destinația (unde se scrie)
- **OP2** = sursa (de unde se citește)
-  sensul este **`dest ← src`**

### ⚠️ Reguli importante

- Nu se pot muta **două variabile din memorie direct între ele** (nu `MOV var1, var2`)
- Trebuie să fie întotdeauna unul dintre operanzi **într-un registru**
- Dimensiunile operanzilor trebuie să fie **egale** (8 biți cu 8 biți, 16 cu 16)


### 📘 **Exemplu**

- data
```asm
A db 5
B db 7
```

- code
```asm
MOV AH A
MOV AL B
MOV A AL
MOV B AH
```

![AX Register](image/ax_memory.png)
