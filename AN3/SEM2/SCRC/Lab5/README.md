# 🖧 Laborator 5 — Rutare Dinamică (OSPF)

> **Cisco Packet Tracer** · Rețele de Calculatoare

Spre deosebire de rutarea statică, OSPF permite routerelor să își descopere singure rețelele vecine și să construiască automat tabela de rutare.

---

## 📌 Scopul Laboratorului

- Configurarea protocolului **OSPF** (Open Shortest Path First)
- Înțelegerea diferenței față de rutarea statică (`ip route`)
- Observarea **vecinilor OSPF**, a redundanței și a **convergenței**
- Demonstrarea reroutării automate când un link cade

> 💬 Spre deosebire de RIP (care numără doar hop-uri), OSPF calculează un **cost** al legăturii bazat pe lățimea de bandă. Dacă un link cade, OSPF găsește automat un alt drum.

---

## 🗺️ Topologie

```
        PC1
         |
        SW1
         |
        R1
       /    \
   serial   serial
     /          \
   R2            R3
     \          /
   serial   serial
       \    /
        (legătură directă R2-R3)
        |              |
       SW2            SW3
        |              |
       PC2            PC3
```

Topologie **triunghi** — fiecare router este conectat la ceilalți doi, oferind redundanță.

---

## 🔧 Pasul 1 — Echipamente și Modulul Serial

### Dispozitive necesare

| Dispozitiv | Model | Cantitate |
|---|---|---|
| Router | Cisco 1941 | 3 |
| Switch | Cisco 2960-24TT | 3 |
| PC | PC-PT | 3 |

### Adaugarea modulului HWIC-2T

Fiecare router are nevoie de **2 porturi seriale** (câte unul pentru fiecare legătură cu celelalte routere). Repetă pe **R1, R2 și R3**:

1. Click pe router → tab **Physical**
2. Click **Power Off**
3. Trage modulul **HWIC-2T** în slotul liber
4. Click **Power On**

> ⚠️ HWIC-2T oferă 2 porturi seriale: `Se0/1/0` și `Se0/1/1`

---

## 📋 Pasul 2 — Plan de Adresare IP

### PC-uri (rețele LAN /24)

| Dispozitiv | IP Address | Subnet Mask | Default Gateway |
|---|---|---|---|
| PC1 | `192.168.10.2` | `255.255.255.0` | `192.168.10.1` |
| PC2 | `192.168.20.2` | `255.255.255.0` | `192.168.20.1` |
| PC3 | `192.168.30.2` | `255.255.255.0` | `192.168.30.1` |

### Legături punct-la-punct între routere (/30)

> 💬 Rețelele `/30` cresc din 4 în 4 și sunt dedicate exclusiv legăturilor seriale.

| Legătură | Router 1 (IP) | Router 2 (IP) |
|---|---|---|
| R1 — R2 | `10.0.0.1/30` | `10.0.0.2/30` |
| R2 — R3 | `10.0.0.5/30` | `10.0.0.6/30` |
| R1 — R3 | `10.0.0.9/30` | `10.0.0.10/30` |

### Interfețe routere

| Router | Interfață | IP Address | Rol |
|---|---|---|---|
| R1 | Gi0/0 | `192.168.10.1/24` | LAN către PC1 |
| R1 | Se0/1/0 | `10.0.0.1/30` | Serial → R2 (DCE) |
| R1 | Se0/1/1 | `10.0.0.9/30` | Serial → R3 |
| R2 | Gi0/0 | `192.168.20.1/24` | LAN către PC2 |
| R2 | Se0/1/0 | `10.0.0.2/30` | Serial → R1 |
| R2 | Se0/1/1 | `10.0.0.5/30` | Serial → R3 (DCE) |
| R3 | Gi0/0 | `192.168.30.1/24` | LAN către PC3 |
| R3 | Se0/1/1 | `10.0.0.6/30` | Serial → R2 |
| R3 | Se0/1/0 | `10.0.0.10/30` | Serial → R1 (DCE) |

---

## 🖥️ Pasul 3 — Configurare PC-uri

Pe fiecare PC: **Desktop → IP Configuration**

**PC1**
```
IP Address:      192.168.10.2
Subnet Mask:     255.255.255.0
Default Gateway: 192.168.10.1
```

**PC2**
```
IP Address:      192.168.20.2
Subnet Mask:     255.255.255.0
Default Gateway: 192.168.20.1
```

**PC3**
```
IP Address:      192.168.30.2
Subnet Mask:     255.255.255.0
Default Gateway: 192.168.30.1
```

---

## 🔀 Pasul 4 — Configurare Switch-uri (SW1, SW2, SW3)

Același procedeu pe toate trei switch-urile — activează portul către PC și portul către router ca `access`:

```bash
enable
configure terminal

# Port către PC
interface fa0/1
switchport mode access
no shutdown

# Port către Router
interface gi0/1
switchport mode access
no shutdown

exit
copy running-config startup-config
```

---

## 🌐 Pasul 5 — Configurare Routere + OSPF

> 💬 **Wildcard mask** = inversul măștii de rețea. Pentru `/24` → `0.0.0.255`, pentru `/30` → `0.0.0.3`

### Router R1

```bash
enable
configure terminal

# LAN către PC1
interface g0/0
ip address 192.168.10.1 255.255.255.0
no shutdown

# Serial către R2 (R1 este DCE pe acest link)
interface serial 0/1/0
ip address 10.0.0.1 255.255.255.252
clock rate 64000
no shutdown

# Serial către R3
interface serial 0/1/1
ip address 10.0.0.9 255.255.255.252
no shutdown

# OSPF
router ospf 1
network 192.168.10.0 0.0.0.255 area 0
network 10.0.0.0 0.0.0.3 area 0
network 10.0.0.8 0.0.0.3 area 0
exit

copy running-config startup-config
```

### Router R2

```bash
enable
configure terminal

# LAN către PC2
interface g0/0
ip address 192.168.20.1 255.255.255.0
no shutdown

# Serial către R1
interface serial 0/1/0
ip address 10.0.0.2 255.255.255.252
no shutdown

# Serial către R3 (R2 este DCE pe acest link)
interface serial 0/1/1
ip address 10.0.0.5 255.255.255.252
clock rate 64000
no shutdown

# OSPF
router ospf 1
network 192.168.20.0 0.0.0.255 area 0
network 10.0.0.0 0.0.0.3 area 0
network 10.0.0.4 0.0.0.3 area 0
exit

copy running-config startup-config
```

### Router R3

```bash
enable
configure terminal

# LAN către PC3
interface g0/0
ip address 192.168.30.1 255.255.255.0
no shutdown

# Serial către R2
interface serial 0/1/1
ip address 10.0.0.6 255.255.255.252
no shutdown

# Serial către R1 (R3 este DCE pe acest link)
interface serial 0/1/0
ip address 10.0.0.10 255.255.255.252
clock rate 64000
no shutdown

# OSPF
router ospf 1
network 192.168.30.0 0.0.0.255 area 0
network 10.0.0.4 0.0.0.3 area 0
network 10.0.0.8 0.0.0.3 area 0
exit

copy running-config startup-config
```

> ⚠️ `clock rate` se setează **doar pe capătul DCE** al fiecărui cablu serial.

---

## ✅ Pasul 6 — Verificare

### Starea interfețelor
```bash
Router#show ip interface brief
```
Toate interfețele configurate trebuie să aibă `Status: up` și `Protocol: up`.

### Vecinii OSPF
```bash
Router#show ip ospf neighbor
```
Fiecare router trebuie să vadă **2 vecini** în starea `FULL`. Dacă un vecin lipsește, verifică adresele IP și starea interfețelor seriale.

### Tabela de rutare
```bash
Router#show ip route
```

Exemplu output pe R1 după convergență OSPF:
```
C    192.168.10.0/24 is directly connected, GigabitEthernet0/0
C    10.0.0.0/30 is directly connected, Serial0/1/0
C    10.0.0.8/30 is directly connected, Serial0/1/1
O    192.168.20.0/24 [110/65] via 10.0.0.2, Serial0/1/0   ← învățată prin OSPF
O    192.168.30.0/24 [110/65] via 10.0.0.10, Serial0/1/1  ← învățată prin OSPF
```

| Prefix | Semnificație |
|---|---|
| `C` | Directly connected — rețea direct atașată |
| `L` | Local — adresa IP a interfeței proprii |
| `O` | OSPF — rută învățată dinamic |

---

## 🔁 Pasul 7 — Test de Convergență (Redundanță)

Acesta este cel mai important avantaj al OSPF față de rutarea statică.

**Scenariul:**
1. Pornește un ping continuu de pe **PC1 → PC3**
2. Oprește link-ul serial dintre **R1 și R3**:
```bash
# Pe R1:
interface serial 0/1/1
shutdown
```
3. Observă că ping-ul **continuă să funcționeze**

**Ce se întâmplă:**

```
Înainte:  PC1 → R1 → R3 → PC3   (drum direct)
După:     PC1 → R1 → R2 → R3 → PC3   (rută alternativă, găsită automat)
```

OSPF detectează că link-ul a căzut și recalculează automat cel mai bun drum disponibil — fără nicio intervenție manuală.

---

## 🏓 Pasul 8 — Testare Conectivitate

```bash
# Ping între routere
Router#ping 10.0.0.2

# Ping între PC-uri (din Desktop → Command Prompt)
C:\>ping 192.168.20.2
C:\>ping 192.168.30.2
```

---

## 🛠️ Troubleshooting

| Problemă | Ce verifici |
|---|---|
| Vecin OSPF lipsă | Adresele IP pe Serial, `no shutdown`, `clock rate` pe DCE |
| Rute `O` lipsă din `show ip route` | Comanda `network` în OSPF (wildcard mask corectă?) |
| Ping între PC-uri eșuează | Default gateway pe PC, rute OSPF prezente pe toate routerele |
| Interfață `up/down` | Modul HWIC-2T adăugat și routerul repornit? |

---

## 📖 Referință Rapidă Comenzi

```bash
show ip interface brief      # starea interfețelor
show ip ospf neighbor        # vecinii OSPF (trebuie FULL)
show ip route                # tabela de rutare (O = OSPF)
show ip ospf                 # informații despre procesul OSPF
ping <IP>                    # test conectivitate
copy running-config startup-config   # salvare
clock rate 64000             # viteză serial (doar DCE)
```

---

## ⚖️ OSPF vs Rutare Statică

| | Rutare Statică | OSPF |
|---|---|---|
| Configurare | Manuală pe fiecare router | O dată, pe fiecare router |
| Adaptare la schimbări | ❌ Nu se adaptează | ✅ Recalculează automat |
| Redundanță | ❌ Necesită rute manuale extra | ✅ Automată |
| Scalabilitate | ❌ Greu de gestionat la scară | ✅ Se scalează bine |
| Metric | — | Cost bazat pe lățime de bandă |