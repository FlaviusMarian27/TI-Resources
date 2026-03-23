### 3. Configurarea Porturilor pentru VLAN-uri

Acum asociem fiecare port fizic cu VLAN-ul corespunzător. PC1 și PC2 → VLAN 10, PC3 și PC4 → VLAN 20:

```
interface fa0/1
switchport mode access
switchport access vlan 10
no shutdown

interface fa0/2
switchport mode access
switchport access vlan 10
no shutdown

interface fa0/3
switchport mode access
switchport access vlan 20
no shutdown

interface fa0/4
switchport mode access
switchport access vlan 20
no shutdown
```

| Comandă | Explicație |
|---------|------------|
| `interface fa0/X` | Selectează portul fizic de configurat |
| `switchport mode access` | Forțează portul în modul access (un singur VLAN) |
| `switchport access vlan X` | Introduce portul în VLAN-ul specificat |
| `no shutdown` | Activează portul (îl pornește dacă era oprit) |

---

### 4. Testarea Conectivității (Ping)

Deschideți **Command Prompt** pe PC1 și rulați:

**Test intern — trebuie să REUȘEASCĂ:**
```
ping 192.168.10.3
```
✅ Răspuns așteptat: `Reply from 192.168.10.3` → PC1 și PC2 comunică (același VLAN 10).

**Test de izolare — trebuie să EȘUEZE:**
```
ping 192.168.20.2
```
✅ Răspuns așteptat: `Request timed out` → switch-ul blochează traficul între VLAN 10 și VLAN 20.

---

### 5. Salvarea Configurației

```
copy running-config startup-config
```

Copiază configurația din **RAM** (temporară) în **NVRAM** (persistentă). Fără acest pas, configurația se pierde la repornire.

---
---

## Laborator 3 — Inter-VLAN Routing (Router-on-a-Stick)

### Ce am învățat în Lab 2 și de ce avem nevoie de mai mult?

În laboratorul anterior, PC-urile din **același VLAN** comunicau, dar PC-urile din **VLAN-uri diferite** nu puteau comunica (ex: PC1 din VLAN 10 nu putea face ping la PC3 din VLAN 20).

**De ce?** VLAN 10 și VLAN 20 sunt rețele separate, iar un switch simplu de Layer 2 nu face rutare între ele.

**Soluția:** Adăugăm un **router**. El va primi traficul de la switch și va ști:
- pentru VLAN 10 → folosește gateway-ul `192.168.10.1`
- pentru VLAN 20 → folosește gateway-ul `192.168.20.1`

Metoda se numește **Router-on-a-Stick** deoarece folosim un **singur cablu** între switch și router, pe care circulă mai multe VLAN-uri prin trunking.

> 💡 **Modurile unui port de switch:**
> - **Access** → pentru un singur VLAN (de obicei conectat la un PC)
> - **Trunk** → pentru mai multe VLAN-uri simultan (de obicei între switch și router, sau între două switch-uri)

### Scopul Laboratorului

- Înțelegerea de ce VLAN-urile diferite nu comunică direct
- Configurarea unui trunk între switch și router
- Crearea de subinterfețe pe router pentru fiecare VLAN
- Setarea routerului ca gateway pentru VLAN-uri
- Testarea comunicării între VLAN-uri

---

### 1. Topologie și Adrese IP

**Echipamente:** 1 Switch, 1 Router (ex: model 1941), 4 PC-uri

**Conexiuni fizice:**

| Echipament | Port   | Conectat la | Port  |
|------------|--------|-------------|-------|
| PC1        | —      | Switch      | Fa0/1 |
| PC2        | —      | Switch      | Fa0/2 |
| PC3        | —      | Switch      | Fa0/3 |
| PC4        | —      | Switch      | Fa0/4 |
| Switch     | Fa0/24 | Router      | G0/0  |

**Schema logică (adrese IP cu Default Gateway):**

| VLAN    | PC  | Adresă IP       | Default Gateway |
|---------|-----|-----------------|-----------------|
| 10 (IT) | PC1 | 192.168.10.2/24 | 192.168.10.1    |
| 10 (IT) | PC2 | 192.168.10.3/24 | 192.168.10.1    |
| 20 (HR) | PC3 | 192.168.20.2/24 | 192.168.20.1    |
| 20 (HR) | PC4 | 192.168.20.3/24 | 192.168.20.1    |

> 💡 **Ce este Default Gateway?**
> Un PC trimite traficul astfel:
> - dacă destinația este în **aceeași rețea** → trimite direct
> - dacă destinația este în **altă rețea** → trimite la default gateway (router)
>
> Exemplu: PC1 are IP `192.168.10.2` → gateway-ul lui este `192.168.10.1`.
> Când PC1 vrea să ajungă la `192.168.20.2`, trimite pachetul către router.

---

### 2. Configurarea PC-urilor

Pe fiecare PC: **Desktop → IP Configuration**

Adăugați **IP Address**, **Subnet Mask** și **Default Gateway** conform tabelului de mai sus.

---

### 3. Accesarea CLI pe Switch

Click pe **Switch → CLI**:

```
enable
configure terminal
```

---

### 4. Configurarea VLAN-urilor pe Switch

Creăm VLAN 10 (IT) și VLAN 20 (HR):

```
vlan 10
name IT
exit

vlan 20
name HR
exit
```

**Verificare:**
```
show vlan brief
```

---

### 5. Atribuirea Porturilor către VLAN-uri

PC1 și PC2 → VLAN 10 | PC3 și PC4 → VLAN 20:

```
interface range fa0/1-2
switchport mode access
switchport access vlan 10
no shutdown
exit

interface range fa0/3-4
switchport mode access
switchport access vlan 20
no shutdown
exit
```

> 💡 Comanda `interface range fa0/1-2` configurează mai multe porturi simultan. Alternativ, se poate folosi `interface fa0/1`, `interface fa0/2` etc. separat.

---

### 6. Configurarea Trunk-ului pe Switch

Portul **Fa0/24** al switch-ului este conectat la router și trebuie setat în modul **trunk**, pentru a transporta traficul mai multor VLAN-uri simultan:

```
interface fa0/24
switchport mode trunk
exit
```

**Verificare trunk:**
```
show interfaces trunk
```

Rezultatul arată portul `Fa0/24` cu encapsulation `802.1q`, status `trunking`, și VLAN-urile active (1, 10, 20).

---

### 7. Configurarea Routerului — Activarea Interfeței Fizice

Click pe **Router → CLI**:

```
enable
configure terminal
interface g0/0
no shutdown
exit
```

> ⚠️ Pe interfața fizică **g0/0** nu punem adresă IP direct — vom folosi subinterfețele.

Pe router avem interfața fizică `g0/0`, dar vrem să o folosim pentru mai multe VLAN-uri. Atunci o împărțim **logic** în două subinterfețe:
- `g0/0.10` → pentru VLAN 10
- `g0/0.20` → pentru VLAN 20

Acestea sunt interfețe **virtuale/logice** create peste aceeași interfață fizică.

---

### 8. Configurarea Subinterfețelor pentru VLAN-uri

```
interface g0/0.10
encapsulation dot1Q 10
ip address 192.168.10.1 255.255.255.0
exit

interface g0/0.20
encapsulation dot1Q 20
ip address 192.168.20.1 255.255.255.0
exit
```

| Comandă | Explicație |
|---------|------------|
| `interface g0/0.10` | Creează subinterfața 10 pe interfața fizică g0/0 |
| `encapsulation dot1Q 10` | Instruiește subinterfața să proceseze cadre etichetate 802.1Q pentru VLAN 10 |
| `ip address 192.168.10.1 ...` | Setează IP-ul care va fi **gateway** pentru toate PC-urile din VLAN 10 |

**Verificare** (din modul privilegiat):
```
show ip interface brief
```

Ar trebui să vedeți `GigabitEthernet0/0.10` cu IP `192.168.10.1` și `GigabitEthernet0/0.20` cu IP `192.168.20.1`, ambele cu status `up`.

---

### 9. Salvarea Configurației

Pe **ambele echipamente** (Switch și Router), din modul privilegiat:

```
copy running-config startup-config
```

---

### 10. Testarea Finală

Deschideți **Command Prompt** pe PC1 (VLAN 10) și rulați:

```
ping 192.168.20.2
```

✅ **Rezultat așteptat:** `Reply from 192.168.20.2`

Spre deosebire de Lab 2, acum ping-ul **reușește** deoarece:
1. PC1 vede că `192.168.20.2` este în altă rețea → trimite pachetul la gateway (`192.168.10.1`)
2. Pachetul ajunge la Switch, care îl trimite prin conexiunea **Trunk** la Router
3. Routerul primește pachetul pe subinterfața `g0/0.10` și îl rutează spre subinterfața `g0/0.20`
4. Pachetul este trimis înapoi prin Switch în rețeaua VLAN 20, ajungând la PC3