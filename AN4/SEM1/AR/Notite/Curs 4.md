# DHCP, Dynamic Host Configuration Protocol

- Protocol la **nivelul 7 (aplicație)**.

- are rol de **asignare automată de adrese IP** către clienții din rețea.

- **2 componente:**
    - **Server DHCP**, pe un calculator cu rol de server/router, configurat cu **IP fix, manual**.
    - **Client DHCP**, pe celelalte calculatoare din aceeași rețea, **fără IP configurat**.

- Serverul ține evidența adreselor alocate (**IP + MAC**), ca să nu dea duplicate. Gestionarea se face **pe bază de MAC**.

- **LEASE TIME:** adresele sunt „închiriate” pe o perioadă. După expirare, clientul cere din nou, de obicei primește aceeași adresă dacă e liberă.

- **Transport:** 
	- Serverul ascultă pe **port 67**
	- clientul pe **port 68**. 

- **Comunicația o inițiază clientul.**

- Foloseste **UDP**

- Pe lângă IP trimite și: 
	- **netmask (obligatorie)**
	- gateway
	- servere DNS
	- servicii NTP
	- BOOTP
	- fișiere de boot

- **Adrese dinamice:** dintr-un interval configurabil, se pot schimba la expirarea lease-ului sau când decide serverul, deci clientul poate primi altă adresă.

- **Rezervare:** pe baza **MAC-ului**, clientul primește mereu aceeași adresă. Se recomandă ca adresele rezervate să **nu facă parte din intervalul dinamic**.


---


# **Mesaje**

|Mesaj|Cine|Detalii din slide|
|---|---|---|
|**DHCPDISCOVER**|client|începe procesul. **Broadcast L3 (255.255.255.255) și L2 (FF:..:FF)**, UDP **port 67**. Broadcast, pentru că **nu are IP și nu știe adresa serverului**|
|**DHCPOFFER**|server|ofertă cu un IP posibil. UDP **port 68**. L3: **broadcast** (clientul nu are IP). L2: **broadcast sau unicast** pe MAC-ul clientului, deja cunoscut|
|**DHCPREQUEST**|client|acceptă oferta. De obicei **broadcast**, ca să informeze pe ceilalți clienți și pe eventualele alte servere DHCP. Poate conține IP-ul din OFFER, **alt IP** sau **IP-ul vechi**|
|**DHCPACK**|server|confirmă REQUEST-ul. **Broadcast sau unicast**|
|**DHCPNACK**|server|**respinge** REQUEST-ul (IP invalid sau indisponibil). De obicei **broadcast**|
|**DHCPRELEASE**|client|anunță **eliberarea** adresei. **Unicast** către IP-ul serverului|
|**DHCPINFORM**|client|clientul **are deja IP** și cere setări adiționale. **Broadcast** dacă nu știe IP-ul serverului, **unicast** dacă îl știe. Serverul răspunde cu **DHCPACK** cu informațiile cerute|
|**DHCPDECLINE**|client|**refuză oferta** din OFFER, când detectează că IP-ul e **deja utilizat** în rețea|

⚠️ **Capcane:**

- **Server 67, client 68.** DISCOVER merge **la** 67, OFFER merge **la** 68.
- **DISCOVER e broadcast** pentru că clientul nu are IP și nu cunoaște serverul.
- **OFFER:** L3 broadcast, dar **L2 poate fi broadcast sau unicast**.
- **NACK = server respinge. DECLINE = client refuză.**
- **RELEASE = unicast** către server. **REQUEST = de obicei broadcast.**
- **INFORM:** clientul are deja IP, iar serverul răspunde cu **ACK**, nu cu OFFER.
- **Netmask e obligatorie**, restul (gateway, DNS) sunt opționale.
- **DHCP e L7**, nu L3 și nu L2.
- Rezervarea e pe **MAC**, nu pe IP.