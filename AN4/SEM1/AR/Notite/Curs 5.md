
# NAT și gateway (slide 60–61)

- **NAT** maschează și mapează adresele IP dintr-o rețea internă într-un alt spațiu de adrese extern. Modifică **header-ul IP** al pachetului, printr-un dispozitiv conectat la ambele rețele, un **router**.

- **Mecanism:** înlocuiește IP-ul clientului din LAN cu **IP-ul public al routerului**.

- Routerul ține o **tabelă de conexiuni**, ca să trimită răspunsurile la nodul corect.

- Nodurile publice **nu văd** rețeaua privată, doar IP-ul public al routerului.

- Routerul e hardware-software la **nivelul 3**, pentru că lucrează direct pe pachete IP.

- **Mascarada (IP masquerading) = SNAT (Source NAT).** Consecință: nodurile interne **pot accesa** rețeaua publică, dar **nu pot fi accesate** din ea. Slide-ul îl numește „Once-to-many NAT“.

- **Gateway** = calculatorul cu rol de router. Denumirea e **relativă la rețeaua locală**, iar el are un IP din **aceeași clasă** cu restul LAN-ului.

- **LAN** = rețea privată. 

- **WAN** = rețea publică.


---


### Rutare și tabela de rutare (slide 62–63)

- **Rutarea** e necesară când destinația e într-un **alt subnet** (prefix de rețea diferit).

- Fiecare nod are o **tabelă de rutare**: rute pentru destinații diferite, fiecare cu IP-ul unui gateway, plus o **rută implicită (default route / default gateway)**. Ea preia tot ce nu se potrivește cu nicio altă rută.

- **Linux:** `route` (de ex. `route -n`) sau `ip route show`.


**Exemplul din slide, `ip route show`:**

1. `default via 192.168.0.1 dev eno1 ... src 192.168.0.20 metric 100` = gateway-ul implicit.
2. `192.168.0.0/16 dev eno1 ...` = ruta pentru subnetul interfeței eno1. Traficul către acest subnet pleacă direct pe eno1.
3. `192.168.122.0/24 dev virbr0 ...` = ruta pentru subnetul interfeței virbr0.
4. `192.168.248.190 via 192.168.0.1 dev eno1` = traficul către acest IP merge **prin gateway**. **Anulează regula 1** pentru acest IP. Fără ea, IP-ul ar fi contactat direct, fiindcă e în același 192.168.0.0/16.


**`route -n` (aceleași rute scrise altfel)**

- Coloane: Destination, Gateway, Genmask, Flags, Metric, Ref, Use, Iface.
- Destinația **0.0.0.0** nu e o adresă validă, e o convenție pentru „**toate adresele**“.
- În exemplu: `UG` la default, `U` la rețele directe, `UGH` la ruta spre un host.
- **Metric** = costul rutei, deci prioritatea. **Cu cât e mai mic, cu atât ruta e preferată.** Ruta cu metric mai mare se folosește doar dacă prima nu e disponibilă (nu se poate ajunge la ea sau interfața e **DOWN**).
- Metric permite 2 rute default cu gateway-uri diferite și costuri diferite.

### DNAT, port forwarding, DMZ (slide 64)

- Implicit, nodurile din rețeaua privată **nu pot fi accesate** din cea publică. Accesarea IP-ului public ajunge la **router**.
- **DNAT (destination NAT), port forwarding:** o conexiune venită pe IP-ul public, pe un **Remote Port**, e trimisă mai departe către un **Local Address** și **Local Port** din LAN.
    - Local Port **poate fi diferit** de Remote Port.
    - Pe un Remote Port se poate pune **o singură regulă** DNAT.
    - Se pot pune **oricâte reguli** către același Local Address și/sau Local Port.
    - E **transparent** pentru host-ul public: el vede doar routerul.
- **DMZ (Demilitarized Zone):** caz **generalizat** de DNAT. Există un **DMZ host** din LAN, iar orice conexiune la router, **indiferent de port**, e trimisă la el.

⚠️ **Capcane:**

- **SNAT/masquerading:** interiorul iese afară, dar **nu poate fi accesat** din exterior.
- **Metric mai mic = prioritate mai mare.**
- **0.0.0.0 în `route -n`** = toate adresele, nu o adresă validă.
- **Ruta către un host anume** (`... via gateway`) are prioritate față de ruta implicită.
- **O singură regulă DNAT pe un Remote Port**, dar mai multe pot duce la același Local Address/Port.
- **DMZ = indiferent de port**, DNAT = pe un port anume.
- **Gateway** e o denumire relativă la LAN, cu IP din **aceeași clasă** ca LAN-ul.