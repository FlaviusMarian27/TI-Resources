### Hub, switch (slide 45–48)

**Rețeaua veche cu cablu coaxial (în serie)**

- **Single point of failure:** o problemă pe cablu sau la o placă de rețea strică comunicația pe toată rețeaua.
- practic pc-urile erau legate in serie.
- **Doar broadcast, necontrolat:** chiar dacă vorbesc doar 2 calculatoare, ceilalți interceptează.
- **Securitate:** un „spion” se injectează ușor pe cablu și vede tot. **Spoofing** ușor (impersonarea nodurilor).
- **Instabilă:** afectată de perturbații electromagnetice.



### **Rețea cu hub**

- Coaxialul se înlocuiește cu **cablu cu 4 perechi torsadate + conectori RJ-45** și un **hub**. Rezultă o rețea **stea**.
- **Rezolvă:** single point of failure (afectat e doar calculatorul cu probleme) și perturbațiile electromagnetice.
- **Hub = echipament activ la L1 OSI**, doar nivel electric, fără „inteligență”.
- **NU rezolvă:** domeniul de broadcast (pachetul ajunge la toate calculatoarele, fie unicast, multicast sau broadcast) și spionarea (un cablu adițional în hub vede toată rețeaua, cu posibilitate de spoofing).
- Practic daca te conectai la oricare port puteai sa asculti ce vorbesc celalalte porturi.




### **Rețea cu switch**

- **Switch = echipament activ la L2 OSI.**

- Identifică **MAC-urile** de pe porturi și ține o **tabelă MAC** (temporară).

- Citește frame-ul Ethernet (MAC sursă și destinație) și trimite pachetul **doar pe portul** unde e MAC-ul destinație.

- Reduce broadcast-ul, îmbunătățește rata de transfer, **rezolvă spionarea unicast** și **parțial spoofing-ul**.

- Performanța depinde de **câte pachete Ethernet procesează pe unitatea de timp**, nu doar de viteza porturilor.

- Exista sw-uri care isi trimit intre ele tabelele de routare prin anumite protocoale.

- Conteaza marimea tabelei si puterea cu care poate trimite sw-ul (cate pachete/sec).

- Avem:
	- **Unmanaged:** doar funcția de bază, nu se configurează și nu se monitorizează, cost mic, rețele simple.
	- **Managed:** configurabil și monitorizabil prin **serial UART, web, telnet, ssh**. Oferă facilități de securitate și stabilitate. Cost mai mare (enterprise mult mai mare).

- te poti conecta remote (telnet, SSH, Web).

- cand se aprinde becul pe portul respectiv, inseamna ca a reusit sa comunice cu celalalt device si are adresa MAC.


⚠️ **Capcane:**

- **Hub = L1, switch = L2.**
- Hub-ul rezolvă single point of failure, dar **nu** broadcast-ul și **nu** spionarea.
- Switch-ul rezolvă spionarea, dar spoofing-ul doar **parțial**.
- **Managed** înseamnă configurare (serial, web, telnet, ssh). **Unmanaged** nu se configurează.


---

# ARP (slide 50–54)

**Idee**

- **ARP** descoperă **adrese MAC** pe baza IP-ului. C1 vrea să trimită un pachet IP către C2, dar pachetele IP merg în frame-uri Ethernet adresate cu MAC, deci C1 trebuie să știe MAC-ul lui C2.

- Paradigma **request-response**. Slide-ul îl plasează la **nivelul 3** și zice că e transportat de pachete **Ethernet de broadcast**. Pachetul are structură fixă.



|                              | ARP Request                                 | ARP Response                                                      |
| ---------------------------- | ------------------------------------------- | ----------------------------------------------------------------- |
| **OpCode**                   | 1                                           | 2                                                                 |
| **Ethernet**                 | **broadcast**, MAC dest `FF:FF:FF:FF:FF:FF` | **unicast** către cel care a întrebat                             |
| **Source MAC / IP**          | cel care trimite cererea                    | cel care răspunde (IP = Destination Protocol Address din request) |
| **Destination MAC** (în ARP) | `00:00:00:00:00:00` (necunoscută)           | MAC-ul celui care a trimis request-ul                             |
| **Destination IP**           | IP-ul căruia i se cere MAC-ul               | IP-ul celui care a trimis request-ul                              |


**Variante și mecanisme**

- **ARP Probe:** request broadcast **înainte** de setarea IP-ului, ca să verifice dacă IP-ul e deja folosit în rețea. Dacă apare răspuns, IP-ul e deja ocupat.

- **GARP (Gratuitous ARP / ARP Announcement):** dispozitivul își anunță **neîntrebat** MAC-ul prin broadcast ARP Request. Se recunoaște prin **IP sursă = IP destinație**.

- **ARP Proxy:** routerul răspunde la ARP Request pentru IP-uri din **altă rețea**, cu **propriul MAC**, și ține o tabelă internă pentru IP-urile din afară. Practic router-ul raspunde cu MAC-ul lui pentru toate pc-urile sale.

- **ARP Caching:** dispozitivul ține o tabelă MAC-IP, ca să reducă cererile. O intrare se invalidează prin **timeout** sau când se constată că **nu mai merg conexiunile** cu perechea IP-MAC. Tabela se actualizează și la **GARP**.

- **ARP Spoofing:** un alt dispozitiv răspunde **mai repede** decât cel real, ca să îl înlocuiască în rețea.



**Comenzi Linux**

- `arp -an` : afișează tabela. `<incomplete>` în loc de MAC = s-a cerut comunicația cu acel IP, dar nu s-a descoperit MAC-ul.
- `arp -i eno1 -s 192.168.4.123 1C:00:34:22:11:AA` : adaugă manual o pereche IP-MAC.
- `arp -d 192.168.4.123` : șterge o înregistrare.
- `arping` : trimite forțat cerere ARP către un IP, așteaptă răspunsul și măsoară timpul. E ca `ping`, dar **nu folosește ICMP**.



⚠️ **Capcane:**

- **Request:** OpCode 1, Ethernet broadcast, dar Destination MAC din pachetul ARP = `00:..:00`.
- **Response:** OpCode 2, **unicast**.
- **GARP** se recunoaște după IP sursă = IP destinație, nu după OpCode.
- **Probe** = înainte de a seta IP-ul. **GARP** = anunț după.
- **Proxy ARP** răspunde cu **MAC-ul routerului**.
- **`arping` ≠ ICMP.**
- Intrarea ARP se invalidează prin **timeout** sau când conexiunea nu mai merge.