## Pasul 1: Topologia Fizică și Conectarea Echipamentelor

[cite_start]**Obiectiv:** Implementarea topologiei hardware conform scenariului, delimitând rețeaua internă a firmei de rețeaua externă (ISP / Internet)[cite: 95, 103].

### 1. Echipamente utilizate
[cite_start]Conform cerințelor obligatorii[cite: 106]:
* **Rețeaua internă (Firma):** * 1x Router (Cisco 1941) - `Router_Firma`
  * 2x Switch-uri (Cisco 2960) - `SW1` și `SW2`
  * 3x PC-uri - `PC1-IT`, `PC2-HR`, `PC3-Guest`
  * 1x Server - `Server-Intern`
* **Rețeaua externă (ISP):** * 1x Router (Cisco 1941) - `Router_ISP`
  * 1x Server - `Server-Extern`

### 2. Tabelul Conexiunilor Fizice
[cite_start]Toate legăturile au fost realizate exact în următoarea ordine, respectând porturile specifice pentru a facilita configurarea ulterioară [cite: 113-117]:

| Ordine | Echipament Sursă | Port Sursă | Echipament Destinație | Port Destinație | Tip Cablu | Observații |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | PC1-IT | FastEthernet0 | SW1 | FastEthernet0/1 | Copper Straight-Through | Cablu rețea standard |
| **2** | PC2-HR | FastEthernet0 | SW1 | FastEthernet0/2 | Copper Straight-Through | Cablu rețea standard |
| **3** | PC3-Guest | FastEthernet0 | SW2 | FastEthernet0/2 | Copper Straight-Through | Cablu rețea standard |
| **4** | Server-Intern | FastEthernet0 | SW2 | FastEthernet0/4 | Copper Straight-Through | Cablu rețea standard |
| **5** | SW1 | FastEthernet0/3 | SW2 | FastEthernet0/1 | Copper Straight-Through | Legătură Trunk între switch-uri |
| **6** | SW2 | GigabitEthernet0/1| Router_Firma | GigabitEthernet0/0| Copper Straight-Through | Legătură Trunk spre router |
| **7** | Router_ISP | Serial0/1/0 | Router_Firma | Serial0/1/0 | Serial DCE (roșu) | Necesită modul **HWIC-2T**. *Ceasul se află pe Router_ISP*. |
| **8** | Router_ISP | GigabitEthernet0/1| Server-Extern | FastEthernet0 | Copper Straight-Through | Legătură server public |

**Note tehnice privind cablarea:**
* S-a folosit cablu **Copper Straight-Through** (negru continuu) pentru conexiunile locale LAN (de mare viteză).
* Pentru conexiunea WAN dintre rețeaua firmei și ISP, routerele au fost oprite temporar pentru instalarea modulelor hardware **HWIC-2T**. Conexiunea s-a realizat cu un cablu **Serial DCE**, inițiat dinspre `Router_ISP` spre `Router_Firma`, stabilind astfel ISP-ul ca echipament care va dicta lățimea de bandă (clock rate).