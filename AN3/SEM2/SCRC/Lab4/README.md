# 🖧 Laborator 4 — Rutare Statică

> **Cisco Packet Tracer** · Rețele de Calculatoare

Conectarea a două rețele LAN separate prin rutare statică manuală pe routere Cisco.

---

## 📌 Scopul Laboratorului

- Conectarea a două rețele diferite prin routere
- Înțelegerea conceptului de **next-hop**
- Configurarea **rutelor statice** pe routere Cisco
- Citirea și înțelegerea **tabelei de rutare**

---

## 💡 Idee de Bază

```
Fiecare LAN = o rețea separată
Routerele le conectează
Routerele NU știu automat toate rețelele → le spunem manual drumul (rute statice)
```

---

## 🗺️ Topologie

```
PC1 --- SW1 --- R1 ===serial=== R2 --- SW2 --- PC2
```

### Dispozitive

| Dispozitiv | Model |
|---|---|
| Switch x2 | Cisco 2960-24TT |
| Router x2 | Cisco 1941 |
| PC x2 | PC-PT |

### Conexiuni

| De la | La | Porturi | Tip cablu |
|---|---|---|---|
| PC1 | SW1 | Fa0 → Fa0/1 | Straight |
| SW1 | R1 | Gi0/1 → Gi0/0 | Straight |
| R1 | R2 | Se0/1/0 → Se0/1/0 | **Serial DCE** |
| R2 | SW2 | Gi0/0 → Gi0/1 | Straight |
| SW2 | PC2 | Fa0/1 → Fa0 | Straight |

---

## 🔧 Pasul 1 — Adaugarea Modulului Serial (HWIC-2T)

Routerul Cisco 1941 nu are porturi seriale implicit. Trebuie adăugat modulul **HWIC-2T** pe **ambele routere**.

1. Click pe router → tab **Physical**
2. Click **Power Off** (oprește routerul)
3. Trage modulul **HWIC-2T** din lista din stânga în slotul liber
4. Click **Power On** (pornește routerul)

> ⚠️ Fă asta pe **R1 și R2** înainte de a trage orice cablu serial.

---

## 📋 Pasul 2 — Plan de Adresare IP

| Dispozitiv | Interfață | IP Address | Subnet Mask | Default Gateway |
|---|---|---|---|---|
| PC1 | Fa0 | `192.168.10.2` | `255.255.255.128` | `192.168.10.1` |
| PC2 | Fa0 | `192.168.20.2` | `255.255.255.128` | `192.168.20.1` |
| R1 | Gi0/0 | `192.168.10.1` | `255.255.255.128` | — |
| R1 | Se0/1/0 | `10.0.0.1` | `255.255.255.252` | — |
| R2 | Gi0/0 | `192.168.20.1` | `255.255.255.128` | — |
| R2 | Se0/1/0 | `10.0.0.2` | `255.255.255.252` | — |

> 💬 Rețeaua `10.0.0.0/30` este dedicată exclusiv legăturii seriale dintre cele două routere.

---

## 🖥️ Pasul 3 — Configurare PC-uri

Pe fiecare PC: **Desktop → IP Configuration**

**PC1**
```
IP Address:      192.168.10.2
Subnet Mask:     255.255.255.128
Default Gateway: 192.168.10.1
```

**PC2**
```
IP Address:      192.168.20.2
Subnet Mask:     255.255.255.128
Default Gateway: 192.168.20.1
```

---

## 🔀 Pasul 4 — Configurare Switch 1 (SW1)

```bash
# Port către PC1
Switch(config)#interface fa0/1
Switch(config-if)#switchport mode access
Switch(config-if)#no shutdown

# Port către R1
Switch(config-if)#interface gi0/1
Switch(config-if)#switchport mode access
Switch(config-if)#no shutdown

# Salvare
Switch#copy running-config startup-config
```

---

## 🔀 Pasul 5 — Configurare Switch 2 (SW2)

Identic cu SW1 — configurează `fa0/1` (către PC2) și `gi0/1` (către R2) ca access ports, apoi salvează.

---

## 🌐 Pasul 6 — Configurare Router R1

```bash
# Interfața LAN către PC1
Router(config)#interface g0/0
Router(config-if)#ip address 192.168.10.1 255.255.255.128
Router(config-if)#no shutdown

# Interfața Serial către R2 (R1 este capătul DCE)
Router(config)#interface serial 0/1/0
Router(config-if)#ip address 10.0.0.1 255.255.255.252
Router(config-if)#clock rate 64000
Router(config-if)#no shutdown

# Rută statică către rețeaua lui PC2
Router(config)#ip route 192.168.20.0 255.255.255.128 10.0.0.2

# Salvare
Router#copy running-config startup-config
```

> ⚠️ `clock rate` se setează **doar pe routerul DCE** (R1 în acest caz). Definește viteza fizică a legăturii seriale în bps.

**Cum funcționează ruta statică:**
```
ip route [rețea destinație] [mască] [next-hop]
ip route 192.168.20.0 255.255.255.128 10.0.0.2
→ "Pentru a ajunge la 192.168.20.0/25, trimite pachetele la R2 (10.0.0.2)"
```

---

## 🌐 Pasul 7 — Configurare Router R2

```bash
# Interfața LAN către PC2
Router(config)#interface g0/0
Router(config-if)#ip address 192.168.20.1 255.255.255.128
Router(config-if)#no shutdown

# Interfața Serial către R1 (R2 este capătul DTE — fără clock rate)
Router(config)#interface serial 0/1/0
Router(config-if)#ip address 10.0.0.2 255.255.255.252
Router(config-if)#no shutdown

# Rută statică către rețeaua lui PC1
Router(config)#ip route 192.168.10.0 255.255.255.128 10.0.0.1

# Salvare
Router#copy running-config startup-config
```

---

## ✅ Pasul 8 — Verificare

### Starea interfețelor
```bash
Router#show ip interface brief
```
Toate interfețele configurate trebuie să aibă `Status: up` și `Protocol: up`.

### Tabela de rutare
```bash
Router#show ip route
```

Exemple de output pe R2:
```
C    10.0.0.0/30 is directly connected, Serial0/1/0
C    192.168.20.0/25 is directly connected, GigabitEthernet0/0
S    192.168.10.0/25 [1/0] via 10.0.0.1        ← ruta statică
```

| Prefix | Semnificație |
|---|---|
| `C` | Directly connected — rețea direct atașată |
| `L` | Local — adresa IP a interfeței proprii |
| `S` | Static — rută configurată manual |

---

## 🏓 Pasul 9 — Testare Conectivitate

### Ping între routere (din CLI)
```bash
Router#ping 10.0.0.2
```
```
!!!!!
Success rate is 100 percent (5/5), round-trip min/avg/max = 1/2/6 ms
```

### Ping între PC-uri (Desktop → Command Prompt)
```bash
C:\>ping 192.168.20.2
```

Dacă primești răspuns → **configurația este corectă** 🎉

---

## 🛠️ Troubleshooting

| Problemă | Ce verifici |
|---|---|
| Interfață `down/down` | `no shutdown` pe interfață |
| Interfață `up/down` | Cablu greșit sau lipsă modul HWIC-2T |
| Ping între routere eșuează | Adresele IP pe Serial, clock rate pe DCE |
| Ping între PC-uri eșuează | Default gateway pe PC-uri, rutele statice pe ambele routere |
| Ruta statică lipsă din `show ip route` | Reintroduce comanda `ip route` în config mode |

---

## 📖 Referință Rapidă Comenzi

```bash
show ip interface brief          # starea interfețelor
show ip route                    # tabela de rutare
show running-config              # configurația curentă
ping <IP>                        # test conectivitate
copy running-config startup-config  # salvare
no shutdown                      # activare interfață
clock rate 64000                 # viteză serial (doar DCE)
```