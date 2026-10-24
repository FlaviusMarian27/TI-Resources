# Lucrarea 5 - VLAN în GNS3 (Administrarea rețelelor)

Pașii de rezolvare, în ordine. Switch-ul folosit: **Ethernet switch** din GNS3. Nodurile: **VPCS**.

## Partea 1: un switch, 2 VLAN-uri

### 1. Rețea simplă fără VLAN
Adaugi un Ethernet switch (Switch1) și 4 VPCS (PC1-PC4), conectate la Ethernet0-3. Setezi IP-urile:

```
PC1> ip 172.30.10.1/16
PC2> ip 172.30.10.2/16
PC3> ip 172.30.20.1/16
PC4> ip 172.30.20.2/16
```

Gateway-ul rămâne `0.0.0.0`, nu e nevoie de el (toate sunt în subnetul 172.30.0.0/16).

### 2. Verificare conectivitate
```
PC1> ping 172.30.10.2
PC1> ping 172.30.20.1
PC1> ping 172.30.20.2
```
Rezultat: **toate merg** (toate porturile sunt în VLAN 1).

### 3. Ștergerea conexiunilor
Ștergi **doar cablurile** dintre PC1-PC4 și Switch1. Nodurile rămân pornite (dacă le oprești, pierzi IP-urile).

### 4. Reconfigurarea switch-ului
Click dreapta pe Switch1 -> **Configure**:
1. Ștergi toate porturile din lista din dreapta.
2. Adaugi 4 porturi noi:

| Port | Tip | VLAN |
|------|-----|------|
| 0 | access | 120 |
| 1 | access | 120 |
| 2 | access | 121 |
| 3 | access | 121 |

3. **Apply** -> **OK**.

### 5. Refacerea conexiunilor
- PC1 -> Ethernet0
- PC2 -> Ethernet1
- PC3 -> Ethernet2
- PC4 -> Ethernet3

### 6. Verificare
| Ping | Rezultat |
|------|----------|
| PC1 <-> PC2 | merge (VLAN 120) |
| PC3 <-> PC4 | merge (VLAN 121) |
| PC1/PC2 <-> PC3/PC4 | `not reachable` (VLAN-uri diferite) |

### 7. Packet capture
Click dreapta pe cablu -> **Start capture** (pornit **înainte** de ping).
- Ping PC1 -> PC2: ARP request/reply, apoi ICMP echo request/reply.
- Ping PC1 -> PC3: doar ARP request-uri repetate, fără reply. Pe legătura lui PC3 nu apare nimic.

## Partea 2: al doilea switch și trunk

### 8. Switch2
Adaugi Switch2 (încă **neconectat** la Switch1). Configure:
1. Ștergi toate porturile.
2. Adaugi:

| Port | Tip | VLAN |
|------|-----|------|
| 0 | access | 120 |
| 1 | access | 121 |

### 9. PC5 și PC6
- PC5 -> Switch2, Ethernet0
- PC6 -> Switch2, Ethernet1

### 10. Adrese IP
```
PC5> ip 172.30.10.3/16
PC6> ip 172.30.20.3/16
```

### 11. Verificare PC5 <-> PC6
```
PC5> ping 172.30.20.3
```
Rezultat: `not reachable` (VLAN 120 vs 121).

### 12. Verificare izolare față de Switch1
PC5 și PC6 nu comunică cu PC1-PC4 (switch-urile nu sunt legate).

### 13. Porturile trunk
GNS3 nu permite modificarea unui switch deja conectat (*Can't modify a switch already connected*), deci:
1. Ștergi cablurile de pe **Switch1** (PC1-PC4) și de pe **Switch2** (PC5, PC6). Nodurile rămân pornite.
2. Switch1 -> Configure: adaugi portul `4`, tip **dot1q**, VLAN `1`. Porturile 0-3 rămân neschimbate.
3. Switch2 -> Configure: adaugi portul `2`, tip **dot1q**, VLAN `1`.
4. Apply -> OK pe ambele.
5. Refaci cablurile PC-urilor (PC1-4 pe Switch1 la Ethernet0-3, PC5 la Ethernet0 și PC6 la Ethernet1 pe Switch2).

VLAN 1 de pe portul dot1q este **native VLAN** și trebuie să fie identic pe ambele capete.

Configurația finală:

| Switch | Port | Tip | VLAN |
|--------|------|-----|------|
| Switch1 | 0, 1 | access | 120 |
| Switch1 | 2, 3 | access | 121 |
| Switch1 | 4 | dot1q | 1 |
| Switch2 | 0 | access | 120 |
| Switch2 | 1 | access | 121 |
| Switch2 | 2 | dot1q | 1 |

### 14. Legătura între switch-uri
Cablu **Switch1 Ethernet4 <-> Switch2 Ethernet2**.

### 15. Testele finale
| Ping | Rezultat |
|------|----------|
| PC1, PC2, PC5 între ele (VLAN 120) | merge |
| PC3, PC4, PC6 între ele (VLAN 121) | merge |
| PC1/PC2/PC5 <-> PC3/PC4/PC6 | `not reachable` |

### 16. Packet capture pe trunk
Captură pe cablul Switch1 <-> Switch2, apoi ping PC1 -> PC5 și PC3 -> PC6.
În Wireshark, sub **802.1Q Virtual LAN**, se vede tag-ul:
- pachetele PC1 <-> PC5: `ID: 120`
- pachetele PC3 <-> PC6: `ID: 121`

Pe legăturile access (PC <-> switch) pachetele sunt **fără tag**.

## Topologia finală

```
PC1 (e0) --- e0 [Switch1] e4 ============ e2 [Switch2] e0 --- PC5
PC2 (e0) --- e1     |                           e1 --- PC6
PC3 (e0) --- e2     |
PC4 (e0) --- e3
```

## Concluzii
- VLAN-ul separă rețeaua **logic, la Layer 2**, fără să schimbi adresarea IP.
- Porturile **access**: fără tag, aparțin unui singur VLAN.
- Porturile **trunk (dot1q)**: transportă mai multe VLAN-uri, cu tag 802.1Q (4 bytes inserați între Src MAC și EtherType).
- Același subnet, VLAN-uri diferite: ARP-ul pleacă, dar nu primește reply, deci `not reachable` (nu "No gateway found").