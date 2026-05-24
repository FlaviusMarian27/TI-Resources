# Pasul 1: Topologia Fizică și Conectarea Echipamentelor

[cite_start]**Obiectiv:** Implementarea topologiei hardware conform scenariului, delimitând rețeaua internă a firmei de rețeaua externă (ISP / Internet)[cite: 95, 103].

### 1. Echipamente utilizate
[cite_start]Conform cerințelor obligatorii[cite: 106]:
* **Rețeaua internă (Firma):** 
  * 1x Router (Cisco 1941) - `Router_Firma`
  * 2x Switch-uri (Cisco 2960) - `SW1` și `SW2`
  * 3x PC-uri - `PC1-IT`, `PC2-HR`, `PC3-Guest`
  * 1x Server - `Server-Intern`
* **Rețeaua externă (ISP):** 
  * 1x Router (Cisco 1941) - `Router_ISP`
  * 1x Server - `Server-Extern`

### 2. Tabelul Conexiunilor Fizice
* Toate legăturile au fost realizate exact în următoarea ordine, respectând porturile specifice pentru a facilita configurarea ulterioară:

| Ordine | Echipament Sursă | Port Sursă | Echipament Destinație | Port Destinație | Tip Cablu | Observații |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | PC1-IT | FastEthernet0 | SW1 | FastEthernet0/1 | Copper Straight-Through | Cablu rețea standard |
| **2** | PC2-HR | FastEthernet0 | SW1 | FastEthernet0/2 | Copper Straight-Through | Cablu rețea standard |
| **3** | PC3-Guest | FastEthernet0 | SW2 | FastEthernet0/2 | Copper Straight-Through | Cablu rețea standard |
| **4** | Server-Intern | FastEthernet0 | SW2 | FastEthernet0/4 | Copper Straight-Through | Cablu rețea standard |
| **5** | SW1 | FastEthernet0/3 | SW2 | FastEthernet0/1 | Copper Straight-Through | Legătură Trunk între switch-uri |
| **6** | SW2 | GigabitEthernet0/1| Router_Firma | GigabitEthernet0/0| Copper Straight-Through | Legătură Trunk spre router |
| **7** | Router_ISP | Serial0/1/0 | Router_Firma | Serial0/1/0 | Serial DCE (roșu) | Necesită modul **HWIC-2T**. *Ceasul se află pe Router_ISP*. |
| **8** | Router_ISP | GigabitEthernet0/1| Server-Extern | FastEthernet0 | Copper Straight-Through | Legătură server public |

**Note tehnice privind cablarea:**
* S-a folosit cablu **Copper Straight-Through** (negru continuu) pentru conexiunile locale LAN (de mare viteză).
* Pentru conexiunea WAN dintre rețeaua firmei și ISP, routerele au fost oprite temporar pentru instalarea modulelor hardware **HWIC-2T**. Conexiunea s-a realizat cu un cablu **Serial DCE**, inițiat dinspre `Router_ISP` spre `Router_Firma`, stabilind astfel ISP-ul ca echipament care va dicta lățimea de bandă (clock rate).

---

# Faza 2: Configurarea Switch-urilor (VLAN și Trunking)

**Obiectiv:** Segmentarea logică a rețelei fizice în 4 rețele virtuale izolate (VLAN-uri) pentru a grupa echipamentele pe departamente. Configurarea porturilor în modurile **Access** (pentru stațiile finale) și **Trunk** (pentru legăturile dintre echipamentele de rețea).

---

### 1. Noțiuni Teoretice pe Scurt
* **VLAN (Virtual Local Area Network):** O tehnologie care permite "felierea" unui switch fizic în mai multe switch-uri virtuale independente. Calculatoarele din VLAN-uri diferite nu pot comunica direct între ele la nivelul Switch-ului, oferind securitate nativă.
* **Port de Access:** Un port configurat să aparțină unui singur VLAN. Se folosește exclusiv pentru conectarea dispozitivelor finale (PC-uri, Imprimante, Servere).
* **Port de Trunk:** O „autostradă” de date. Un port special care permite trecerea pachetelor din toate VLAN-urile simultan prin același cablu fizic, folosind tag-uri de identificare (standardul 802.1Q).

---

### 2. Ghid de Configurare Pas cu Pas

* Oricine dorește să replice această configurație trebuie să deschidă consola CLI a fiecărui switch și să introducă blocurile de comenzi de mai jos.

#### 🖥️ Pasul A: Configurația pe Switch-ul 1 (SW1)
Pe acest switch creăm cele 4 VLAN-uri, asociem portul `Fa0/1` pentru PC-ul de IT, portul `Fa0/2` pentru PC-ul de HR și deschidem portul de Trunk `Fa0/3` către celălalt switch.

```cisco
enable
configure terminal

! --- 1. Crearea și denumirea VLAN-urilor obligatorii ---
vlan 10
name IT
vlan 20
name HR
vlan 30
name GUEST
vlan 40
name SERVERS
exit

! --- 2. Alocarea porturilor de Access pentru utilizatori ---
! Portul Fa0/1 devine membru exclusiv în VLAN 10 (IT)
interface fa0/1
switchport mode access
switchport access vlan 10
exit

! Portul Fa0/2 devine membru exclusiv în VLAN 20 (HR)
interface fa0/2
switchport mode access
switchport access vlan 20
exit

! --- 3. Configurarea portului de interconectare (Trunk) ---
! Portul Fa0/3 face legătura cu SW2 și va transporta toate VLAN-urile
interface fa0/3
switchport mode trunk
exit

! --- 4. Salvarea permanentă a setărilor ---
end
copy running-config startup-config
```

#### 🖥️ Pasul B: Configurația pe Switch-ul 2 (SW2)
Pe al doilea switch se repetă crearea VLAN-urilor. Alocăm portul Fa0/2 pentru PC-ul de Guest, portul Fa0/4 pentru Serverul Intern și setăm ambele porturi magistrale (Fa0/1 spre celălalt switch și Gig0/1 spre router) ca Trunk.

```
enable
configure terminal

! --- 1. Crearea și denumirea VLAN-urilor obligatorii ---
vlan 10
name IT
vlan 20
name HR
vlan 30
name GUEST
vlan 40
name SERVERS
exit

! --- 2. Alocarea porturilor de Access pentru utilizatori și servere ---
! Portul Fa0/2 devine membru exclusiv în VLAN 30 (GUEST)
interface fa0/2
switchport mode access
switchport access vlan 30
exit

! Portul Fa0/4 devine membru exclusiv în VLAN 40 (SERVERS)
interface fa0/4
switchport mode access
switchport access vlan 40
exit

! --- 3. Configurarea porturilor de Trunk ---
! Portul Fa0/1 preia conexiunea Trunk venită de la SW1
interface fa0/1
switchport mode trunk
exit

! Portul Gig0/1 face legătura cu Router_Firma și trimite traficul spre rutare
interface gig0/1
switchport mode trunk
exit

! --- 4. Salvarea permanentă a setărilor ---
end
copy running-config startup-config
```
#### 3. Comenzi de Verificare (Cum validăm munca?)

* După rularea comenzilor de mai sus, oricine poate verifica dacă configurarea a fost făcută corect folosind următoarele comenzi în modul privilegiat (Switch#):

* După rularea comenzilor de mai sus, verifică configurarea în modul privilegiat (`Switch#`):

##### 1. `show vlan brief`

* **Rezultat așteptat:**
    * Cele 4 VLAN-uri (10, 20, 30, 40) apar cu statusul `active`
    * Pe SW1 → porturile VLAN 10 și 20 sunt vizibile
    * Pe SW2 → porturile VLAN 30 și 40 sunt vizibile
    * ⚠️ Porturile configurate ca Trunk **vor dispărea** din această listă — comportament normal

##### 2. `show interfaces trunk`
* **Rezultat așteptat:**
    * Afișează interfețele Trunk active cu encapsularea `802.1q`
    * SW1: `Fa0/3`
    * SW2: `Fa0/1` și `Gig0/1`

---

# Faza 3: Inter-VLAN Routing (Router-on-a-Stick)

**Obiectiv:** Permiterea comunicării între diferitele VLAN-uri prin intermediul unei singure conexiuni fizice la router, folosind tehnica "Router-on-a-Stick".

---

### 1. Noțiuni Teoretice pe Scurt
* **De ce avem nevoie de router?** Echipamentele din VLAN-uri diferite funcționează ca și cum ar fi în rețele complet separate. Un switch normal (Layer 2) nu știe cum să trimită mesaje dintr-o rețea în alta. Pentru asta este nevoie de un echipament de rutare (Layer 3).
* **Ce este Router-on-a-Stick?** În loc să tragem câte un cablu fizic din switch în router pentru fiecare departament în parte (ceea ce ar costa mult și ar consuma rapid porturile), tragem un singur cablu configurat ca Trunk. Pe router, tăiem portul fizic respectiv în porturi logice numite **subinterfețe** (`.10`, `.20`, etc.). [cite_start]Fiecare subinterfață va acționa ca un Gateway (poartă de ieșire) pentru VLAN-ul ei [cite: 27-28].
* **Encapsulation dot1Q:** Este comanda prin care routerul învață să "citească" și să "lipească" etichetele specifice standardului 802.1Q fiecărui pachet de date, asigurându-se astfel că mesajul pleacă spre departamentul corect.

---

### 2. Ghid de Configurare Pas cu Pas

* **Atenție:** Atunci când accesați CLI-ul unui router nou și apare mesajul `Would you like to enter the initial configuration dialog? [yes/no]:`, tastați mereu **no** pentru a putea face configurările manual.

#### 🖥️ Configurația pe `Router_Firma`

```cisco
enable
configure terminal

! --- 1. Activarea portului fizic magistral ---
! Subinterfețele nu pot funcționa dacă interfața principală este oprită
interface gig0/0
no shutdown
exit

! --- 2. Configurarea Default Gateway-urilor pentru fiecare VLAN ---
! Gateway pentru VLAN 10 - Departamentul IT
interface gig0/0.10
encapsulation dot1Q 10
ip address 192.168.10.1 255.255.255.0
exit

! Gateway pentru VLAN 20 - Departamentul HR
interface gig0/0.20
encapsulation dot1Q 20
ip address 192.168.20.1 255.255.255.0
exit

! Gateway pentru VLAN 30 - Rețeaua GUEST
interface gig0/0.30
encapsulation dot1Q 30
ip address 192.168.30.1 255.255.255.0
exit

! Gateway pentru VLAN 40 - Zona SERVERS
interface gig0/0.40
encapsulation dot1Q 40
ip address 192.168.40.1 255.255.255.0
exit

! --- 3. Salvarea permanentă a setărilor ---
end
copy running-config startup-config

```

### 3. `show ip interface brief`
* **Rezultat așteptat:**
    * Interfața `GigabitEthernet0/0` cu statusul `up/up`
    * Cele 4 subinterfețe (`.10`, `.20`, `.30`, `.40`) fiecare cu IP-ul atribuit și statusul `up/up`

---

# Faza 4: Configurarea Serviciilor Interne (DHCP, DNS, HTTP)

**Obiectiv:** Configurarea `Server-Intern` pentru a oferi servicii web și rezoluție de nume (DNS), precum și alocarea dinamică de adrese IP (DHCP) pentru stațiile din rețea, folosind funcția de DHCP Relay pe router.

### 1. Configurarea de bază a Serverului Intern
* Deoarece acest server oferă servicii esențiale, i-a fost alocat un IP static în VLAN 40:
* **IP Address:** `192.168.40.10`
* **Subnet Mask:** `255.255.255.0`
* **Default Gateway:** `192.168.40.1`
* **DNS Server:** `192.168.40.10` (serverul acționează ca propriul său DNS)

### 2. Activarea Serviciilor Web și DNS
* **HTTP/HTTPS:** Serviciile au fost activate (On) pentru a permite accesarea paginii web a firmei.
* **DNS:** A fost creat un *A Record* care mapează numele `www.firma.local` la adresa IP a serverului (`192.168.40.10`).

### 3. Configurarea Bazinelor DHCP (DHCP Pools)
* Pe server au fost create 3 bazine distincte pentru a oferi IP-uri automate departamentelor:

| Nume Pool | Default Gateway | DNS Server | Start IP | Subnet Mask | Max Users |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **IT_POOL** | 192.168.10.1 | 192.168.40.10 | 192.168.10.2 | 255.255.255.0 | 100 |
| **HR_POOL** | 192.168.20.1 | 192.168.40.10 | 192.168.20.2 | 255.255.255.0 | 100 |
| **GUEST_POOL**| 192.168.30.1 | 192.168.40.10 | 192.168.30.2 | 255.255.255.0 | 100 |

### 4. Configurarea DHCP Relay (IP Helper) pe Router
* Deoarece broadcast-urile DHCP nu trec de la sine dintr-un VLAN în altul (clienții fiind în VLAN 10, 20, 30, iar serverul în VLAN 40), am configurat funcția de DHCP Relay pe subinterfețele routerului `Router_Firma`:

```cisco
enable
configure terminal

interface gig0/0.10
ip helper-address 192.168.40.10
exit

interface gig0/0.20
ip helper-address 192.168.40.10
exit

interface gig0/0.30
ip helper-address 192.168.40.10
exit

end
copy running-config startup-config
```

---

## Faza 5: Configurarea Rețelei Externe (ISP) și a Conexiunii WAN

**Obiectiv:** Stabilirea legăturii fizice și alocarea adreselor IP publice pentru conexiunea dintre rețeaua firmei și furnizorul de Internet, inclusiv configurarea adresei pentru Serverul Extern.

### 1. Configurarea interfeței WAN pe Router_Firma
* Am configurat portul Serial ce face legătura cu ISP-ul cu prima adresă IP din clasa publică alocată (`220.110.0.0/30`):

```cisco
enable
configure terminal

interface serial0/1/0
ip address 220.110.0.1 255.255.255.252
no shutdown
exit

end
copy running-config startup-config
```

### 2. Configurarea interfețelor pe Router_ISP
* Pe routerul furnizorului am setat capătul opus al legăturii WAN (inclusiv stabilirea ratei de ceas clock rate 64000, Router_ISP fiind echipamentul DCE) și interfața LAN către rețeaua de Internet simulată (10.0.0.0/8):

```
enable
configure terminal

! Configurarea interfeței WAN (spre Firmă)
interface serial0/1/0
ip address 220.110.0.2 255.255.255.252
clock rate 64000
no shutdown
exit

! Configurarea interfeței LAN (spre Server-Extern)
interface gig0/1
ip address 10.0.0.1 255.0.0.0
no shutdown
exit

end
copy running-config startup-config
```
### 3. Configurarea Serverului Extern

* Pentru a simula o resursă din Internet, Serverul Extern a primit o adresă IP statică din clasa publică alocată de ISP:

| Parametru | Valoare |
|---|---|
| IPv4 Address | `10.0.0.10` |
| Subnet Mask | `255.0.0.0` |
| Default Gateway | `10.0.0.1` |

**Verificare:** Link-ul fizic (cablul Serial) a devenit activ (`up/up`), confirmând stabilitatea conexiunii dintre Firmă și ISP.

---

# Faza 6: Rutarea Dinamică (OSPF)

**Obiectiv:** Configurarea protocolului de rutare OSPF pentru ca routerul firmei să poată descoperi dinamic calea către rețeaua externă (Internet), iar ISP-ul să cunoască rețeaua de legătură WAN.

### 1. Configurarea OSPF pe Router_Firma
* Am activat procesul OSPF și am anunțat strict rețeaua de legătură WAN (folosind Wildcard Mask `0.0.0.3` aferentă măștii `/30`). Rețelele interne (VLAN-urile) nu au fost anunțate în OSPF pentru a respecta cerințele de securitate și pregătirea pentru NAT.
```cisco
enable
configure terminal

! Activam procesul OSPF 1
router ospf 1
! Anuntam reteaua legaturii WAN catre ISP
network 220.110.0.0 0.0.0.3 area 0
exit

end
copy running-config startup-config
```

### 2. Configurarea OSPF pe Router_ISP
* Pe routerul furnizorului am anunțat atât legătura WAN, cât și rețeaua Internetului simulat (10.0.0.0/8 cu Wildcard 0.255.255.255), permițând astfel propagarea rutei externe către routerul firmei.

```cisco
enable
configure terminal

! Activam procesul OSPF 1
router ospf 1
! Anuntam reteaua legaturii WAN
network 220.110.0.0 0.0.0.3 area 0
! Anuntam reteaua in care se afla Server-Extern
network 10.0.0.0 0.255.255.255 area 0
exit

end
copy running-config startup-config
```

* **Verificare:**
    * Afișarea tabelei de rutare pe Router_Firma (folosind comanda show ip route) a validat succesul configurării. Ruta 10.0.0.0/8 apare învățată prin protocolul OSPF (notată cu litera O), iar adiacența între vecini a fost stabilită cu succes (LOADING to FULL).

---

# Faza 7: NAT Overload / PAT (Network Address Translation)

**Obiectiv:** Configurarea traducerii adreselor de rețea (NAT) pe `Router_Firma` pentru a permite stațiilor cu adrese IP private din VLAN-urile interne să comunice cu exteriorul (Internetul).

### 1. Definirea interfețelor (Inside / Outside)
Am specificat direcția traficului marcând interfața WAN ca ieșire (`outside`) și subinterfețele VLAN-urilor ca intrare (`inside`).

```cisco
enable
configure terminal

! 1. Definim interfața de iesire spre Internet (Outside)
interface serial0/1/0
ip nat outside
exit

! 2. Definim interfețele de intrare dinspre firmă (Inside)
interface gig0/0.10
ip nat inside
exit

interface gig0/0.20
ip nat inside
exit

interface gig0/0.30
ip nat inside
exit

interface gig0/0.40
ip nat inside
exit

! 3. Creăm lista de acces (ACL 1) cu rețelele care au voie să fie traduse
! Folosim wildcard mask (inversul lui 255.255.255.0)
access-list 1 permit 192.168.10.0 0.0.0.255
access-list 1 permit 192.168.20.0 0.0.0.255
access-list 1 permit 192.168.30.0 0.0.0.255
access-list 1 permit 192.168.40.0 0.0.0.255

! 4. Activăm regula generală de NAT Overload
ip nat inside source list 1 interface serial0/1/0 overload

! Ieșim și salvăm
end
copy running-config startup-config
```

### 2. Crearea listei de acces pentru NAT (ACL)
*   Am creat o listă de acces standard (ACL 1) care definește exact ce clase de IP-uri private sunt autorizate să iasă pe Internet (VLAN 10, 20, 30 și 40), folosind wildcard mask.

```cisco CLI
access-list 1 permit 192.168.10.0 0.0.0.255
access-list 1 permit 192.168.20.0 0.0.0.255
access-list 1 permit 192.168.30.0 0.0.0.255
access-list 1 permit 192.168.40.0 0.0.0.255
```

### 3. Activarea PAT (Port Address Translation)
* Am legat lista de acces (traficul permis) de interfața externă (serial0/1/0), adăugând parametrul overload pentru a masca toate adresele interne sub singura adresă IP publică disponibilă.

```cisco CLI
ip nat inside source list 1 interface serial0/1/0 overload
exit
copy running-config startup-config
```

**Verificare:**
* S-a inițiat un test de conectivitate *(Ping)* de pe PC1-IT (IP privat: *192.168.10.2*) către *Server-Extern* (IP public: *10.0.0.10*). Răspunsul favorabil confirmă că routerul preia pachetele, le traduce corect și le rutează înapoi către sursă.

---

# Faza 8: Securitate și Reguli de Acces (ACL-uri)

**Obiectiv:** Implementarea politicilor de securitate între VLAN-uri folosind liste de acces extinse (Extended ACLs) pe router, pentru a restrânge traficul nedorit și a permite doar serviciile necesare.

### 1. Definirea Politicilor pentru Departamentul HR (ACL 120)
* Conform cerințelor, departamentul HR nu trebuie să aibă acces la rețeaua IT, dar trebuie să poată comunica cu restul resurselor (Servere, Guest, Internet).

```cisco
enable
configure terminal

! =========================================================
! ACL 120 pentru HR (VLAN 20)
! =========================================================
! Regula 5: HR nu acceseaza IT
access-list 120 deny ip 192.168.20.0 0.0.0.255 192.168.10.0 0.0.0.255

! Permitem restul traficului (HR poate accesa servere, net, guest, etc.)
access-list 120 permit ip any any

! =========================================================
! ACL 130 pentru GUEST (VLAN 30)
! =========================================================
! Regula 1: Guest nu acceseaza IT
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.10.0 0.0.0.255

! Regula 2: Guest nu acceseaza HR
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.20.0 0.0.0.255

! Regula 3: Guest are acces la server DOAR prin HTTP (port 80)
access-list 130 permit tcp 192.168.30.0 0.0.0.255 host 192.168.40.10 eq 80

! Extra (necesar tehnic): Guest are voie DNS (port 53) ca sa traduca numele site-ului
access-list 130 permit udp 192.168.30.0 0.0.0.255 host 192.168.40.10 eq 53

! Regula 4: Blocam restul traficului spre rețeaua de servere (Asta blocheaza implicit PING-ul)
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.40.0 0.0.0.255

! Regula 7: Guest are acces la Internet (permitem orice altceva a ramas)
access-list 130 permit ip any any

! =========================================================
! Aplicarea listelor "la usa" routerului (pe subinterfețe)
! =========================================================

! Aplicam "paza" la intrarea traficului dinspre HR
interface gig0/0.20
ip access-group 120 in
exit

! Aplicam "paza" la intrarea traficului dinspre GUEST
interface gig0/0.30
ip access-group 130 in
exit

! Regula 6: IT are acces complet (nu punem niciun ACL pe gig0/0.10)

end
copy running-config startup-config
```

### 2. Izolarea Vizitatorilor / GUEST (ACL 130)

* Rețeaua destinată oaspeților este cea mai restrictivă — nu au acces în rețelele angajaților (IT și HR) și au acces extrem de limitat la serverul intern.

#### Logica regulilor (în ordine)

| # | Regulă | Detaliu |
|---|---|---|
| 1 | **Blocare către departamente** | Respinge orice trafic dinspre Guest către IT și HR |
| 2 | **Permisiune HTTP (Web)** | Permite TCP port 80 doar către `192.168.40.10` (serverul intern) |
| 3 | **Permisiune DNS** | Permite UDP port 53 către `192.168.40.10` — necesar pentru rezolvarea `www.firma.local` |
| 4 | **Blocare generală spre Servere** | Taie orice alt trafic dinspre Guest către `192.168.40.0` (inclusiv Ping) |
| 5 | **Liber spre Internet** | Tot ce nu a fost interzis mai sus este permis |

### 3. Configurare ACL 130

```cisco
! Definim lista 130 pentru GUEST
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.10.0 0.0.0.255
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.20.0 0.0.0.255
access-list 130 permit tcp 192.168.30.0 0.0.0.255 host 192.168.40.10 eq 80
access-list 130 permit udp 192.168.30.0 0.0.0.255 host 192.168.40.10 eq 53
access-list 130 deny ip 192.168.30.0 0.0.0.255 192.168.40.0 0.0.0.255
access-list 130 permit ip any any
```

### 4. Aplicarea regulilor „La Ușă"

* Listele de acces sunt inutile dacă nu sunt atribuite unei interfețe. Regulile se aplică pe subinterfețele corespunzătoare fiecărui VLAN, în direcția **IN** (la intrare) — routerul verifică și distruge pachetele interzise **exact în momentul în care acestea intră în router**, fără să le mai lase să circule prin restul rețelei.

> ⚠️ Rețeaua IT nu are niciun ACL aplicat, bucurându-se de acces complet conform cerințelor.

```cisco
! Aplicam filtrul pe "teava" departamentului HR
interface gig0/0.20
ip access-group 120 in
exit

! Aplicam filtrul pe "teava" vizitatorilor
interface gig0/0.30
ip access-group 130 in
exit

end
copy running-config startup-config
```

### 5. Verificare

#### PC3-Guest — Command Prompt

| Comandă | Rezultat așteptat | Motiv |
|---|---|---|
| `ping 192.168.10.2` | `Destination host unreachable` | Blocat de Regula 1 |
| `ping 192.168.40.10` | `Destination host unreachable` | Blocat de Regula 4 |
| `ping 10.0.0.10` | `Reply` | Permis de Regula 5 |

#### PC3-Guest — Web Browser

| URL | Rezultat așteptat | Motiv |
|---|---|---|
| `http://www.firma.local` | Pagina web a Packet Tracer apare | Permis de Regula 3 |

#### PC2-HR — Command Prompt

| Comandă | Rezultat așteptat | Motiv |
|---|---|---|
| `ping 192.168.10.2` | `Destination host unreachable` | Blocat de Regula 5 |

### 5. **Concluzie Testare**

* Testele efectuate din command prompt și browser confirmă că izolarea funcționează perfect: Ping-urile interzise primesc răspunsul Destination host unreachable direct de la gateway, în timp ce navigarea web și accesul spre Internet au rămas complet funcționale.