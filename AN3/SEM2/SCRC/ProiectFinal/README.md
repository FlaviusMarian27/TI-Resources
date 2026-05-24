## Pasul 1: Topologia Fizică și Conectarea Echipamentelor

**Obiectiv:** Implementarea topologiei hardware conform scenariului, delimitând rețeaua internă a firmei de rețeaua externă (ISP / Internet).

**1. Echipamente utilizate:**
* **Rețeaua internă (Firma):** * 1x Router (Cisco 1941) - `Router_Firma`
  * 2x Switch-uri (Cisco 2960) - `SW1` și `SW2`
  * 3x PC-uri - `PC1-IT`, `PC2-HR`, `PC3-Guest`
  * 1x Server - `Server-Intern`
* **Rețeaua externă (ISP):** * 1x Router (Cisco 1941) - `Router_ISP`
  * 1x Server - `Server-Extern`

**2. Conexiuni fizice realizate:**
* **Cablare LAN (Copper Straight-Through):** Am folosit cablu Ethernet standard (negru) pentru conexiunile locale de mare viteză: PC-uri la switch-uri, Server la switch, interconectarea switch-urilor (Trunk) și legătura Gigabit dintre SW2 și Router_Firma.
* **Cablare WAN (Serial DCE):** Pentru a simula o conexiune reală la distanță între firmă și furnizorul de internet, am procedat astfel:
  * Am oprit routerele și am adăugat modulele hardware **HWIC-2T** pentru a beneficia de interfețe seriale.
  * Am conectat cele două routere folosind un cablu **Serial DCE** (roșu cu ceas). Cablul a fost inițiat de la `Router_ISP` către `Router_Firma`, stabilind astfel ISP-ul ca echipament DCE (cel care dictează rata de ceas / viteza conexiunii).