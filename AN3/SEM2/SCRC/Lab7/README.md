# Laborator 7: NAT & PAT (Network Address Translation)

## 🎯 Scopul laboratorului

Acest laborator a vizat înțelegerea și configurarea mecanismelor de translatare a adreselor de rețea pentru a permite dispozitivelor dintr-o rețea privată să acceseze resurse de pe Internet folosind un singur IP public.

**Obiective principale:**

- Înțelegerea diferenței dintre IP-ul privat (folosit în LAN) și IP-ul public (folosit în WAN).
- Configurarea **NAT Overload (PAT — Port Address Translation)**.
- Observarea modului în care routerul diferențiază traficul mai multor PC-uri prin porturi.

---

## 1. Topologie și Plan de Adresare

### 🏗️ Echipamente utilizate

| Rol | Dispozitiv | Detalii |
|-----|-----------|---------|
| LAN (Private Network) | PC1, PC2 + Switch (SW1) | Conectate la routerul de margine |
| Marginea Rețelei | Router **CE** (Customer Edge) | Realizează traducerea NAT |
| Internet (WAN) | Router **SP** (Service Provider) | Conectat prin legătură Serială DCE la CE |
| Server Extern | External Server | Simulează o resursă publică de pe Internet |

### 🌐 Plan de adresare IP

| Zonă | Dispozitiv / Interfață | Adresă IP |
|------|------------------------|-----------|
| Inside (LAN) | PC1 | `192.168.1.10/24` |
| Inside (LAN) | PC2 | `192.168.1.20/24` |
| Inside (LAN) | Gateway (CE Gi0/0) | `192.168.1.1/24` |
| Inside Global (Public) | CE Serial0/1/0 | `220.110.0.2/30` |
| Outside (WAN) | SP Serial0/1/0 | `220.110.0.1/30` |
| Outside (WAN) | Server Extern | `10.0.0.10/8` |

---

## 2. Configurarea NAT pe Routerul CE

Routerul CE a fost configurat să marcheze interfețele interne și externe și să aplice regula de translatare.

```cisco
! Definirea interfețelor
interface GigabitEthernet0/0
 ip address 192.168.1.1 255.255.255.0
 ip nat inside

interface Serial0/1/0
 ip address 220.110.0.2 255.255.255.252
 ip nat outside

! Rută implicită către Internet
ip route 0.0.0.0 0.0.0.0 Serial0/1/0

! ACL pentru a selecta cine are voie să fie tradus
access-list 1 permit 192.168.1.0 0.0.0.255

! Activarea NAT Overload (PAT)
ip nat inside source list 1 interface Serial0/1/0 overload
```

---

## 3. Configurarea Routerului SP (Service Provider)

Pe routerul ISP am configurat legătura serială cu rată de ceas (DCE) și ruta de întoarcere către rețeaua privată.

```cisco
interface Serial0/1/0
 ip address 220.110.0.1 255.255.255.252
 clock rate 64000

interface GigabitEthernet0/1
 ip address 10.0.0.1 255.0.0.0

! Ruta înapoi către rețeaua privată prin routerul CE
ip route 192.168.1.0 255.255.255.0 220.110.0.2
```

---

## 4. Verificare și Rezultate

### ✅ Teste de conectivitate (Ping)

| Test | Comandă | Rezultat |
|------|---------|----------|
| PC → Gateway local | `ping 192.168.1.1` | ✅ Succes |
| PC → Server extern | `ping 10.0.0.10` | ✅ Succes |

### 🔍 Monitorizare NAT

```cisco
show ip nat statistics
show ip nat translations
```

- `show ip nat statistics` — confirmă interfețele marcate ca **Inside** și **Outside**.
- `show ip nat translations` — afișează mapările active (IP privat ↔ IP public + port).

> **Observație tehnică:** La utilizarea protocolului ICMP (ping), traducerile apar scurt în tabelă și sunt șterse rapid după finalizarea cererii *(Expired translations)*. Pentru a vizualiza tabela de translatare în timp real, s-a utilizat un ping continuu:
> ```
> ping -t 10.0.0.10
> ```