# Administrarea rețelelor: Laborator 2 (GNS3)

Ce am făcut până acum: capitolul 1 din lucrare (rețea simplă cu nod Cloud/NAT + nod Firefox). Capitolul 3 (switch simplu) urmează.

## Setup

- GNS3 2.2.61 pe Ubuntu, laptop pe Wi-Fi (`wlo1`), fără cablu (`enp4s0`).
- Topologie: nod **NAT** legat de un nod **Firefox** (QEMU, adaptor Intel e1000, MAC de bază `0c:93:cd:eb:00:00`).
- Nodul Firefox QEMU nu are tab-ul *Network Configuration* din lucrare (cel cu `auto eth0` / `iface eth0 inet dhcp` e la nodul Docker webterm). Pe QEMU nu e nimic de editat, ia IP prin DHCP singur.
- Consolă: click dreapta pe nod → *console* (VNC), apoi terminal din VM.

### Cloud vs NAT

| | Cloud | NAT |
|---|---|---|
| Cum merge | bridge pe placa reală | rețea internă GNS3 (`192.168.122.0/24`) |
| Pe Wi-Fi | **nu merge** (routerul vede un singur MAC, aruncă pachetele cu alt MAC) | merge |
| Pe cablu | merge | merge |

Testat: cu Cloud pe Wi-Fi nu a luat IP/internet, cu NAT a mers direct. Pentru laborator, dacă PC-ul e pe cablu, Cloud pe interfața cu cablu; altfel NAT.

## Exercițiul 1: comanda `ip`

| Cerință | Comandă | Rezultat |
|---|---|---|
| 1.1 Placa activă (fără `lo`) | `ip link` | `eth0` (`state UP`) |
| 1.2 MAC | `ip link` | `0c:93:cd:eb:00:00` |
| 1.3 IP placă | `ip address show` | `192.168.122.84/24` |
| 1.4 Gateway | `ip route show` | `192.168.122.1` (`default via ... dev eth0`) |

Celelalte plăci (`dummy0`, `tunl0`, `ip_vti0`) sunt `DOWN`.

## Exercițiul 2: `ifconfig`

Merge pe VM. Afișează aceleași date ca `ip` (MAC `HWaddr`, IP `inet addr`, mască `255.255.255.0`, broadcast `192.168.122.255`) în alt format. Nu arată gateway-ul. E comandă veche (pachet opțional), pe alte sisteme poate lipsi.

## Exercițiul 3: `ping`

Oprire cu **Ctrl+C**.

| Țintă | Rezultat |
|---|---|
| 3.1 Gateway `192.168.122.1` | 0% loss, ~0.5 ms, `ttl=64` |
| 3.2 `8.8.8.8` | 0% loss, ~40 ms, `ttl=114` |
| 3.3 IP propriu (`192.168.122.84` și `127.0.0.1`) | 0% loss, ~0.15-0.3 ms, nu iese din VM |
| 3.4 `10.0.0.1` (inexistent) | 100% packet loss, niciun răspuns |
| 3.5 `google.com` | numele e rezolvat în IP (`142.251.38.238`) prin DNS, ~65 ms, `ttl=111` |

Observație la 3.5: ping-ul face rezolvare DNS, deci rețeaua are și conectivitate IP, și rezolvare de nume.

## Exercițiul 4: `traceroute 8.8.8.8`

- **Primul hop: `192.168.122.1`**, gateway-ul (laptopul, `flavius-Katana-GF66-12UEO`).
- Drum complet de 11 hop-uri până la `dns.google (8.8.8.8)`, ~41 ms.
- Hop-urile cu `* * *` (4, 9, 10) nu răspund la sonde, nu e eroare.

## Exercițiul 5: `arp`

O singură intrare:

```
192.168.122.1 at 52:54:00:cd:7e:3e [ether] on eth0
```

Explicație: ARP ține doar IP → MAC pentru echipamentele din **aceeași rețea locală** cu care VM-ul a comunicat. Singurul e gateway-ul. Destinațiile din afară (8.8.8.8, google.com, hop-urile) nu apar, pentru că pachetele merg la MAC-ul gateway-ului, care le trimite mai departe.

## De reținut / capcane

- Valorile (IP, gateway, MAC) diferă în laborator, se citesc din comenzi, nu se copiază de aici.
- `ip route show` pentru gateway (`default via`), nu `ip address`.
- Ping-ul nu se oprește singur, Ctrl+C.
- `ifconfig` poate lipsi pe unele sisteme.
- Ping către `127.0.0.1` ≠ ping către IP-ul plăcii, notează-le pe amândouă.

## De făcut

- Capitolul 3: Cloud/NAT pe primul port al unui **Ethernet switch** + 2 noduri Firefox cu DHCP, refăcut ex. 1, ping între cele două noduri în ambele sensuri.
- 3.6 (ping între nodurile colegilor) are nevoie de rețea reală (Cloud pe cablu), pe NAT nu merge.
