# Laborator Administrarea rețelelor: VLAN-uri pe switch-uri Cisco

Obiective: VLAN-uri, porturi access și trunk, mai multe IP-uri pe același switch (interfețe VLAN), testare cu VPCS, VLAN de management și acces prin telnet.

## Topologia finală

- **SWITCH1** (Cisco IOSvL2): PC1 (Gi0/0), PC2 (Gi0/1), PC3 (Gi0/2), PC4 (Gi0/3), Debian (Gi3/2), trunk către SWITCH2 (Gi3/3)
- **SWITCH2** (Cisco IOSvL2): PC5 (Gi0/0), PC6 (Gi0/1), trunk către SWITCH1 (Gi3/3)
- **Debian 12.6**: ens4 spre Cloud (Internet, DHCP), ens5 spre SWITCH1 Gi3/2 (IP static)

| Nod | IP | VLAN |
|---|---|---|
| PC1 | 172.30.10.1 / 255.255.0.0 | 120 (OFFICE) |
| PC2 | 172.30.10.2 / 255.255.0.0 | 120 (OFFICE) |
| PC5 | 172.30.10.3 / 255.255.0.0 | 120 (OFFICE) |
| PC3 | 172.30.20.1 / 255.255.0.0 | 121 (GAMES) |
| PC4 | 172.30.20.2 / 255.255.0.0 | 121 (GAMES) |
| PC6 | 172.30.20.3 / 255.255.0.0 | 121 (GAMES) |
| Debian ens5 | 172.80.8.1 / 255.255.255.0 | 500 (MGMT) |
| SWITCH1 Vlan500 | 172.80.8.2 / 255.255.255.0 | 500 |
| SWITCH2 Vlan500 | 172.80.8.3 / 255.255.255.0 | 500 |

---

## Partea 1: un switch, două VLAN-uri (pașii 1-13)

### Pașii 1-2: topologia și IP-urile
Pui un switch Cisco și 4 VPCS, legate pe Gi0/0 - Gi0/3. Pe fiecare VPCS:
```
PC1> ip 172.30.10.1 255.255.0.0
PC2> ip 172.30.10.2 255.255.0.0
PC3> ip 172.30.20.1 255.255.0.0
PC4> ip 172.30.20.2 255.255.0.0
```
- `save` pe VPCS scrie configurația în folderul proiectului, deci rămâne după restart. Fără `save` se pierde.
- Cu masca /16 toate cele 4 PC-uri sunt în aceeași rețea (172.30.x.x), deci pingul merge între toate. Switch-ul nu are încă VLAN-uri, toate porturile sunt în VLAN 1.

### Pașii 3-4: starea inițială
```
Switch> enable
Switch# show interfaces status
```
- Doar primele 4 porturi (Gi0/0 - Gi0/3) sunt `connected`, restul `notconnect`.
- Toate porturile sunt în **VLAN 1** (VLAN-ul default) și în mod access/dinamic.
- Pe switch-urile Cisco, porturile se identifică drept interfețe: Gi0/0, Gi0/1 etc. (Gigabit, modul/port).

### Pașii 5-6: crearea VLAN-urilor
```
configure terminal
vlan 120
 name OFFICE
 exit
vlan 121
 name GAMES
 exit
end
show vlan
```
- `vlan 120` creează VLAN-ul și intră în modul lui de configurare (se schimbă promptul, de aceea e nevoie de `exit`).
- Numele (OFFICE, GAMES) rămâne **doar local pe switch**, nu se transmite în rețea. Contează doar TAG-ul (120, 121).
- În `show vlan`, VLAN-urile noi apar fără niciun port în coloana Ports.

### Pașii 7-9: alocarea porturilor
Gi0/0 și Gi0/1 în VLAN 120, Gi0/2 și Gi0/3 în VLAN 121, toate **access / untagged**.
```
configure terminal
interface range gigabitEthernet 0/0 - 1
 switchport trunk encapsulation dot1q
 switchport mode access
 switchport access vlan 120
 exit
interface range gigabitEthernet 0/2 - 3
 switchport trunk encapsulation dot1q
 switchport mode access
 switchport access vlan 121
 end
```
- `switchport trunk encapsulation dot1q`: setează standardul IEEE 802.1Q. E o particularitate a imaginii din simulator. În mod normal nu e necesar sau nici nu există. Trebuie dat **înainte** de `switchport mode ...`.
- `switchport mode access`: portul devine access (un singur VLAN, trafic fără tag).
- `switchport access vlan 120`: VLAN-ul untagged al portului.
- `interface range` selectează mai multe interfețe, iar setările se aplică tuturor. În primul bloc `gigabitEthernet 0/0 - 1` înseamnă Gi0/0 și Gi0/1.
- Ajutor în consolă: `?` după orice cuvânt arată opțiunile, TAB face autocomplete.

### Pașii 10-12: verificare și salvare
```
show vlan
show interfaces status
write
show running-config
```
- `show vlan`: Gi0/0, Gi0/1 în OFFICE, Gi0/2, Gi0/3 în GAMES.
- `show interfaces status`: coloana Vlan arată 120 / 121.
- `write` salvează în memoria switch-ului. Fără el, configurația se pierde la restart.
- În `show running-config`, sub interfețe apar liniile `switchport access vlan ...` și `switchport mode access`.

### Pasul 13: testul
```
PC1> ping 172.30.10.2   (același VLAN 120: merge)
PC1> ping 172.30.20.1   (VLAN 121: NU merge)
PC3> ping 172.30.20.2   (același VLAN 121: merge)
```
Segregarea funcționează: cele două VLAN-uri nu comunică între ele (ar fi nevoie de un router/L3).

---

## Partea 2: al doilea switch și trunk (pașii 14-23)

### Pașii 14-17: SWITCH2 și PC5/PC6
- Adaugi SWITCH2 (nelegat încă de SWITCH1), PC5 pe Gi0/0 și PC6 pe Gi0/1.
- Pe SWITCH2 creezi VLAN 120 (OFFICE) și 121 (GAMES) și pui Gi0/0 în 120, Gi0/1 în 121 (aceleași comenzi ca la SWITCH1, doar pentru 2 porturi).
- IP-uri:
```
PC5> ip 172.30.10.3 255.255.0.0
PC6> ip 172.30.20.3 255.255.0.0
```

### Pașii 18-19: teste înainte de trunk
- PC5 → PC6: **nu merge** (VLAN-uri diferite).
- PC5 → PC1/PC2 și PC6 → PC3/PC4: **nu merge**, pentru că switch-urile nu sunt legate.

### Pașii 20-22: legătura trunk
Legi SWITCH1 Gi3/3 de SWITCH2 Gi3/3. Pe **ambele** switch-uri:
```
configure terminal
interface gigabitEthernet 3/3
 switchport trunk encapsulation dot1q
 switchport mode trunk
 switchport trunk allowed vlan add 120,121
 exit
exit
write
show interfaces status
show running-config
```
- Un port **trunk** transportă mai multe VLAN-uri între switch-uri, pe aceeași legătură, folosind **tag-uri 802.1Q**.
- `switchport mode trunk`: portul devine trunk. În `show interfaces status` apare `trunk` la coloana Vlan.
- `switchport trunk allowed vlan add 120,121`: specifică VLAN-urile transportate.
- **Atenție:** lista implicită a unui trunk este deja 1-4094 (toate), deci `add` nu o restrânge (în `show interfaces trunk` se vede tot `1-4094`). Dacă vrei un trunk strict, folosești varianta **fără** `add`: `switchport trunk allowed vlan 120,121,500`. Fără `add` lista se **înlocuiește**, cu `add` se **adaugă** la ea.
- `show interfaces trunk` are 3 liste: *allowed on trunk* (ce permite configurația), *allowed and active* (permis și existent ca VLAN pe switch), *forwarding* (ce trece efectiv).

Teste: PC1, PC2 și PC5 (VLAN 120) comunică între ele, la fel PC3, PC4 și PC6 (VLAN 121). Între cele două grupuri nu.

### Pasul 23: packet capture
Click dreapta pe legătura dintre switch-uri → *Start capture* → Wireshark. Cu un ping continuu (`ping 172.30.10.3 -t`) și filtrul `icmp`:
- pe trunk, pachetele au tag **802.1Q** cu ID 120 (PC1 ↔ PC5) sau 121 (PC3 ↔ PC6)
- pe legătura PC1 ↔ SWITCH1 (port access) pachetele **nu au** tag

Tag-ul se adaugă la intrarea pe trunk și se scoate la ieșirea pe un port access.

---

## Partea 3: VLAN de management și telnet (pașii 24-36)

Scop: interfețele de management ale switch-urilor nu se lasă accesibile din rețelele de utilizatori, ci dintr-un VLAN separat (VLAN 500, MGMT), la care au acces doar administratorii.

### Pașii 24-26: nodul Debian
- Adaugi un Cloud și un nod Debian 12.6 (nu îl pornești încă).
- Click dreapta Debian → Configure → Network → **Adapters = 2**. *Configure custom adapters* arată numele interfețelor (ens4 și ens5). Le notezi.
- **ens4** → Cloud (Internet), **ens5** → SWITCH1 Gi3/2. Apoi pornești Debian.

### Pasul 27: ens4 pe DHCP + Internet
Login: `debian` / `debian`. În `/etc/network/interfaces`:
```
auto ens4
iface ens4 inet dhcp
```
Verificare: `ip a`, `ping 8.8.8.8` (conectivitate), `ping google.com` (verifică și DNS-ul).

### Pasul 28: pachetele
```
sudo apt update
sudo apt install net-tools dnsutils telnet
```
- `net-tools` (ifconfig), `dnsutils` (nslookup), `telnet` (clientul).
- Dacă apare *Waiting for cache lock ... unattended-upgrades*, Debian își face update-urile de securitate în fundal. Aștepți sau dai reboot, apoi reiei instalarea.

### Pasul 29: ens5 cu IP static
În `/etc/network/interfaces`:
```
auto ens5
iface ens5 inet static
    address 172.80.8.1
    netmask 255.255.255.0
```
Apoi restart (sau reboot) și verifici cu `ip a`. Dacă `systemctl restart networking` dă eroare, un `sudo reboot` rezolvă.

### Pașii 30-31: VLAN 500 pe ambele switch-uri
```
configure terminal
vlan 500
 name MGMT
 exit
interface gigabitEthernet 3/3
 switchport trunk allowed vlan add 500
 exit
exit
show vlan
show interfaces trunk
```
VLAN-ul trebuie creat pe **ambele** switch-uri și permis pe trunk, altfel nu ajunge la SWITCH2.

### Pasul 32: IP pe interfața VLAN 500 (SVI)
```
configure terminal
interface vlan 500
 ip address 172.80.8.2 255.255.255.0     (SWITCH1)
 ip address 172.80.8.3 255.255.255.0     (SWITCH2)
 no shutdown
 exit
exit
```
- Interfața VLAN (SVI) e „adresa switch-ului” în acel VLAN, prin care îl administrezi.
- `no shutdown` pornește interfața. Fără el rămâne `administratively down`.
- Interfața VLAN devine `up/up` doar dacă există cel puțin un port activ în VLAN 500 (sau trunk care îl transportă).

### Pasul 33: portul spre Debian (doar SWITCH1)
```
configure terminal
interface gigabitEthernet 3/2
 switchport trunk encapsulation dot1q
 switchport mode access
 switchport access vlan 500
 exit
exit
```
Acesta este punctul de intrare în rețeaua de management. SWITCH2 este vizibil în VLAN 500 prin trunk, nu are nevoie de legătură separată către Debian.

### Pasul 34: test din Debian
```
ping 172.80.8.2    (SWITCH1)
ping 172.80.8.3    (SWITCH2, prin trunk)
```

### Pasul 35: utilizator și telnet, pe ambele switch-uri
```
configure terminal
username admin privilege 15 password admin
line vty 0 4
 login local
 transport input telnet
 exit
enable secret admin
exit
write
```
- `username admin privilege 15 password admin`: utilizator local cu privilegii maxime (nivel 15).
- `line vty 0 4`: cele 5 linii virtuale pentru acces la distanță.
- `login local`: autentificare pe baza utilizatorilor locali.
- `transport input telnet`: acceptă conexiuni telnet.
- `enable secret admin`: parola pentru `enable`.

Test din Debian (în enunț apare `172.30.0.2` ca exemplu, la noi adresele sunt din VLAN 500):
```
telnet 172.80.8.2
telnet 172.80.8.3
```
Login `admin` / `admin`. Userul cu privilege 15 intră direct în `Switch#`, fără `enable` (parola enable se cere doar userilor cu privilegii mai mici).

### Pasul 36: salvare și verificare
```
write
show running-config
```
Verifici: VLAN-urile, interfețele access, trunk-ul Gi3/3, `interface Vlan500` cu IP, userul admin și `line vty 0 4` cu `login local` și `transport input telnet`.

---

## Parole în running-config

```
enable secret 5 $1$KCAd$...                    (hash MD5)
username admin privilege 15 password 0 admin    (text clar)
```
| Tip | Semnificație |
|---|---|
| 0 | text clar |
| 5 | hash MD5 (`enable secret`, `username ... secret`) |
| 7 | criptare slabă, reversibilă (`service password-encryption`) |

`enable secret` are prioritate față de `enable password`.

## Capcane și de reținut

- `switchport trunk encapsulation dot1q` se dă înainte de `switchport mode ...` (particularitate a simulatorului).
- Access = untagged, un singur VLAN. Trunk = tagged 802.1Q, mai multe VLAN-uri. Tag-ul apare doar pe trunk.
- Numele unui VLAN e local, tag-ul (ID-ul) e cel care contează.
- Fără `write`, configurația switch-ului se pierde la restart.
- VLAN-urile trebuie create pe **ambele** switch-uri și permise pe trunk.
- `allowed vlan add` adaugă la listă, `allowed vlan X,Y` o înlocuiește. Lista implicită e 1-4094.
- Interfața VLAN rămâne down fără `no shutdown` sau fără port activ în acel VLAN.
- VLAN-uri diferite nu comunică fără un dispozitiv de nivel 3 (router).
- Interfețele de management se izolează într-un VLAN separat, din motive de securitate.
- Pe VPCS, IP-ul se păstrează doar cu `save`.
