# Lucrarea 3 - Administrarea rețelelor
## 4. Packet Capture: vizualizare pachete DHCP, ICMP etc. (README detaliat)

Document de studiu pentru partea 4 a lucrării: ce se cere, teoria necesară, pașii cu comenzi, ce ar trebui observat în Wireshark și explicațiile pentru întrebările din lucrare. Exemplele sunt din capturile făcute în laborator.

---

## Cuprins
1. Teorie necesară
2. Pregătirea mediului
3. Partea A: rețea fără NAT (pașii 1-3)
4. Partea B: rețea cu NAT (pașii 4-5)
5. Tabel final broadcast vs unicast
6. Capcane de examen
7. Rezumat comenzi și filtre Wireshark

---

## 1. Teorie necesară

### 1.1 Adresa MAC vs adresa IP
- **IP** identifică nodul în rețea (logic). **MAC** identifică placa de rețea (fizic) și se folosește pentru livrarea cadrului în interiorul aceleiași rețele.
- Un cadru Ethernet are nevoie de **MAC-ul destinației**. Dacă nodul știe doar IP-ul, trebuie să afle MAC-ul prin **ARP**.

### 1.2 ARP (Address Resolution Protocol)
- **ARP request:** "Cine are IP-ul X? Spune-i lui Y." Se trimite în **broadcast** (`ff:ff:ff:ff:ff:ff`).
- **ARP reply:** "IP-ul X este la MAC-ul Z." Se trimite **unicast**, direct către cel care a întrebat.
- **Gratuitous ARP:** un nod își anunță propria adresă IP, în broadcast (apare când își setează sau își primește un IP nou și verifică dacă adresa e duplicat).

### 1.3 Broadcast vs unicast
- **Broadcast:** MAC destinație `ff:ff:ff:ff:ff:ff` (Wireshark scrie `Broadcast`) sau IP destinație `255.255.255.255`. Ajunge la **toate nodurile** din rețea.
- **Unicast:** are un singur destinatar (MAC concret).

### 1.4 Cum se comportă switch-ul
- Switch-ul **învață** adresele MAC: când primește un cadru pe un port, memorează "MAC-ul sursă este pe acest port".
- Cadrele **broadcast** le trimite pe **toate porturile** (mai puțin cel de intrare).
- Cadrele **unicast** către un MAC cunoscut le trimite **doar pe portul destinației**.
- Switch-ul **nu rutează** între subnet-uri diferite. Doar comută cadre în interiorul unei rețele.

### 1.5 ICMP, ping, TTL, traceroute
- **ICMP echo request / reply** = pachetele comenzii `ping`.
- **TTL** (Time To Live) scade cu 1 la fiecare router. Când ajunge la 0, routerul aruncă pachetul și trimite înapoi **ICMP "Time-to-live exceeded"**.
- **Traceroute** (în VPCS: `trace`) trimite pachete cu TTL = 1, 2, 3 etc. și află routerele din răspunsurile "TTL exceeded".

### 1.6 DHCP (schimbul DORA)
Serverul DHCP (aici, nodul NAT) oferă automat IP, mască, gateway:
1. **D**iscover: clientul caută un server (broadcast, IP sursă `0.0.0.0`, destinație `255.255.255.255`).
2. **O**ffer: serverul oferă o adresă.
3. **R**equest: clientul cere oficial adresa oferită (broadcast).
4. **A**CK: serverul confirmă.

### 1.7 Decizia "local sau gateway"
Înainte să trimită un pachet, nodul aplică masca proprie pe adresa proprie și pe cea a destinației:
- **aceeași rețea** -> trimite direct (ARP pentru destinație, apoi cadrul);
- **rețea diferită** -> trimite la **gateway** (ARP pentru gateway);
- **rețea diferită și fără gateway** -> nu trimite nimic (în VPCS: `No gateway found`).

---

## 2. Pregătirea mediului

1. Topologia din partea 3: **Switch1** + **PC1, PC2, PC3** (noduri VPCS).
2. Oprești și repornești toate nodurile (nu păstrezi adrese IP vechi).
3. **Click dreapta pe fiecare cablu -> Start capture.** Se deschide câte un Wireshark pentru fiecare linie.
4. Aranjezi ferestrele una lângă alta, ca să vezi toate capturile simultan.
5. În fiecare fereastră, în bara "Apply a display filter", pui:
```
dhcp || arp || icmp
```
(în versiuni mai vechi de Wireshark, DHCP se numește `bootp`).

**Notă:** pasul 2 din lucrare vorbește despre "cele 4 linii", dar înainte de a adăuga NAT-ul rețeaua are doar **3 cabluri**. A patra linie apare la pasul 4, când se adaugă NAT-ul.

---

# 3. Partea A: rețea fără NAT

Topologie:
```
       Switch1
      /   |   \
    PC1  PC2  PC3
```

## Pasul 3.1: adrese IP din același subnet

**Cerință:** dați celor 3 noduri câte un IP din același subnet, astfel încât să comunice; verificați cu ping.

**Comenzi:**
```
PC1> ip 192.168.0.1/26
PC2> ip 192.168.0.2/26
PC3> ip 192.168.0.3/26
```
Verificare:
```
PC1> show
PC1> ping 192.168.0.2
PC1> ping 192.168.0.3
```

**Explicație:** cele 3 adrese sunt în subnet-ul 192.168.0.0/26 (hosturi .1-.62), deci comunică direct prin switch, fără gateway. Masca trebuie să fie **aceeași pe toate nodurile**.

*Observație:* când dai `ip ...`, VPCS afișează "Checking for duplicate address...", adică trimite un ARP pentru a verifica dacă adresa e deja folosită. Pachetul apare în capturi înainte de ping.

## Pasul 3.2: ping PC1 -> PC3

**Cerință:** dați ping de la nodul 1 la nodul 3; apar pachetele ICMP pe toate liniile? Argumentați.

**Comenzi:** (opțional, înainte, ca să vezi din nou ARP-ul: `clear arp` pe PC1 și PC3)
```
PC1> ping 192.168.0.3
```

**Exemplu: captura de pe linia PC1-Switch1:**

| Nr. | Source | Destination | Protocol | Info |
|---|---|---|---|---|
| 1 | 00:50:79:66:68:00 | Broadcast | ARP | Who has 192.168.0.3? Tell 192.168.0.1 |
| 2 | 00:50:79:66:68:02 | 00:50:79:66:68:00 | ARP | 192.168.0.3 is at 00:50:79:66:68:02 |
| 3-12 | 192.168.0.1 <-> 192.168.0.3 | | ICMP | 5 perechi Echo (ping) request / reply, seq 1-5 |

**Exemplu: captura de pe linia PC2-Switch1:** apare doar pachetul 1 (ARP request, broadcast). Nu apar ARP reply și nici ICMP.

**Răspuns la întrebare: NU, pachetele ICMP nu apar pe toate liniile.**

**Argumentare:**
1. PC1 nu cunoaște MAC-ul lui PC3, deci trimite ARP request în **broadcast**.
2. Switch-ul trimite broadcast-ul pe **toate porturile**, deci toate cele 3 linii îl văd. PC2 vede cererea, observă că IP-ul nu e al lui și o ignoră.
3. PC3 răspunde cu ARP reply **unicast**. Switch-ul a învățat deja MAC-ul lui PC1 (din ARP request) și îl trimite doar pe portul lui PC1.
4. De aici înainte, ICMP-ul este **unicast** între PC1 și PC3 (cunoscute în tabela MAC a switch-ului), deci apare doar pe liniile **PC1 și PC3**.

## Pasul 3.3: ping către un IP inexistent

**Cerință:** ping de la nodul 1 către un IP inexistent; explicați ce se întâmplă.

**Comenzi:**
```
PC1> ping 192.168.0.8
```

**Exemplu: ce apare în captură** (3 pachete identice, la ~1 secundă distanță):
```
ARP  Broadcast  Who has 192.168.0.8? Tell 192.168.0.1
ARP  Broadcast  Who has 192.168.0.8? Tell 192.168.0.1
ARP  Broadcast  Who has 192.168.0.8? Tell 192.168.0.1
```
Niciun ARP reply, niciun ICMP.

**Explicație:**
1. 192.168.0.8 este în același subnet cu PC1 (/26), deci PC1 o consideră locală și are nevoie de MAC-ul ei.
2. PC1 trimite ARP request în broadcast. **Nimeni nu are** adresa 192.168.0.8, deci nu răspunde nimeni.
3. PC1 retrimite cererea (3 încercări), apoi renunță.
4. Fără MAC-ul destinației nu poate construi cadrul Ethernet, deci pachetul **ICMP nu este trimis niciodată**.

**De reținut:** un ping către un IP inexistent din același subnet se oprește la **ARP**.

---

# 4. Partea B: rețea cu NAT

## Pasul 4: adăugarea nodului NAT

**Cerință:** opriți simularea, adăugați un nod NAT conectat la switch; înainte de pornire, aveți deschise capturi pe fiecare cablu.

**Pași:**
1. Oprești simularea (butonul STOP).
2. Tragi un nod **NAT** în topologie și îl legi la **Switch1**.
3. **Înainte de START:** click dreapta pe fiecare dintre cele **4 cabluri** (PC1, PC2, PC3, NAT) -> **Start capture**.
4. Pui filtrul `dhcp || arp || icmp` în fiecare fereastră.

Topologie:
```
        NAT1
          |
       Switch1
      /   |   \
    PC1  PC2  PC3
```

**De ce capturile se pornesc înainte de START:** altfel se pierd primele pachete (exact cele de la DHCP).

## Pasul 5: pornirea simulării
Apeși START și deschizi consola fiecărui nod VPCS. Adresele manuale de la partea A nu mai contează; `ip dhcp` le înlocuiește.

## Pasul 5.1: DHCP

**Cerință:** configurați pe rând fiecare nod prin DHCP și urmăriți schimbul de pachete dintre NAT și nod; priviți toate capturile simultan.

**Comenzi (pe rând, pe fiecare PC):**
```
PC1> ip dhcp
DDORA IP 192.168.122.205/24 GW 192.168.122.1

PC2> ip dhcp
DDORA IP 192.168.122.206/24 GW 192.168.122.1

PC3> ip dhcp
DDORA IP 192.168.122.207/24 GW 192.168.122.1
```
Verificare: `show`. (Dacă un nod are deja IP, `ip dhcp -x` face release înainte.)

**Exemplu: captura de pe linia NAT, pentru PC1:**

| Pachet | Descriere | Tip |
|---|---|---|
| DHCP Discover | 0.0.0.0 -> 255.255.255.255, apare de 2 ori (același Transaction ID) | broadcast |
| ARP "Who has 192.168.122.205? Tell 192.168.122.1" | NAT-ul verifică dacă adresa e liberă | broadcast |
| ICMP echo request | de la 192.168.122.1 către .205, fără răspuns ("no response found") | unicast |
| DHCP Offer | 192.168.122.1 -> 192.168.122.205, apare de 2 ori | unicast (IP) |
| DHCP Request | 0.0.0.0 -> 255.255.255.255 | broadcast |
| DHCP ACK | 192.168.122.1 -> 192.168.122.205 | unicast (IP) |
| Gratuitous ARP pentru 192.168.122.205 | de la PC1, 3 pachete | broadcast |

**Explicații:**
- **Schimbul DORA:** Discover -> Offer -> Request -> ACK.
- **De ce Discover și Request sunt broadcast:** PC1 nu are încă adresă IP și nu știe unde e serverul.
- **De ce apar 2 Discover și 2 Offer** (și `DDORA` în consolă): înainte să ofere adresa, serverul a verificat că nu e folosită (ARP + ping către .205); verificarea a durat câteva secunde. PC1 n-a primit răspuns în ~1 secundă și a retrimis Discover-ul (aceeași tranzacție), iar serverul a răspuns la fiecare.
- **Gratuitous ARP:** după ce primește adresa, PC1 o anunță în rețea.

**Exemplu de broadcast vizibil pe toate liniile:** în captura de pe linia PC1 apar pachete DHCP Request și Gratuitous ARP pentru 192.168.122.207, adică ale lui **PC3**. Se văd pe linia lui PC1 pentru că sunt broadcast, deci switch-ul le trimite pe toate porturile.

*De completat/verificat în Wireshark:* la Offer și ACK, MAC-ul destinației (Ethernet II -> Destination): dacă e al lui PC1, pachetul e unicast complet; dacă e `ff:ff:ff:ff:ff:ff`, e broadcast.

## Pasul 5.2: ping către Internet

**Cerință:** ping de pe nodul 1 către 8.8.8.8; se observă pachetele ICMP pe toate liniile? Argumentați.

**Comenzi:**
```
PC1> ping 8.8.8.8
84 bytes from 8.8.8.8 icmp_seq=1 ttl=117 time=21.808 ms
```

**Exemplu: captura de pe linia PC1-Switch1:**

| Pachet | Tip |
|---|---|
| ARP request "Who has 192.168.122.1? Tell 192.168.122.205" | broadcast |
| ARP reply "192.168.122.1 is at 52:54:00:cd:7e:3e" | unicast |
| 5 perechi ICMP echo request / reply, 192.168.122.205 <-> 8.8.8.8 | unicast |

**Explicații:**
- 8.8.8.8 este în afara subnet-ului, deci PC1 trimite pachetul la **gateway** (nodul NAT, 192.168.122.1). De aceea face ARP pentru gateway, nu pentru 8.8.8.8.
- Pe ICMP: **IP-ul destinației este 8.8.8.8**, dar **MAC-ul destinației este al NAT-ului** (`52:54:00:cd:7e:3e`). IP-ul rămâne același pe tot traseul, MAC-ul se schimbă la fiecare hop.
- **TTL:** request-ul pleacă cu ttl=64 (valoarea VPCS); reply-ul vine cu ttl=117 (setat de destinație, scăzut de routerele de pe drum).

**Răspuns la întrebare: NU, ICMP-ul nu apare pe toate liniile.** Este unicast, iar switch-ul îl trimite doar pe portul lui PC1 și pe cel al NAT-ului. Liniile PC2 și PC3 nu primesc pachetele ICMP (pot primi doar broadcast-ul ARP).

*De completat/verificat:* confirmă în capturile PC2 și PC3 că nu apare ICMP.

## Pasul 5.3: traceroute

**Cerință:** traceroute de pe fiecare nod (nu în paralel) către 8.8.8.8; urmăriți pachetele.

**Comenzi (pe rând, pe fiecare nod):**
```
PC1> trace 8.8.8.8
trace to 8.8.8.8, 8 hops max, press Ctrl+C to stop
 1   192.168.122.1
 2   192.168.1.1
 3   10.0.3.101
 4   10.30.4.129
 5   10.220.215.172
 6   10.221.100.110
 7   *  * 10.221.96.48
 8   *  *  *
```
(în VPCS comanda este `trace`, nu `traceroute`)

**Exemplu: captura de pe linia PC1:** serie de mesaje **ICMP "Time-to-live exceeded"**, câte 3 de la fiecare hop, toate către 192.168.122.205:

| Hop | Sursa mesajului |
|---|---|
| 1 | 192.168.122.1 (NAT, gateway) |
| 2 | 192.168.1.1 |
| 3 | 10.0.3.101 |
| 4 | 10.30.4.129 |
| 5 | 10.220.215.172 |
| 6 | 10.221.100.110 |

**Explicație:**
1. PC1 trimite probe către 8.8.8.8 cu TTL = 1, apoi 2, 3 etc. (câte 3 probe pentru fiecare valoare).
2. Routerul unde TTL-ul ajunge la 0 aruncă pachetul și răspunde cu ICMP "Time-to-live exceeded", cu **adresa lui ca sursă**. Așa se descoperă traseul.
3. VPCS trimite implicit probe **UDP**; acestea nu se văd cu filtrul `dhcp || arp || icmp` (de aceea numerele pachetelor sar din 2 în 2). Se văd dacă adaugi `|| udp`.
4. Toate pachetele din trace sunt **unicast**.

**Observații:**
- Comanda se oprește la **8 hop-uri** (`-m 8` implicit). Pentru traseu mai lung: `trace 8.8.8.8 -m 20`.
- Hop-ul 1 este mereu gateway-ul (NAT).
- Un ping cu TTL mic produce același efect: `ping 8.8.8.8 -T 5` a dat "TTL expired in transit" de la 10.220.215.172, adică exact hop-ul 5.

*De completat:* trace și de pe PC2 și PC3 (pe rând), cu observații din capturi.

## Pasul 5.4: IP din altă clasă pe un nod

**Cerință:** schimbați IP-ul unui nod cu unul din altă clasă; ping de pe acel nod pe altul și invers; explicați fenomenul.

**Comenzi:** nodurile au adrese clasa C (192.168.122.x/24). Pe PC1 se pune o adresă clasa A:
```
PC1> ip 10.0.0.5/8
PC1> show
```

**Exemplu: ce apare imediat în captură:** 3 pachete **Gratuitous ARP** pentru 10.0.0.5 (broadcast), la ~1 secundă distanță: PC1 își anunță noua adresă.

**Test A: PC1 -> PC2:**
```
PC1> ping 192.168.122.206
No gateway found
```
**Test B: PC2 -> PC1:**
```
PC2> ping 10.0.0.5
10.0.0.5 icmp_seq=1 timeout
10.0.0.5 icmp_seq=2 timeout
... (5 timeout-uri)
```

**Explicații:**
- **Test A:** PC1 (10.0.0.5/8) vede 192.168.122.206 în **altă rețea**. Nu are gateway (la setarea manuală fără gateway, cel primit prin DHCP s-a pierdut), deci nu trimite nimic. **Niciun pachet pe fir**, doar mesajul din consolă.
- **Test B:** PC2 are gateway (192.168.122.1) și vede 10.0.0.5 în afara subnet-ului lui, deci trimite pachetul **la NAT**, nu la PC1: ARP request în broadcast pentru gateway ("Who has 192.168.122.1? Tell 192.168.122.206"), apoi ICMP unicast către MAC-ul NAT-ului. NAT-ul nu poate livra pachetul la PC1, deci apare **timeout**. ICMP-ul nu apare pe linia lui PC1.

**Fenomenul, pe scurt:** nodurile sunt pe același switch, dar în **subnet-uri diferite**, deci nu comunică direct. Fiecare nod decide, după adresa și masca proprie, dacă destinația e locală sau trebuie trimisă la gateway. Switch-ul doar comută cadre în interiorul unei rețele, nu rutează între ele.

## Pasul 5.5: broadcast vs unicast, pe situații

| Situație | Pachete **broadcast** | Pachete **unicast** |
|---|---|---|
| Fără NAT: ping PC1 -> PC3 | ARP request | ARP reply, ICMP echo request / reply |
| Fără NAT: IP inexistent | ARP request (repetat de 3 ori) | - (niciun ICMP) |
| 5.1 DHCP | Discover, Request, ARP "Who has .205?" (NAT), Gratuitous ARP | ICMP echo de verificare (NAT), Offer, ACK (la nivel IP) |
| 5.2 ping 8.8.8.8 | ARP request pentru gateway | ARP reply, ICMP echo request / reply |
| 5.3 trace | - | probe UDP, ICMP "TTL exceeded", ARP NAT <-> PC1 |
| 5.4 IP din altă clasă | Gratuitous ARP pentru 10.0.0.5, ARP request pentru gateway (PC2) | ICMP către gateway; PC1 -> PC2 nu trimite nimic |

---

# 5. Tabel final: cum recunoști rapid tipul

| Pachet | Broadcast sau unicast | Apare pe |
|---|---|---|
| ARP request | broadcast | toate liniile |
| ARP reply | unicast | liniile celor doi participanți |
| Gratuitous ARP | broadcast | toate liniile |
| DHCP Discover | broadcast | toate liniile |
| DHCP Request | broadcast | toate liniile |
| DHCP Offer / ACK | unicast la nivel IP (MAC de verificat în captură) | linia NAT + linia clientului (cel puțin) |
| ICMP echo request / reply | unicast | liniile participanților |
| ICMP "TTL exceeded", probe trace | unicast | liniile participanților |

---

# 6. Capcane de examen

- **Switch-ul nu "întreabă" nimic:** ARP request-ul îl trimite **nodul** (ex. PC1); switch-ul doar îl retransmite pe toate porturile.
- **ICMP-ul unicast nu apare pe toate liniile**, doar pe cele ale participanților. ARP request-ul (broadcast) apare pe toate.
- **Ping către IP din alt subnet:** nu ajunge la ARP pentru acel IP; nodul fie trimite la gateway, fie, fără gateway, afișează `No gateway found`.
- **Ping către IP inexistent din același subnet:** doar ARP request-uri fără reply, niciun ICMP.
- **La ping 8.8.8.8:** IP-ul destinație este 8.8.8.8, dar MAC-ul destinației este al gateway-ului.
- **Două Discover și două Offer** nu sunt eroare: sunt o retransmisie (serverul verifica adresa), iar `DDORA` din consolă reflectă asta.
- **`trace`, nu `traceroute`** în VPCS; se oprește implicit la 8 hop-uri.
- **Un ping către propria adresă** nu testează nimic (time=0.001 ms).
- **Capturile se pornesc înainte de acțiune**, altfel se pierd pachetele.
- **`ip <adresă>/<prefix>` fără gateway** șterge gateway-ul primit prin DHCP; fără prefix, masca implicită este /24.

---

# 7. Rezumat comenzi și filtre

**Comenzi VPCS:**

| Comandă | Rol |
|---|---|
| `ip <adresă>/<prefix>` | IP manual (fără prefix: /24) |
| `ip dhcp` | IP prin DHCP (`-x` release, `-r` renew, `-d` decodare pachete) |
| `show` | afișează IP/mască/gateway/MAC |
| `show arp` / `clear arp` | tabela ARP / ștergere |
| `ping <adresă>` | testare (`-T <ttl>` setează TTL) |
| `trace <adresă>` | traseul pachetelor (`-m <ttl>` număr maxim de hop-uri) |

**Filtre Wireshark:**

| Filtru | Afișează |
|---|---|
| `dhcp \|\| arp \|\| icmp` | DHCP, ARP și ICMP (filtrul folosit în lucrare) |
| `dhcp` (sau `bootp`) | doar DHCP |
| `arp` / `icmp` / `udp` | doar ARP / ICMP / UDP |
| `eth.dst == ff:ff:ff:ff:ff:ff` | doar broadcast |
| `ip.addr == 192.168.122.205` | pachetele unui singur nod |