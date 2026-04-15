# Laborator 6: DHCP, DNS și Web Server

## 🎯 Scopul Laboratorului

În acest laborator am renunțat la configurarea manuală a adreselor IP și am simulat o rețea reală folosind servicii dedicate.

| Obiectiv | Descriere |
|---|---|
| **DHCP** | Alocarea automată a adreselor IP către PC-uri |
| **DNS** | Traducerea numelor (`www.lab.local`) în adrese IP |
| **Web Server** | Găzduirea unei pagini web accesibile prin rețea |
| **DHCP Relay** | Folosirea `ip helper-address` pentru cereri DHCP cross-router |

---

## 1. Topologie și Echipamente

Rețeaua este împărțită în **două segmente** (LAN 1 și LAN 2), separate printr-un router.

```
[ PC1 ]──┐                           ┌──[ DHCP Server - .100 ]
[ PC2 ]──┤──[ SW1 ]──[ R1 ]──[ SW2 ]──┤──[ DNS Server  - .101 ]
         │                           ├──[ WEB Server  - .102 ]
         │                           └──[ PC3 ]
      LAN 1                              LAN 2
  192.168.10.0/24                   192.168.20.0/24
```

### Planul de Adresare IP

| Segment | Rețea | Gateway |
|---|---|---|
| **LAN 1** | `192.168.10.0/24` | `192.168.10.1` |
| **LAN 2** | `192.168.20.0/24` | `192.168.20.1` |

### IP-uri Statice Servere

| Dispozitiv | Adresă IP |
|---|---|
| Server DHCP | `192.168.20.100` |
| Server DNS | `192.168.20.101` |
| Server WEB | `192.168.20.102` |

---

## 2. Configurarea Routerului (R1) și DHCP Relay

PC-urile din **LAN 1** se află în altă rețea față de Serverul DHCP (**LAN 2**), deci cererile DHCP ar fi blocate de router. Soluția este `ip helper-address`, care transformă routerul într-un **DHCP Relay Agent**.

```cisco
enable
configure terminal

! 1. Interfața LAN 1 — activăm DHCP Relay
interface g0/0
 ip address 192.168.10.1 255.255.255.0
 ip helper-address 192.168.20.100
 no shutdown
exit

! 2. Interfața LAN 2 — spre Servere
interface g0/1
 ip address 192.168.20.1 255.255.255.0
 no shutdown
exit

copy running-config startup-config
```

> **`ip helper-address 192.168.20.100`** — redirecționează cererile DHCP broadcast din LAN 1 ca unicast către Serverul DHCP din LAN 2.

---

## 3. Configurarea Serverelor

### A. Serverul DHCP (`192.168.20.100`)

**IP Configuration (static):**
- IP: `192.168.20.100` | Subnet: `255.255.255.0` | Gateway: `192.168.20.1` | DNS: `192.168.20.101`

**Serviciul DHCP → ON**, Pool: `LAN1`

| Parametru | Valoare |
|---|---|
| Default Gateway | `192.168.10.1` |
| DNS Server | `192.168.20.101` |
| Start IP Address | `192.168.10.10` |

---

### B. Serverul DNS (`192.168.20.101`)

**IP Configuration (static):**
- IP: `192.168.20.101` | Gateway: `192.168.20.1`

**Serviciul DNS → ON**, A Record adăugat:

| Name | Address |
|---|---|
| `www.lab.local` | `192.168.20.102` |

---

### C. Serverul WEB (`192.168.20.102`)

**IP Configuration (static):**
- IP: `192.168.20.102` | Gateway: `192.168.20.1`

**Servicii HTTP și HTTPS → ON** (configurație implicită Packet Tracer).

---

## 4. Verificare și Testare

### ✅ 1. Obținerea adresei IP prin DHCP

Pe **PC1** și **PC2** am setat placa de rețea pe modul `DHCP`.

**Rezultat:** PC-urile au primit automat adrese începând cu `192.168.10.10` — confirmare că DHCP Relay funcționează corect.

---

### ✅ 2. Testarea conectivității (Ping)

```
ping 192.168.10.1    → Succes  (Gateway LAN 1)
ping 192.168.20.102  → Succes  (Server Web)
```

---

### ✅ 3. Testarea rezolvării DNS

```
ping www.lab.local
```

Pachetele au ajuns la `192.168.20.102` — confirmă funcționarea Serverului DNS.

---

### ✅ 4. Testul Web Browser

```
http://www.lab.local
```

S-a afișat cu succes pagina principală Cisco Packet Tracer de pe **PC1**.