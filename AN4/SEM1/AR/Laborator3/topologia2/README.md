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

---

## 4. Packet Capture: vizualizare pachete DHCP, ICMP etc. (partea fără NAT)

## Scop
Observarea pachetelor (ARP, ICMP) într-o rețea simplă, formată dintr-un switch și 3 noduri VPCS, folosind Wireshark pornit pe cablurile de legătură.

## Topologie
```
       Switch1
      /   |   \
    PC1  PC2  PC3
```
Nodurile au fost oprite și repornite, fără adrese IP la început. Pe fiecare cablu (PC1-Switch1, PC2-Switch1, PC3-Switch1) s-a pornit câte o captură Wireshark (click dreapta pe cablu, Start capture).

## 3.1 Adrese IP din același subnet
```
PC1> ip 192.168.0.1/26
PC2> ip 192.168.0.2/26
PC3> ip 192.168.0.3/26
```
Conexiunea a fost verificată cu `ping` între noduri.

## 3.2 Ping PC1 -> PC3
```
PC1> ping 192.168.0.3
```
Captura de pe linia PC1-Switch1:

| Nr. | Pachet | Tip |
|---|---|---|
| 1 | ARP request: Who has 192.168.0.3? Tell 192.168.0.1 | broadcast |
| 2 | ARP reply: 192.168.0.3 is at 00:50:79:66:68:02 | unicast |
| 3-12 | 5 perechi ICMP echo request / reply (seq 1-5) | unicast |

Captura de pe linia PC2-Switch1: apare doar ARP request-ul (broadcast), fără ARP reply și fără ICMP.

**Răspuns: pachetele ICMP nu apar pe toate liniile.**
Argument: PC1 nu cunoaște MAC-ul lui PC3, deci trimite un ARP request în broadcast (ff:ff:ff:ff:ff:ff), pe care switch-ul îl transmite pe toate porturile. Doar PC3 răspunde, unicast. Switch-ul învață adresele MAC pe porturi și trimite traficul unicast (ARP reply, ICMP) doar pe portul destinației, adică pe liniile PC1 și PC3. PC2 vede doar broadcast-ul.

## 3.3 Ping către un IP inexistent
```
PC1> ping 192.168.0.8
```
Captura arată 3 ARP request-uri identice, în broadcast, la ~1 secundă distanță:
```
Who has 192.168.0.8? Tell 192.168.0.1
```
Nu apare niciun ARP reply și niciun pachet ICMP.

**Explicație:** adresa 192.168.0.8 este în același subnet (/26), deci PC1 o consideră locală și caută MAC-ul prin ARP. Niciun nod nu are acea adresă, deci nu răspunde nimeni. PC1 retrimite cererea (3 încercări), apoi renunță. Fără MAC-ul destinației nu se poate construi cadrul Ethernet, deci pachetul ICMP nu este trimis niciodată.

## Broadcast vs unicast (până aici)
| Pachet | Tip |
|---|---|
| ARP request | broadcast |
| ARP reply | unicast |
| ICMP echo request / reply | unicast |

## Observații
- Capturile trebuie pornite înainte de acțiune, altfel se pierd pachetele.
- Un IP din alt subnet nu ajunge la ARP: nodul fără gateway afișează `No gateway found` și nu trimite nimic pe fir.
- Urmează: adăugarea nodului NAT în switch (a patra captură), DHCP, ping 8.8.8.8, trace și IP din altă clasă (pașii 4-5 din lucrare).