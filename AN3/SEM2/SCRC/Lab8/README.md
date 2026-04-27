# Laborator 8: Access Control Lists (ACL)

## 🎯 Scopul laboratorului

Acest laborator a vizat securizarea unei rețele fragmentate în VLAN-uri, controlând ce trafic este permis și ce trafic este blocat. S-a demonstrat diferența dintre blocarea la nivel de rețea sursă (ACL Standard) și blocarea granulară la nivel de protocol/port (ACL Extins).

---

## 1. Topologie și Adresare

Rețeaua a fost configurată cu 4 VLAN-uri rutate prin metoda **Router-on-a-Stick**:

| VLAN | Nume | Subnet | Dispozitiv |
|------|------|--------|-----------|
| VLAN 10 | USERS | `192.168.10.0/24` | PC1 |
| VLAN 20 | STAFF | `192.168.20.0/24` | PC2 |
| VLAN 30 | GUESTS | `192.168.30.0/24` | PC3 |
| VLAN 40 | SERVERS | `192.168.40.0/24` | Web Server |

---

## 2. Politici de Securitate Implementate

### A. ACL Standard — Blocare trafic între VLAN 10 și VLAN 20

**Obiectiv:** VLAN 10 nu are voie să comunice cu VLAN 20.

> **Regulă de plasament:** ACL-urile standard filtrează doar după adresa sursă, deci regula se aplică **cât mai aproape de destinație** — direcția `out` pe subinterfața VLAN 20.

```cisco
! Blocăm rețeaua 192.168.10.0 și permitem restul
access-list 10 deny   192.168.10.0 0.0.0.255
access-list 10 permit any

! Aplicare pe interfața de destinație
interface GigabitEthernet0/0.20
 ip access-group 10 out
```

---

### B. ACL Extins — Filtrare trafic din VLAN 30 spre Server

**Obiectiv:** VLAN 30 poate accesa serverul **doar prin HTTP (port 80)**. Ping-ul (ICMP) și orice alt trafic spre server sunt blocate.

> **Regulă de plasament:** ACL-urile extinse se aplică **cât mai aproape de sursă** — direcția `in` pe subinterfața VLAN 30 — pentru a opri traficul nedorit din fașă.

```cisco
! Permitem doar HTTP (TCP port 80) spre server
access-list 110 permit tcp 192.168.30.0 0.0.0.255 host 192.168.40.10 eq www

! Blocăm explicit ping-ul (ICMP) spre server
access-list 110 deny icmp 192.168.30.0 0.0.0.255 host 192.168.40.10

! Blocăm orice alt trafic IP spre rețeaua serverului
access-list 110 deny ip 192.168.30.0 0.0.0.255 192.168.40.0 0.0.0.255

! Permitem restul traficului (ex: spre Internet / alte rețele)
access-list 110 permit ip any any

! Aplicare pe interfața sursă
interface GigabitEthernet0/0.30
 ip access-group 110 in
```

---

## 3. Rezultatele Testelor (Validare)

Toate testele de conectivitate au validat regulile impuse:

| # | Test | Rezultat | Explicație |
|---|------|----------|------------|
| 1 | Ping PC1 → PC2 (`192.168.20.10`) | ❌ `Destination host unreachable` | Blocat de ACL 10 |
| 2 | Ping PC1 → Server (`192.168.40.10`) | ✅ `Reply` primit | Permis de `permit any` din ACL 10 |
| 3 | Ping PC3 → Server (`192.168.40.10`) | ❌ `Destination host unreachable` | ICMP blocat de ACL 110 |
| 4 | HTTP PC3 → Server (`192.168.40.10`) | ✅ Pagina s-a încărcat | TCP port 80 permis de ACL 110 |