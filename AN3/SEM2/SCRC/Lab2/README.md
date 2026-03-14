# Ghid de Configurare: Laboratorul 2 - VLAN-uri pe Switch

Acest document explică pașii necesari pentru configurarea Virtual LAN-urilor (VLAN-urilor) pe un switch Cisco. Scopul acestui laborator este crearea VLAN-urilor pe un switch, asocierea porturilor la aceste VLAN-uri și verificarea separării traficului între ele.

## 1. Topologie și Adrese IP

Pentru acest laborator s-au adăugat un Switch (model 2960) și 4 PC-uri. Echipamentele au fost conectate cu cabluri Straight-Through în felul următor:

* **PC1** conectat la portul **Fa0/1**
* **PC2** conectat la portul **Fa0/2**
* **PC3** conectat la portul **Fa0/3**
* **PC4** conectat la portul **Fa0/4**



Înainte de a configura switch-ul, fiecare PC trebuie să primească o adresă IP din meniul Desktop → IP Configuration.

| VLAN | PC | Adresă IP | Subnet Mask |
| :--- | :--- | :--- | :--- |
| 10 (IT) | PC1 | 192.168.10.2 | 255.255.255.0 |
| 10 (IT) | PC2 | 192.168.10.3 | 255.255.255.0 |
| 20 (HR) | PC3 | 192.168.20.2 | 255.255.255.0 |
| 20 (HR) | PC4 | 192.168.20.3 | 255.255.255.0 |

> **Notă importantă pentru studenți:** În varianta inițială a îndrumătorului, PC3 și PC4 aveau ambele adresa `192.168.20.2/24`. Pentru a evita un conflict de IP-uri, adresa pentru PC4 a fost corectată manual la `.3`.

---

## 2. Accesarea CLI și Crearea VLAN-urilor

Vom crea două VLAN-uri: VLAN 10-IT și VLAN 20-HR. Dăm click pe Switch și intrăm în tab-ul CLI.

```cisco
enable
configure terminal
```
* `enable`: Inițializează modul privilegiat (modul vizualizare/profesor).
* `configure terminal`: Permite intrarea în modul de configurare globală, unde putem aduce modificări echipamentului.

```cisco
vlan 10
name IT
exit
vlan 20
name HR
exit
```
* `vlan 10` / `vlan 20`: Creează VLAN-ul cu numărul specificat.
* `name IT` / `name HR`: Atribuie un nume descriptiv rețelei virtuale create.
* `exit`: Ne întoarce un pas înapoi în meniul de configurare.

Pentru a verifica dacă VLAN-urile au fost create corect, folosim comanda `show vlan brief` (în modul privilegiat).

---

## 3. Configurarea porturilor pentru VLAN-uri

Acum că am creat "camerele" virtuale, trebuie să mutăm porturile fizice în ele. PC1 și PC2 vor face parte din VLAN 10. PC3 și PC4 vor face parte din VLAN 20.

```cisco
interface fa0/1
switchport mode access
switchport access vlan 10

interface fa0/2
switchport mode access
switchport access vlan 10
```
* `interface fa0/1`: Selectează portul fizic pe care dorim să îl configurăm.
* `switchport mode access`: Forțează portul să funcționeze în modul de acces (destinat calculatoarelor finale, nu altor switch-uri).
* `switchport access vlan 10`: Introduce portul respectiv în VLAN 10.

*Procesul se repetă pentru porturile `fa0/3` și `fa0/4`, alocându-le către `vlan 20`.*

---

## 4. Testarea Conectivității (Ping)

Pentru a demonstra că separarea traficului funcționează, deschidem Command Prompt pe PC1 și rulăm comenzi Ping.

1.  **Test intern (reușit):** `ping 192.168.10.3`. Răspunsul așteptat este "Reply from 192.168.10.3", cu 0% loss. Acest lucru confirmă că PC1 comunică perfect cu PC2 (ambele în VLAN 10).
2.  **Test de izolare (eșuat intenționat):** `ping 192.168.20.2`. Răspunsul așteptat este "Request timed out". Acest lucru confirmă că switch-ul blochează cu succes traficul între departamentul IT (VLAN 10) și departamentul HR (VLAN 20).

---

## 5. Salvarea Configurației

Orice configurare făcută se află în memoria RAM (temporară). Pentru a nu pierde munca la repornirea echipamentului, din modul privilegiat rulăm:

```cisco
copy running-config startup-config
```
* `copy running-config startup-config`: Copiază configurația curentă din memoria volatilă în memoria NVRAM (stabilă).