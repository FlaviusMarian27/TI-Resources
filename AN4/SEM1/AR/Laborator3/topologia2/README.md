# Lucrarea 3 - Administrarea rețelelor
## 2. Noduri VPCS și nodul NAT: topologia cu switch și 3 noduri VPCS

## Scop
Conectarea a 3 noduri VPCS printr-un switch la un nod NAT în GNS3, obținerea adreselor prin DHCP și verificarea conectivității (Internet și între noduri).

## Topologie
```
        NAT1
          |
       Switch1
      /   |   \
    PC1  PC3  PC2
```
Toate nodurile sunt pornite din butonul START din bara GNS3 (legăturile devin verzi).

## Pași

**1. DHCP pe fiecare nod VPCS**
```
PCx> ip dhcp
```
Exemplu (PC3):
```
DDORA IP 192.168.122.207/24 GW 192.168.122.1
```
Adrese primite: PC1 = 192.168.122.205, PC2 = 192.168.122.206, PC3 = 192.168.122.207. Gateway-ul (nodul NAT) este 192.168.122.1.

**2. Ping către Internet**
```
PC3> ping 8.8.8.8
84 bytes from 8.8.8.8 icmp_seq=1 ttl=117 time=21.808 ms
```
Conectivitatea cu Internetul funcționează.

**3. Traceroute (în VPCS: `trace`)**
```
PC3> trace 8.8.8.8
 1   192.168.122.1     (nodul NAT, gateway)
 2   192.168.1.1
 3   10.0.3.101
 4   10.30.4.129
 5   10.220.215.172
 6   10.221.100.110
 7   10.221.96.48
 8   * * *
```
Hop-ul 1 este gateway-ul NAT. Comanda se oprește la 8 hop-uri (valoarea implicită `-m 8`); pentru traseul complet se poate folosi `trace 8.8.8.8 -m 20`.

Observație: `ping 8.8.8.8 -T 5` setează TTL-ul la 5 și returnează "TTL expired in transit" de la 10.220.215.172, adică exact hop-ul 5 din trace.

**4. Ping între cele 3 noduri**
Din PC3:
```
PC3> ping 192.168.122.205   -> răspunsuri OK
PC3> ping 192.168.122.206   -> răspunsuri OK
```
La fel din PC1 și PC2. Cele 3 noduri comunică prin switch (același subnet 192.168.122.0/24).

## Observații
- Fiecare nod rulează separat `ip dhcp`.
- Ping-ul către propria adresă (ex. PC3 -> 192.168.122.207) nu testează conectivitatea (time=0.001 ms).
- NAT-ul trebuie pornit, altfel nodurile nu primesc IP.
- VPCS nu se conectează la un nod Cloud (conflicte de MAC).

