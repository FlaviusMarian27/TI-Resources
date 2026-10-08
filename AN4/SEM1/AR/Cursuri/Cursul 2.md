# Adresare. Adrese MAC. Adrese IP


# Adresa MAC

- ajută la identificarea device-urilor din aceeași rețea.
- este specifică pentru layer 2 din stiva OSI.
- este o adresă scrisă în hardware și se poate modifica.
- sunt gestionate de IEEE.
- sunt reprezentate prin intermediul standardului EUI-48 și mai puțin EUI-64.
- dacă prin absurd am duplicat de MAC, dar se află fiecare în altă rețea nu afecteză.


![Adresa MAC Format](Images/MACFormat.png)


- ***1: Multicast*** - rezervat pentru anumite protocoale.
- ***1: Locally administered*** - îi o mașină virtuală/docker/etc, nu îi ceva fizic!!!!
- FF:FF:FF:FF:FF:FF - este adresă de broadcast și este folosit de DHCP și ARP.

---

# Adresa IP

- este specifică layer-ului 3 din stiva OSI.
- pentru a identifica dispozitivele din altă rețea.
- avem IPv4 și IPv6.
- IPv6 este foarte nesigur și are multe bug-uri.
- IPv4 este reprezentat pe 32 bits sau 4 bytes.
- IPv6 este reprezentat pe 128 bits sau 32 bytes.
- sunt gestionate de IANA:
	- AFRINIC - Africa
	- APNIC - Asia
	- ARIN - America de Nord
	- LACNIC - America Latină și Caraibe
	- RIPE NCC - Europa, Orientul Mijlociu și Asia Centrală

![Clasele pentru adresele IP](Images/ClaseIP.png)

- Net ID -> pentru rețea.
- Host ID -> îți indentifică dispozitivul.