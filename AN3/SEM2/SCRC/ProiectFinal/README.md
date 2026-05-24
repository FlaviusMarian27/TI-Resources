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
[cite_start]Toate legăturile au fost realizate exact în următoarea ordine, respectând porturile specifice pentru a facilita configurarea ulterioară [cite: 113-117]:

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

Oricine dorește să replice această configurație trebuie să deschidă consola CLI a fiecărui switch și să introducă blocurile de comenzi de mai jos.

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

După rularea comenzilor de mai sus, oricine poate verifica dacă configurarea a fost făcută corect folosind următoarele comenzi în modul privilegiat (Switch#):

După rularea comenzilor de mai sus, verifică configurarea în modul privilegiat (`Switch#`):

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

[cite_start]**Obiectiv:** Permiterea comunicării între diferitele VLAN-uri prin intermediul unei singure conexiuni fizice la router, folosind tehnica "Router-on-a-Stick" [cite: 26-27].

---

### 1. Noțiuni Teoretice pe Scurt
* **De ce avem nevoie de router?** Echipamentele din VLAN-uri diferite funcționează ca și cum ar fi în rețele complet separate. Un switch normal (Layer 2) nu știe cum să trimită mesaje dintr-o rețea în alta. Pentru asta este nevoie de un echipament de rutare (Layer 3).
* **Ce este Router-on-a-Stick?** În loc să tragem câte un cablu fizic din switch în router pentru fiecare departament în parte (ceea ce ar costa mult și ar consuma rapid porturile), tragem un singur cablu configurat ca Trunk. Pe router, tăiem portul fizic respectiv în porturi logice numite **subinterfețe** (`.10`, `.20`, etc.). [cite_start]Fiecare subinterfață va acționa ca un Gateway (poartă de ieșire) pentru VLAN-ul ei [cite: 27-28].
* **Encapsulation dot1Q:** Este comanda prin care routerul învață să "citească" și să "lipească" etichetele specifice standardului 802.1Q fiecărui pachet de date, asigurându-se astfel că mesajul pleacă spre departamentul corect.

---

### 2. Ghid de Configurare Pas cu Pas

**Atenție:** Atunci când accesați CLI-ul unui router nou și apare mesajul `Would you like to enter the initial configuration dialog? [yes/no]:`, tastați mereu **no** pentru a putea face configurările manual.

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