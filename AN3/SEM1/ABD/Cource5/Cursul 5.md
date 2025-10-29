
## 🔹Ce este o tranzacție

-  O **tranzacție** este o unitate logică de lucru (logical unit of work) formată dintr-un set de operații SQL care se execută împreună.  
-  Fie toate reușesc, fie toate sunt anulate.

```sql 
START TRANSACTION;

UPDATE accounts SET balance = balance - 100 WHERE account_id = 1;
UPDATE accounts SET balance = balance + 100 WHERE account_id = 2;

COMMIT;
-- Dacă apare o eroare, se poate face ROLLBACK;
```

- Dacă se întrerupe curentul **înainte de COMMIT**, MySQL revine automat la starea anterioară (rollback implicit, prin jurnal intern).

---
# 🔹Proprietățile ACID

| Proprietate     | Ce înseamnă                                    | Explicație clară                                                                                                                                |
| --------------- | ---------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| **Atomicity**   | „Totul sau nimic”                              | Fie toate operațiile din tranzacție sunt efectuate, fie niciuna. Dacă una eșuează → ROLLBACK.                                                   |
| **Consistency** | Starea bazei rămâne validă                     | Toate regulile (chei primare, străine, constrângeri, trigger-e) trebuie respectate. O tranzacție nu poate lăsa baza într-o stare inconsistentă. |
| **Isolation**   | Tranzacțiile simultane nu se încurcă între ele | Rezultatul final trebuie să fie ca și cum tranzacțiile ar fi rulate pe rând. Se controlează prin nivelurile de izolare.                         |
| **Durability**  | După COMMIT, datele nu se pierd                | Chiar dacă se oprește serverul sau pică curentul, datele comise rămân salvate (datorită redo log, binlog, flush la disk).                       |

---

# 🔹Niveluri de izolare(MySQL -InnoDB)

| Nivel                                 | Dirty Read | Non-Repeatable Read | Phantom Read                      | Observații                           |
| ------------------------------------- | ---------- | ------------------- | --------------------------------- | ------------------------------------ |
| READ UNCOMMITTED                      | Da         | Da                  | Da                                | Cel mai rapid, dar riscant           |
| READ COMMITTED                        | Nu         | Da                  | Da                                | Fiecare SELECT vede doar date comise |
| REPEATABLE READ _(implicit în MySQL)_ | Nu         | Nu                  | Nu (InnoDB previne phantom reads) | Recomandat                           |
| SERIALIZABLE                          | Nu         | Nu                  | Nu                                | Cel mai sigur, dar blochează mult    |
- În tranzacții critice se folosește `SELECT ... FOR UPDATE` pentru a bloca rândurile citite.

---
# 🔹Motor de stocare: InnoDB vs MyISAM

- **InnoDB** – oferă tranzacții, integritate referențială, ACID complet.
- **MyISAM** – _nu_ suportă tranzacții; folosește blocări pe tabel, nu e ACID-compliant.

---
## 🔹 Binary Logs (binlog)
-  Se înregistrează toate tranzacțiile comise.
-  Permite:    
    -  **replicare** (slave-uri pot reface aceleași tranzacții),
    -  **point-in-time recovery** (revenire la un moment anterior),
    -  **auditare**.

---
## 🔹 Durability și setări importante

```sql
SHOW VARIABLES LIKE 'innodb_flush_log_at_trx_commit';
```

Valori:
- `1` = cel mai sigur (flush la fiecare COMMIT)
- `2` = flush în cache OS
- `0` = flush o dată pe secundă (rapid, dar risc de pierdere)

```sql
SHOW VARIABLES LIKE 'sync_binlog';
-- 1 = sincronizează binlog-ul la fiecare COMMIT (maximă siguranță)
```

## 🔹 Best Practices (Tips & Tricks)

1. **Tranzacțiile cât mai scurte** – nu bloca resurse inutil.
2. **Rollback doar când e nevoie** – altfel performanța scade.
3. **Validări în DB (triggers, constraints)** – mai rapid și sigur decât în cod.
4. **Folosește `SELECT ... FOR UPDATE`** când vrei să blochezi rânduri pentru modificare.
5. **Evită input de la utilizator în timpul unei tranzacții**.
6. **Ai grijă la DDL** – `ALTER`, `DROP`, `CREATE` fac _implicit COMMIT_!
7. **Monitorizează tranzacțiile active:**

```sql
SELECT * FROM information_schema.innodb_trx;
```

1. **Evită autocommit în operații complexe:**

```SQL
SET autocommit = 0;
```