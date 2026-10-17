# Laborator 4 - Administrarea rețelelor (Linux Debian 12.6 în GNS3)

Ce am făcut acasă, pas cu pas, ca recapitulare pentru laborator.

## Topologie

Cloud1 (enp4s0) -> Switch1 (e0); Debian12.6-1 (ens4) pe e1; PC1 (VPCS) pe e2; webterm-1 (Firefox, eth0) pe e3.
Toate nodurile se leagă la switch. Direcția cablului nu contează.

Valori de acasă (rețea 192.168.1.0/24, gateway 192.168.1.1):
- PC1 (VPCS): 192.168.1.4 (DHCP)
- Debian: 192.168.1.5 (DHCP), ulterior 192.168.1.50 (static)
- MAC Debian: 0c:a7:94:97:00:00

## Pașii 1-3: pornire și DHCP pe VPCS

1. Construiești topologia de mai sus.
2. Start all, aștepți să fie toate verzi.
3. În consola PC1:
```
ip dhcp
show ip
ping 8.8.8.8
```
`show ip` e comandă de VPCS, nu merge pe Debian.

## Pasul 4: Debian cu DHCP

Login `debian` / `debian`, apoi:
```
sudo su
ip a
nano /etc/network/interfaces
```
Placa a fost **ens4** (verifici mereu cu `ip a`). În fișier decomentezi blocul DHCP:
```
auto ens4
iface ens4 inet dhcp
```
Blocul static rămâne comentat. Salvare: Ctrl+O, Enter. Ieșire: Ctrl+X. Apoi:
```
systemctl restart networking
ip a
ping 8.8.8.8
```

## Pasul 5: ping între noduri

- Din Debian: `ping -c 4 192.168.1.4`
- Din PC1: `ping 192.168.1.5`

## Pașii 6-9: pachete și verificări

```
apt update
apt install net-tools
ifconfig
ethtool ens4
```
- `ifconfig` arată aceleași IP și MAC ca `ip a` (masca 255.255.255.0 = /24, doar format diferit).
- `ethtool`: Speed 10000Mb/s, Duplex Full, Link detected yes (placă virtuală, auto-negotiation off).
- Dacă lipsește: `apt install ethtool`.

## Pașii 10-12: apache2

```
apt install apache2
sudo reboot
sudo netstat -anptu
```
Rezultat: `:80` în stare LISTEN, proces `apache2`.
- `tcp` = IPv4, `tcp6` = IPv6. Apache apare pe tcp6 (`:::80`), dar răspunde și pe IPv4.
- **Capcană:** fără root, coloana PID/Program name arată `-`, deci nu vezi procesul.

## Pașii 13-14: Firefox (webterm)

**Capcană:** webterm vine cu DHCP comentat și nu are IP implicit. Rezolvare:
click dreapta pe webterm-1 -> Edit config -> decomentezi blocul DHCP pentru eth0:
```
auto eth0
iface eth0 inet dhcp
```
Save, apoi Stop și Start pe nod (config-ul se citește doar la pornire).

Apoi în Firefox: `google.com` (Internet), apoi `http://IP_DEBIAN`. Apare "Apache2 Debian Default Page - It works!".

## Pasul 15: modificarea paginii

```
nano /var/www/html/index.html
```
Pui un HTML simplu, salvezi. În Firefox: **Ctrl+F5** (full refresh), altfel vezi pagina din cache.

## Pasul 16: IP static

În `/etc/network/interfaces`: comentezi blocul DHCP, decomentezi blocul static.

Acasă:
```
auto ens4
iface ens4 inet static
        address 192.168.1.50
        netmask 255.255.255.0
        gateway 192.168.1.1
        dns-nameservers 8.8.8.8
```

**La laborator:** address și netmask le dă profesorul, gateway = **172.20.0.1**.

Apoi `reboot` și reiei pașii 8, 12, 13, 14, 15:
- `ping 8.8.8.8`
- `ifconfig` (IP-ul static pe placă)
- `sudo netstat -anptu | grep :80` (apache2 LISTEN)
- Firefox: google.com, apoi `http://IP_STATIC`
- modifici `index.html`, Ctrl+F5

## Capcane de reținut

- Numele plăcii se verifică cu `ip a`, nu se presupune.
- Nu lăsa active în același timp blocurile dhcp și static pentru aceeași placă.
- `netstat` fără root nu arată procesul.
- webterm nu are IP până nu decomentezi DHCP în Edit config.
- Ctrl+F5 în Firefox după modificarea paginii.
- Cere profesorului și masca de rețea, nu doar IP-ul.
- Dacă ping 8.8.8.8 merge, dar google.com nu, adaugi `dns-nameservers 8.8.8.8`.
