# Administrarea rețelelor: Laborator 2, partea 3 (switch simplu)

Continuare după capitolul 1 (vezi `README.md`). Aici am adăugat un switch și două noduri Firefox.

## Topologie (proiect `Partea3`)

- **NAT1** legat de `e0` (primul port) al **Switch1** (Ethernet switch).
- **Firefox31.1.1~2-1** (nod 1) pe `e1`.
- **Firefox31.1.1~2-2** (nod 2) pe `e2`.
- Am folosit **NAT** în loc de Cloud, pentru că laptopul e pe Wi-Fi (Cloud nu merge pe Wi-Fi, vezi `README.md`). La laborator, pe PC cu cablu, NAT-ul se înlocuiește cu Cloud pe interfața cu cablu.
- Etichetele porturilor se văd din *View → Show/Hide interface labels*.
- Punctele roșii de la noduri = oprite. *Start all nodes* le face verzi.

## 3.1 Conexiuni

Cloud/NAT pe primul port al switch-ului (`e0`), Firefox-urile pe celelalte porturi (`e1`, `e2`).

## 3.2 DHCP

Nodurile Firefox QEMU nu au fișier de rețea de editat, iau IP prin DHCP singure. Ambele au primit adresă din `192.168.122.0/24`.

## 3.3 Pornire

*Start all nodes*, apoi click dreapta pe fiecare nod → *console* (VNC).

## 3.4 Exercițiile din capitolul 1, pe fiecare nod

| | Nod 1 (`~2-1`) | Nod 2 (`~2-2`) |
|---|---|---|
| Placa activă | `eth0` | `eth0` |
| MAC | `0c:56:a8:5e:00:00` | `0c:da:e0:f9:00:00` |
| IP | `192.168.122.77/24` | `192.168.122.97/24` |
| Gateway | `192.168.122.1` | `192.168.122.1` |

- `ip link`, `ip address show`, `ip route show` și `ifconfig` merg pe ambele noduri.
- MAC-urile și IP-urile sunt **diferite**, gateway-ul e **același** (aceeași rețea, prin același switch și NAT).
- Pe nodul 1, `ping 8.8.8.8`: 0% loss, ~40-44 ms, `ttl=114`.

## 3.5 Ping între cele două noduri (ambele sensuri)

| Sens | Comandă | Rezultat |
|---|---|---|
| Nod 1 → Nod 2 | `ping 192.168.122.97` | 8/8 primite, 0% loss, ~2.6-4.7 ms |
| Nod 2 → Nod 1 | `ping 192.168.122.77` | 5/5 primite, 0% loss, ~0.7-1 ms |

Timpurile sunt mai mari decât ping-ul către IP-ul propriu (~0.15 ms), pentru că pachetul iese din VM, trece prin switch și ajunge în altă mașină. Primul pachet e mai lent și pentru că trebuie rezolvat MAC-ul prin ARP.

### `arp` după ping

Au apărut **2 intrări** pe fiecare nod (la partea 1 era doar una):

| Pe nodul | Intrări |
|---|---|
| Nod 1 | `192.168.122.1` (gateway) la `52:54:00:cd:7e:3e`; `192.168.122.97` (nod 2) la `0c:da:e0:f9:00:00` |
| Nod 2 | `192.168.122.77` (nod 1) la `0c:56:a8:5e:00:00`; `192.168.122.1` (gateway) la `52:54:00:cd:7e:3e` |

Explicație: ARP ține IP → MAC pentru echipamentele din aceeași rețea locală cu care am comunicat. Acum am vorbit atât cu gateway-ul, cât și cu celălalt nod, deci sunt două intrări.

## 3.6 Ping între nodurile colegilor

Nefăcut încă, doar la laborator. Colegul rulează `ip address show` în nodul lui și îmi dă IP-ul de la `eth0`, apoi dăm ping între noi în ambele sensuri.

Necesită rețea reală: **Cloud pe interfața cu cablu**, nu NAT (pe NAT fiecare are rețeaua lui internă ascunsă). IP-urile vor fi din rețeaua laboratorului, nu `192.168.122.x`.

## De reținut / capcane

- Primul port al switch-ului trebuie să fie cel către Cloud/NAT.
- Ambele noduri trebuie pornite înainte de ping.
- Ping către alt nod: IP-ul lui, nu al meu. La ARP apar doar echipamentele cu care am comunicat.
- Valorile (IP, MAC) diferă în laborator, se citesc din comenzi.

## De făcut

- Rulat pe ambele noduri: `ping` către gateway, `10.0.0.1`, `google.com`, `traceroute 8.8.8.8`, dacă nu sunt făcute.
- 3.6 la laborator.
