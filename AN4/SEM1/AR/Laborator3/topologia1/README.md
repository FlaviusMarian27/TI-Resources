# Lucrarea 3 - Administrarea rețelelor
## 2. Noduri VPCS și nodul NAT

## Scop
Conectarea unui nod VPCS la un nod NAT în GNS3 și obținerea unei adrese IP automat prin DHCP.

## Topologie
```
PC1 (VPCS) ---- NAT1
```
Nodurile sunt pornite din butonul START din bara GNS3. Portul VPCS trece din DOWN (roșu) în UP (verde).

## Comenzi folosite (consola VPCS)
| Comandă | Rol |
|---|---|
| `?` | lista de comenzi |
| `ip ?` | ajutor pentru comanda `ip` |
| `show` | afișează IP/mască/gateway/MAC |
| `show arp` | tabela ARP |
| `ip dhcp` | cere adresă IPv4 prin DHCP |

## Pași și rezultate

**1. Verificare înainte de DHCP** (`show`)
```
PC1    0.0.0.0/0    0.0.0.0    00:50:79:66:68:00
```
Nodul nu are adresă IP asignată.

**2. Cerere DHCP** (`ip dhcp`)
```
DDORA IP 192.168.122.205/24 GW 192.168.122.1
```
Serverul DHCP din nodul NAT a oferit adresa, masca și gateway-ul. Schimbul DHCP e DORA: Discover, Offer, Request, Ack.

**3. Verificare după DHCP** (`show`)
```
PC1    192.168.122.205/24    192.168.122.1    00:50:79:66:68:00
```
Adresa a fost primită automat.

**4. Reluarea exercițiilor din capitolul anterior**
De clarificat cu profesorul cum se interpretează (capitolul 1 conține doar calcule de adrese).

## Observații
- Fără `ip dhcp` sau `ip <adresă>/<prefix>`, nodul VPCS nu are IP.
- Nodurile VPCS nu se conectează la un nod Cloud (conflicte de MAC în rețeaua externă).
- NAT-ul trebuie pornit, altfel nu răspunde nimeni la DHCP.