# Lucrarea 3 - Administrarea rețelelor
## 3. Asignare manuală de adresă IP în nod VPCS

## Scop
Rețea izolată (fără NAT sau Cloud), formată dintr-un switch și 3 noduri VPCS. Se asignează manual adrese IP și se testează comunicarea cu `ping`, în funcție de subnet.

## Topologie
```
       Switch1
      /   |   \
    PC1  PC2  PC3
```
Nodurile sunt pornite din butonul START din bara GNS3.

## Comenzi folosite (consola VPCS)
| Comandă | Rol |
|---|---|
| `ip <adresă>/<prefix>` | setează adresa IP și masca (fără prefix, masca implicită este /24) |
| `show` | afișează IP/mască/gateway |
| `ping <adresă>` | testează conectivitatea |

## Cazul 1: `192.168.0.0 mask 255.255.255.192` (/26)

**Calcul (din cap. 1, ex. 5):**
- baza /24 (clasa C), masca /26
- hosturi/subnet = 2^(32-26) - 2 = 62
- borrowed bits = 26 - 24 = 2, deci 4 subnet-uri
- increment = 256 - 192 = 64

| Subnet | Network address | Interval hosturi | Broadcast |
|---|---|---|---|
| 1 | 192.168.0.0 | 192.168.0.1 - 192.168.0.62 | 192.168.0.63 |
| 2 | 192.168.0.64 | 192.168.0.65 - 192.168.0.126 | 192.168.0.127 |
| 3 | 192.168.0.128 | 192.168.0.129 - 192.168.0.190 | 192.168.0.191 |
| 4 | 192.168.0.192 | 192.168.0.193 - 192.168.0.254 | 192.168.0.255 |

Se testează primele 3 subnet-uri.

### A) Adrese din același interval (ping reușit)
Masca /26 pe toate nodurile.

| Subnet | PC1 | PC2 | PC3 |
|---|---|---|---|
| 1 | 192.168.0.1/26 | 192.168.0.2/26 | 192.168.0.3/26 |
| 2 | 192.168.0.65/26 | 192.168.0.66/26 | 192.168.0.67/26 |
| 3 | 192.168.0.129/26 | 192.168.0.130/26 | 192.168.0.131/26 |

Rezultat obținut pentru subnet-ul 1 (din PC1):
```
PC1> ping 192.168.0.2
84 bytes from 192.168.0.2 icmp_seq=1 ttl=64 time=2.316 ms
PC1> ping 192.168.0.3
84 bytes from 192.168.0.3 icmp_seq=1 ttl=64 time=1.918 ms
```
Ping-ul reușește: nodurile sunt în aceeași rețea (192.168.0.0/26) și comunică direct prin switch.

### B) Adrese din intervale diferite (ping eșuat)
PC3 a fost mutat în alt subnet (192.168.0.129/26), PC1 a rămas pe 192.168.0.1/26:
```
PC1> ping 192.168.0.129
No gateway found
```
Ping-ul eșuează: adresele sunt în subnet-uri diferite (192.168.0.0/26 și 192.168.0.128/26), iar nodul nu are gateway configurat, deci pachetul nu este trimis. Switch-ul nu rutează între subnet-uri.

## Observații
- Pentru comunicare directă, nodurile trebuie să fie în același subnet și să aibă aceeași mască.
- Nu se folosesc pe noduri adresele de network (.0, .64, .128) și de broadcast (.63, .127, .191).
- Masca trebuie dată pe toate nodurile; fără prefix, VPCS folosește /24.
- Primul ping poate da timeout (ARP), următoarele reușesc.
- Cazurile 2-6 se testează la fel, cu intervalele din cap. 1, ex. 5.