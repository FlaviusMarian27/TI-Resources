
# Definiție

- **Server web** = este o aplicație de tip server care servește conținut HTML unui client numit **browser**.
- Browser-ul descarcă fișierele HTML și le interpretează.
- Comunică prin **http** și **https**, ambele protocoale fiind de layer 7.
- Folosesc TCP
- Poate asculta pe orice port TCP, dar by default pe port-ul 80 pentru http și 443 pentru https.
- Primul server web (httpd) a fost creat în 1990 de Tim Berners-Lee la CERN.

---

## 2. URI / URL / URN (slide 83–84)

- **URI** = string cu sintaxă bine definită care identifică o resursă în rețea. Poate specifica locația, numele sau ambele.
- Sintaxa generală (RFC 3986): `scheme:[//authority]path[?query][#fragment]`
    - **scheme**: protocolul (http, https, ftp, mailto, ldap, ldaps).
    - **authority**: `[user:password@]host[:port]`.
    - **path**: calea resursei.
    - **query**: parametri opționali trimiși serverului.
    - **fragment**: o subsecțiune din rezultat.
- **URL** = caz particular de URI: identifică resursa, locația și protocolul/metoda de acces.
- **URN** = caz particular de URI: identifică resursa **după nume**, fără locație, protocol sau metodă de acces.
- Orice URL e URI, dar **nu orice URI e URL**, pentru că URN nu e URL.
- Exemple URL din slide: `https://myuser:mypassword@example.org/myindex.html`, `ftp://...`, `mailto:...`, `file:///home/valy/ptr.c`, `ssh://git@gitlab.upt.ro:8822/...`. Exemple URN: `urn:isbn:...`, `urn:uuid:...`.

## 3. Protocolul HTTP (slide 85–90)

- Nivel aplicație (OSI 7), între browser și server web. Transport **ASCII text**. Standardizat de IANA prin RFC-uri.
- Schema `http` implică comunicare pe **portul 80 TCP**.
- Paradigma **client-server, request-response**. Dialogul e inițiat de client, **după** ce clientul a stabilit conexiunea TCP.

**Request:**

```
<method> <request-target> <HTTP-version>
<headers>

<body>
```

- Linia goală **face parte din protocol și e necesară**.
- **request-target** = calea resursei (URL-ul).
- **HTTP-version** = HTTP/1.1, HTTP/2, HTTP/3.
- **headers** = metadata, perechi `Header-name: Header-value` (ex. `Host: staff.cs.upt.ro`, `User-Agent: Mozilla/5.0`, `Accept: text/html`).
- **body** = payload.

**Metode:**

|Metodă|Rol|
|---|---|
|**GET**|cere o resursă (fișier html identificat prin URL). Poate avea directive în headers, dar **nu are body**|
|**HEAD**|ca GET, dar răspunsul **nu conține datele**, doar header-ele|
|**POST**|trimite date de la browser la server, ca să încarce sau să înlocuiască o resursă|
|**DELETE**|șterge o resursă|
|**PATCH**|actualizează **parțial** o resursă|
|**OPTIONS**|cere lista capabilităților serverului și a metodelor http suportate|
|**TRACE**|depanare, de tip **echo**: datele trimise de client sunt returnate de server|
|**CONNECT**|creează un **tunel** prin serverul web, de obicei pentru transfer prin proxy|

- Exemplu GET din slide: `GET /~valy/ HTTP/1.1`, `Host: staff.cs.upt.ro`, `Connection: close`.

**Response:**

```
<HTTP-version> <status-code> <reason-phrase>
<headers>

<body>
```

- **status-code** = cod numeric, tot ASCII.
- **reason-phrase** = text explicativ scurt al codului.

**Coduri de stare** (slide 89–90):

|Clasă|Coduri|
|---|---|
|**1XX informativ**|**100 Continue**: clientul a trimis o cerere fără body, iar serverul așteaptă și body-ul. **101 Switching protocols**: clientul a cerut schimbarea protocolului, iar serverul a acceptat|
|**2XX succes**|**200 OK**: răspuns standard, poate avea body. **202 Accepted**: datele cerute sunt în procesare. **204 No Content**: cererea e procesată corect, dar răspunsul nu are conținut|
|**3XX redirectare**|**301 Moved Permanently**: resursa e mutată permanent la alt URI. **307 Temporary Redirect**: mutată temporar, în viitor se folosește același URI inițial. **308 Permanent Redirect**: ca 301, dar mai restrictiv|
|**4XX eroare client**|**400 Bad Request**: sintaxă greșită, dimensiune prea mare sau incorectă, request invalid. **401 Unauthorized**: clientul nu s-a autentificat corect. **403 Forbidden**: acces restricționat, fie clientul nu a inițiat autentificarea, fie e autentificat dar nu are drepturi. **404 Not Found**: resursa nu e la URL-ul cerut. **406 Not Acceptable**: resursa există, dar nu poate fi accesată în formatul cerut prin header. **407 Proxy Authentication Required**: autentificare validă cerută de un proxy. **408 Request Timeout**: serverul nu a răspuns în timp util și clientul nu mai așteaptă|
|**5XX eroare server**|**500 Internal Server Error**: eroare generică, situație de excepție neprelucrabilă. **501 Not Implemented**: serverul nu recunoaște metoda din request sau nu poate răspunde în acel moment. **502 Bad Gateway**: serverul contactat e configurat ca proxy, iar backend-ul nu răspunde sau nu poate fi contactat. **Service Unavailable**: server prea încărcat sau în mentenanță, de obicei temporar|

## 4. Protocolul HTTPS (slide 91–93)

- Extensie a http în care mesajele sunt **criptate cu TLS** (Transport Layer Security). Scop: limitarea interceptării prin atacuri **man-in-the-middle**.
- Schema `https`, **portul 443 TCP**.
- Înainte de mesajele http se face o conexiune TLS care stabilește cheile și algoritmul de criptare și **confirmă identitatea serverului prin certificat**.
- **TLS e protocol binar** (nu ASCII ca http).

**Pașii TLS handshake:**

1. Conexiune **TCP** între browser și server.
2. Clientul trimite **ClientHello**: versiunea TLS suportată (1.2, 1.3), algoritmii de criptare suportați și **ClientRandom** (secvență aleatorie generată de browser, folosită la cheile de sesiune).
3. Serverul răspunde cu **ServerHello**: versiunea TLS aleasă (poate refuza sesiunea dacă nu suportă nicio versiune), **ServerRandom** și **certificatul digital** (cheia publică a serverului plus identitatea, semnate de un **CA**).
4. Clientul verifică certificatul:
    - e semnat de un CA valid (root sau intermediar). Browserele au lista CA-urilor **root**, pentru validare locală rapidă. Pentru CA-urile **intermediare** se contactează CA-ul respectiv;
    - numele din certificat coincide cu numele accesat;
    - perioada de valabilitate;
    - **opțional**: verificarea revocării, de exemplu prin **OCSP** (Online Certificate Status Protocol).
5. **Schimbul de chei**: se negociază o cheie secretă între browser și server. Se generează cu algoritmi care folosesc **ClientRandom și ServerRandom**, rezultând **session-key**.
6. Browserul trimite **Finished**, criptat cu session-key.
7. Serverul decriptează și răspunde cu **Finished** criptat.
8. Conexiune TLS stabilită. De acum mesajele http se transportă **criptate, în format binar**.

**Fișierele de care are nevoie serverul (slide 93):**

1. **private.key**: cheia privată. Decriptează secretul trimis de client (clientul criptează cu cheia publică a serverului). Servește și la verificarea internă a certificatului.
2. **cert.pem**: certificatul serverului, semnat de CA. Demonstrează identitatea față de client.
3. **chain.pem**: certificatul **CA-ului intermediar** care a emis certificatul serverului. Trebuie să fie semnat de un root CA.
4. **fullchain.pem**: certificatul CA-ului intermediar plus certificatul serverului. **Nu conține root CA**, pentru că acesta e deja în trust store-ul browserului.

## 5. Apache web server (slide 94–101)

**Virtual host** (slide 94–95):

- Un server web poate servi **mai multe site-uri pe același IP**, după valoarea din câmpul host (authority). Fiecare site are un **VirtualHost**. Altfel ar fi nevoie de un server web și un IP pentru fiecare site.
- `<VirtualHost *:80>` = valabil pe **toate IP-urile** sistemului (`*`), pe portul 80.
- **ServerName**: serverul îl compară cu header-ul `Host:` din request. Dacă coincid, servește site-ul respectiv.
- **ServerAdmin**: adresa de mail a administratorului.
- **DocumentRoot**: calea de pe disk a fișierelor site-ului. Se caută întâi `index.html` sau `index.htm` (prima pagină).
- **ErrorLog** și **CustomLog**: fișierele de log (în exemplu `..._error.log` și `..._access.log combined`).
- **`<Directory>`**: proprietăți pentru **directoare din sistemul de fișiere** (nu sunt secțiuni ale aplicației web).
- **`<Location>`**: proprietăți pentru **secțiuni/locații ale aplicației web**. În exemplu, `<Location /uploads/>` are propriul CustomLog.

**HTTPS în Apache** (slide 96):

- `<VirtualHost *:443>` cu `SSLEngine on`, `SSLCertificateFile` (cert.pem), `SSLCertificateKeyFile` (privkey.pem) și `SSLCertificateChainFile` (chain.pem).

**Restricționarea accesului** (slide 97):

- În `<Location /user>`: de obicei se începe cu `Deny from all`, apoi se adaugă `Allow from <ip sau clasă de ip>` (ex. `193.226.12.0/24`, `172.16.0.0/16`). Practic e un fel de firewall.
- Alternativ, tag-uri `<Require ...>`: `require any`, `require valid user`, `require ip`, `require auth_imap`, `require ldap-group`.

**Redirectare** (slide 98):

- `Redirect /site/ https://cs.upt.ro`: orice conexiune la `http://myserver.cs.upt.ro/site` merge la `https://cs.upt.ro`. Primul argument e locația accesată, al doilea destinația.
- E **vizibilă în browser: se schimbă URL-ul**.
- `Redirect / https://myserver.cs.upt.ro` redirectează automat tot traficul **http → https**, deci clienții folosesc conexiune securizată.

**Reverse proxy** (slide 99–100):

- Serverul web **nu servește el pagina**, ci trimite cererea către alt server web (**backend server**). Spre deosebire de redirectare, **nu e vizibil clientului**. Serverul proxy rescrie URL-ul.
- De obicei proxy-ul e în rețeaua **publică**, iar backend-ul în rețeaua **privată**, „în spatele” lui.
- Se implementează cu **ProxyPass** și **ProxyPassReverse**, sintaxă `path url`.
- **ProxyPass `/new/` `http://192.168.7.31/`**: cererile venite pe `/new/` sunt trimise către backend. Transparent pentru browser. Backend-ul **nu trebuie să fie vizibil în Internet**.
    - `http://myserver.cs.upt.ro/new/index.html` → `http://192.168.7.31/index.html`
    - `http://myserver.cs.upt.ro/new/general/api` → `http://192.168.7.31/general/api`
- **ProxyPassReverse**: efect invers. Rescrie calea din **răspunsul** backend-ului, ca să conțină numele din ServerName. Exemplu: `http://192.168.7.31/mysite.html` → `http://myserver.cs.upt.ro/new/mysite.html`.
- Fără ProxyPassReverse, clientul primește direct URL-ul backend-ului, ceea ce nu are sens, pentru că răspunsul pare să vină de la alt host.
- Doar cu ProxyPass și ProxyPassReverse ai proxy cu **URL-uri statice**. Pentru **URL-uri dinamice** e nevoie de modulul **Rewrite**.

**Load balancing** (slide 101):

- Se folosește **împreună cu directivele de proxy** și extinde proxy-ul la mai multe backend-uri.
- `<Proxy "balancer://myapp">` cu mai multe **BalancerMember** (oricâte) și `ProxySet lbmethod=byrequests`. Slide-ul spune că e o regulă de tip **round-robin**. „myapp” e doar numele balancer-ului.
- Apoi se configurează obligatoriu `ProxyPass /new/ "balancer://myapp/"` și `ProxyPassReverse`, ca la un singur backend.
- Se poate folosi un balancer **și cu un singur backend**.

## 6. Certificate Let's Encrypt cu certbot (slide 102–105)

**Generalități:**

- Certificatele https se obțin de la un CA pentru: un domeniu (`example.com`), un subdomeniu, un **wildcard de rang 1** (`*.example.com`) sau un wildcard de rang 2 pentru un subdomeniu (`*.subdomeniu.example.com`).
- Sunt emise pe **perioadă limitată** și se reînnoiesc. De obicei sunt **contra cost**.
- **Self-signed** (nesemnat de CA): asigură **doar criptarea**, **nu verificarea identității** serverului. Generează eroare/warning în browser. Browserul poate permite continuarea sau poate refuza accesul.
- CA-uri gratuite, pe perioade scurte: **Let's Encrypt, ZeroSSL**.
- **Let's Encrypt**: certificate pe **90 de zile**, gratuit. **Fără interfață de management și fără avertizare de expirare**, deci administratorul gestionează certificatul. Accesibil prin **RestAPI** sau clienți dedicați precum **certbot**.
- Pentru emitere, LetsEncrypt verifică prin **challenge-uri** că cel care cere administrează domeniul. Procesul e automatizat cu certbot.

**HTTP-01 challenge** (slide 103):

- Serverul trebuie să servească un fișier la `http://<domeniu>/.well-known/acme-challenge/<token>`. Token = hash generat de LetsEncrypt. În fișier: token plus **thumbprint** (hash) generat din cheia privată a contului/cererii.
- Comanda: `certbot certonly --standalone --preferred-challenges http -d host.example.com`
- Certbot pornește un **server web temporar pe portul 80**, generează cheia privată, creează fișierul cu token și trimite cererea. LetsEncrypt accesează `host.example.com`. Dacă prin DNS ajunge la serverul pornit de certbot, confirmă controlul și emite certificatul. Altfel procesul se anulează.

**DNS challenge** (slide 104–105):

- Verificarea se face printr-un **record TXT** la un subdomeniu dedicat, cu un hash aleator generat de certbot: `_acme-challenge.subdomeniu.example.com IN TXT "hash-challenge..."`.
- Serverele LetsEncrypt verifică challenge-ul pe **servere DNS globale recursive**: **Google (8.8.8.8, 8.8.4.4) și Cloudflare (1.1.1.1, 1.0.0.1)**, **nu** interoghează direct serverul authoritative.
- Administratorul trebuie să **aștepte propagarea** modificării. Se recomandă **TTL mic** pentru `_acme-challenge`.
- Comanda wildcard: `certbot certonly --manual --preferred-challenges=dns --email=email@example.com --agree-tos -d *.example.com`
- Pașii:
    1. Se rulează comanda.
    2. Certbot contactează LetsEncrypt, emite o cheie privată și afișează hash-ul challenge-ului. Apoi **se suspendă** și așteaptă administratorul.
    3. Administratorul adaugă recordul TXT în DNS-ul authoritative.
    4. Verifică propagarea în Google și Cloudflare, cu `dig`, toolbox.googleapps.com sau dnschecker.org.
    5. Alege continuarea, după ce e sigur că s-a propagat.
    6. Certbot validează domeniul și emite certificatul.

## ⚠️ Capcane de examen

- **URL ⊂ URI.** URN e URI, dar nu e URL.
- **GET nu are body.** HEAD întoarce doar header-ele. TRACE = echo. CONNECT = tunel prin proxy. PATCH = actualizare **parțială**.
- **http = ASCII text, TLS = binar.**
- **401 ≠ 403:** la 401 nu te-ai autentificat corect. La 403 accesul e interzis chiar dacă ești autentificat și identificat.
- **301 = permanent, 307 = temporar** (se păstrează URI-ul inițial), **308 = ca 301 dar mai restrictiv**.
- **502** apare când serverul e proxy și backend-ul nu răspunde. Slide-ul 90 scrie greșit „502 – Service Unavailable” pentru ultimul cod. La examen reții descrierea de pe slide (server încărcat sau în mentenanță, de obicei temporar).
- **4XX = vina clientului, 5XX = vina serverului.**
- **Redirect vizibil** (URL-ul se schimbă în browser), **reverse proxy invizibil** (transparent pentru client).
- **ProxyPass** trimite cererea la backend, **ProxyPassReverse** rescrie răspunsul. Fără el, clientul vede URL-ul backend-ului.
- Pe slide-uri apare scris **„ProxyPas”**, o greșeală de tipar. Directiva corectă e `ProxyPass`.
- **`<Directory>` = sistemul de fișiere, `<Location>` = locație în aplicația web.**
- `lbmethod=byrequests` = **round-robin**. Balancer-ul merge și cu un singur backend.
- **fullchain.pem** = intermediar plus server, **fără root CA**. Root CA e deja în trust store-ul browserului.
- **Self-signed** = doar criptare, fără verificarea identității.
- **Let's Encrypt = 90 de zile**, fără interfață de management și fără avertizare de expirare.
- **HTTP-01** servește fișierul la `/.well-known/acme-challenge/<token>` și certbot pornește un server pe **portul 80**. **DNS challenge** = record **TXT** la `_acme-challenge`, verificat pe **Google și Cloudflare**. **Wildcard** se face prin DNS challenge.
- **ClientRandom + ServerRandom** → **session-key**. Clientul criptează cu cheia **publică** a serverului, serverul decriptează cu cheia **privată**.

