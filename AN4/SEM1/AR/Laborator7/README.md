# Lucrarea 6: Configurarea inițială a unui switch Cisco (GNS3)

Administrarea rețelelor, laborator 6.

## Obiective

- Switch cu management vs switch fără management
- Switch-uri compatibile Cisco
- Comenzi de bază Cisco
- Asignarea unui IP de management pe VLAN-ul standard Cisco
- Conectarea prin telnet la un switch cu management

## Teorie

**Switch fără management**
- Switch-uri "uzuale", folosite în general în rețele rezidențiale.
- Fac doar comutarea de pachete în funcție de adresa MAC.
- Performanțe mai reduse, tabele MAC relativ mici.
- Nu oferă monitorizare sau configurare.
- Sunt mai ieftine și mai populare datorită simplității.

**Switch cu management**
- Permit configurarea și monitorizarea.
- Pe lângă comutarea după MAC, oferă și alte facilități, de exemplu VLAN conform **IEEE 802.1q**.
- Interfața de configurare: linie de comandă (CLI), web, sau ambele, în funcție de producător.
- Se configurează inițial **fizic** (Ethernet sau port serial), apoi primește o adresă IP și se integrează în rețea.
- Apoi poate fi accesat **remote**, prin IP. Remote e preferat pentru că switch-ul poate fi într-o locație greu accesibilă.

**În GNS3** se folosește un switch Cisco generic, IOS 15.2, modulul `CiscoIOSvL215.2(20200924:215240)-1`. Poate diferi de un switch real, dar majoritatea funcționalităților sunt similare.

## Topologie

- Switch Cisco (CiscoIOSvL2)
- Nod Cloud (conexiune la Internet)
- Nod Debian 12.6 cu 2 plăci de rețea

| Interfața Debian | Se conectează la | Rol |
|---|---|---|
| `ens4` (prima) | Cloud | DHCP, Internet |
| `ens5` (a doua) | `Gi0/0` de la switch | IP static 172.30.0.1/24 |

Adrese: Debian **172.30.0.1**, switch **172.30.0.2**, ambele `/24` (255.255.255.0).

---

## Pașii

### Pasul 1: Crearea rețelei
Se creează o rețea formată dintr-un switch Cisco `CiscoIOSvL215.2(20200924:215240)-1`, un nod Cloud și un nod Debian 12.6. Încă nu se fac conexiunile.

### Pasul 2: Configurarea nodului Debian (înainte de pornire)
- Click dreapta pe Debian → **Configure** → tabul **Network**.
- La **Adapters** apare valoarea 1. Se schimbă în **2**.
- Click pe **Configure custom adapters** pentru a vedea numele celor două interfețe (ex: `ens4` și `ens5`). Numele se rețin, vor fi necesare mai târziu.
- Nu se modifică nimic altceva. Se apasă **Apply**/**OK** în ambele ferestre.

Scop: nodul Debian se conectează cu o interfață la Cloud (Internet) și cu alta la switch-ul Cisco.

### Pasul 3: Conexiunile
- Prima interfață (ex: `ens4`) → nodul **Cloud**
- Cealaltă interfață (ex: `ens5`) → **Gi0/0** de la switch

### Pasul 4: Pornirea
Se pornesc toate modulele din simulare (butonul play verde).

### Pasul 5: Internet pe Debian
- Se deschide consola nodului Debian. Login: user `debian`, parola `debian`.
- Interfața conectată la Cloud (`ens4`) se configurează cu IP automat prin **DHCP**, conform laboratorului 4.
- Se aplică setările și se verifică Internetul:
```
ping 8.8.8.8
ping google.com
```
Primul testează conectivitatea la Internet, al doilea testează și DNS-ul.

### Pasul 6: Instalarea pachetelor
```
sudo apt update
sudo apt install net-tools dnsutils telnet
```
- `apt update` actualizează lista de pachete.
- `net-tools`, `dnsutils` și `telnet` sunt pachetele necesare (telnet e clientul folosit la pasul 13).

### Pasul 7: IP static pe cealaltă interfață
Interfața conectată la switch (`ens5`) primește adresa statică **172.30.0.1 / 255.255.255.0**, conform laboratorului 4. Se aplică setările și se **restartează nodul Debian**.

Exemplu în `/etc/network/interfaces`:
```
auto ens5
iface ens5 inet static
    address 172.30.0.1
    netmask 255.255.255.0
```
Verificare cu `ip a`: `ens4` are IP de la DHCP, `ens5` are 172.30.0.1/24.

### Pasul 8: Consola switch-ului și comanda `?`
- Click dreapta pe switch → **Console**. La apăsarea tastei **Enter** apare promptul `Switch>`.
- Comanda `?` cere switch-ului lista de comenzi suportate.
- La mesajul `--More--`: **Enter** = o linie nouă, **space** = mai multe linii.
- Comenzile Cisco sunt organizate **arborescent**, ca directoarele unui sistem de operare. Pentru a executa o comandă trebuie să ajungem la ea prin structura arborescentă.
- `?` arată mereu comenzile disponibile din nodurile **direct următoare** față de nodul curent.
- Promptul switch-ului se schimbă ca să arate nodul curent din arborele de comenzi.

### Pasul 9: Modurile de lucru (`enable` și `configure terminal`)
- `configure terminal` din `Switch>` dă **eroare**: subarborele de configurare e privilegiat și trebuie întâi accesat modul privilegiat.
- `enable` → promptul devine `Switch#` (modul privilegiat).
- `configure terminal` → promptul devine `Switch(config)#` (rămânem în modul privilegiat, dar am trecut în alt subarbore).
- `?` confirmă că suntem într-un alt subarbore (altă listă de comenzi).
- `exit` → revenirea în subarborele anterior.

Rezumat prompturi:

| Prompt | Mod |
|---|---|
| `Switch>` | utilizator |
| `Switch#` | privilegiat |
| `Switch(config)#` | configurare |

### Pasul 10: IP de management pe vlan 1
Configurare inițială: se pregătește accesul din rețea, nu doar din consolă. Secvența (începând din `Switch#`):
```
configure terminal        # se intră în modul de configurare
interface vlan 1          # se selectează interfața vlan1, alt subarbore
ip address 172.30.0.2 255.255.255.0   # adresa IP statică, setată manual
no shutdown               # pornește interfața (inițial era în shutdown)
exit                      # se iese din subarborele interfeței vlan1
exit                      # se iese din modul de configurare
```

### Pasul 11: Verificarea conexiunii
Din nodul Debian:
```
ping 172.30.0.2
```
Dacă răspunde, legătura Debian ↔ switch funcționează.

### Pasul 12: User, parolă și telnet
Din consola switch-ului:
```
configure terminal                          # intră în modul de configurare
username admin privilege 15 password admin  # user "admin", parola "admin", privilegii maxime (15)
line vty 0 4                                # selectează liniile logice de acces, intră în modul lor de configurare
login local                                 # login din baza de date locală de utilizatori (cea cu userul admin)
transport input telnet                      # liniile acceptă autentificări pe protocolul telnet
exit                                        # iese din modul de configurare al liniei vty 0 4
enable secret admin                         # parola cerută la comanda enable (tot admin)
exit                                        # iese din modul de configurare general
write                                       # salvează setările în memoria switch-ului
```

### Pasul 13: Conectarea prin telnet
- Se deschide (redeschide) consola nodului Debian. Login dacă nu e deja făcut.
- Conectarea la switch prin telnet, nu prin consolă:
```
telnet 172.30.0.2
```
- După conectare, la apăsarea tastei Enter switch-ul cere user și parolă: `admin` / `admin`.
- Apoi `enable`. Parola cerută este cea setată la `enable secret` (pasul 12), adică tot `admin`.
- Prin această interfață telnet se pot executa comenzile switch-ului.

### Pasul 14: Comanda `show`
Se execută din rădăcina CLI-ului (`Switch#`). Acceptă un număr foarte mare de parametri și subparametri, tot sub formă arborescentă. Lista parametrilor se obține cu `show ?`.
```
show running-config   # configurația switch-ului din memorie (IP, user, telnet, enable secret)
show version          # versiunea de software și hardware a switch-ului
show clock            # data și ora curentă a switch-ului
```

### Pasul 15: Comanda `?`
- Nu este o comandă propriu-zisă.
- După fiecare comandă trimisă cu `?`, switch-ul afișează parametrii ce pot urma sau explicații suplimentare, dar **numai pentru nivelul următor**.
- Se poate folosi `?` pentru orice nivel, după completarea unui parametru din nivelul inferior.
- Se folosește `show` împreună cu `?` ca să aflăm parametri și subparametri:
```
show ?
show ip ?
show ip interface ?
```

---

## Capcane de examen

- `configure terminal` fără `enable` dă **eroare** (subarbore privilegiat).
- Prompturi: `Switch>` (utilizator), `Switch#` (privilegiat), `Switch(config)#` (configurare).
- Interfața vlan1 e inițial în **shutdown**, deci e nevoie de `no shutdown`.
- Parola de la `enable` este cea din `enable secret`, nu cea de la user.
- `privilege 15` = nivelul cel mai ridicat de privilegii.
- `line vty 0 4` = liniile logice de acces remote.
- `login local` = login din baza de date locală de utilizatori.
- `transport input telnet` = liniile acceptă telnet.
- `write` salvează configurația în memoria switch-ului.
- Telnet-ul se face **din Debian**, nu din consola switch-ului.
- `?` nu e comandă, arată doar parametrii pentru nivelul următor.
- `--More--`: Enter = o linie, space = mai multe linii.
- Adrese: Debian 172.30.0.1, switch 172.30.0.2, mască 255.255.255.0.