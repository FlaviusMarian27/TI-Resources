
## 1. Ce este DOCKER- ul?

-  un  software virtualizat
-  dezvoltarea și implementarea aplicațiilor este mult mai ușoară
- Docker-ul împachetează aplicația + librăriile + configurațiile + mediul de execuție într-o singură imagine portabilă
- portabil, ușor de partajat și distribuit

### Ce probleme a rezolvat Docker-ul?

Practic nu mai trebuie să faci anumite configurări pentru sistemul tău de operare.

---

## 2. Virtual Machine(VM) vs Docker

 - Astfel orice sistem de operare are 2 layere importante, mai exact `OS Application Layer` și  `OS Kernel`
 
![Layers](images/layers.png)
- practic layer-ul de kernel comunică cu partea hardware(CPU, memory etc), fiind cel care face legătura între hardware și aplicații.
- în layer-ul de aplicații se află, practic aplicații propriu zise, cum ar fi Chrome, Word etc.

#### Docker
- Docker-ul virtualizeaza layer-ul  de aplicații doar.
- practic când rulezi container-ul, acesta conține aplicațiile din layer-ul de aplicații al sistemului de operare și alte aplicații instalate în vârful layerului de aplicații, cum ar fi pentru Java la runtime sau în alte limbaje cum ar fi python.

#### VM
- folosește ambele layere, practic are complet acces la sistemul de operare.
- când descarci o imagine, atunci se folosește kernel-uș propriu zis.

---

## 3. Diferența dintre Docker și VM

 - imaginile de pe **Docker** sunt mult mai mici, pentru că folosesc practic doar un singur layer din sistemul de operare, pe când pentru VM sunt mai mari pentru că folosește ambele layere.

- Dimensiunea imaginilor în **Docker** pot ajunge de ordinul MB, pe când în VM pot ajunge de ordinul GB.

![Dimensiiuni](images/MBGB.png)
 
 - O altă mare diferență ar viteza. Practic pentru un **VM** poate lua câteva minute să pornească, pe când **Docker-ul** are nevoie de câteva secunde, deoarece acesta folosește de fiecare dată kernelul hostului, pe când **VM-ul** kernelul propriului calculator.
- Compatibilitatea poate reprezenta un alt factor esențial. Astfel o **VM** este compatibilă cu orice sistem de operare, pe când **Docker-ul** nu este la fel de compatibil, fiind conceput pentru Linux. 

### Oare de ce apare o astfel de incompatibilitate? 

- Răspunsul este unul simplu, practic având layer-ul de aplicații pentru Linux, avem nevoie de Kernelul de Linux, nu de windows/MAC OS, astfel de aici apare incompatibilitatea. 
-  O rezolvare a fost practic **Docker Desktop**, care are un ***Hypervisor***, care oferă o compatibilitate cu **Windows** și **MAC OS** pentru a putea rula imaginia pe Linux fără vreo problemă.

![Dimensiiuni](images/DOCKERDESKTOP.png)

---
## 4. Docker Images vs Docker Containers

- Docker-ul poate împacheta în artefact, cum ar fi zip, tar, jar etc și să trimită direct către server imaginea.

#### Docker image

-  este un artefact al aplicației executabile.
-  conține codul sursă al aplicației.
-  îmbunătățiri adăugate pentru varibile, cum ar fi crearea de directoare, fișiere etc.

#### Docker container

-  pornește aplicația.
-  rulează o instanță a imaginii.
-  din aceeași imagine se pot rula mai multe containers.


Comenzi: 

- Pentru listarea tuturor imaginiilor:

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker images
```

![Docker](images/DockerImages.png)

- Listarea tuturor containers care ruleaza:

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker ps
```

![Docker](images/DockerPs.png)

---

## 5. Docker Registries

- reprezintă un storage pentru toate Docker images de tip artefact. Un exemplu ar putea fi Docker Hub-ul.

![Docker3](images/DockerRegistries.png)

- o altă comandă esențială ar putea fi **pull**. 

##### docker pull {nume_imagine}:{tag/versiune}

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker pull nginx:1.29
```

- practic clientul va contacta Docker Hub că vrea imaginea nginx cu tagul specificat, totul local.

![Docker4](images/DockerPull.png)
- pentru ce curioși pot folosi comanda images.

---
## 6. Run an Image

- dacă dorim să rulăm o imagine avem nevoie de comanda run.

##### docker run {nume_imagine}:{tag/versiune}

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker run nginx:1.29
```

- practic știm că un container a rulat cu succes atunci când vedem logurile.

![Docker4](images/DockerRun.png)

- **Atenție**!!! Terminalul este practic blocat. Dacă deschidem o altă fereastră pentru un nou terminal și vom tasta comanda **ps** vom vedea ce containere rulează. Pentru  a opri rularea apasă combinația de taste ***CTRL + C***, iar procesul moare.

![Docker5](images/DockerRunPs.png)

-  Pentru a evita ***detached mode*** sau blocarea terminalului adăugam ***-d***.

##### docker run -d {nume_imagine}:{tag/versiune}

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker run -d nginx:1.29
```

![Docker5](images/DockerRunD.png)

- practic rezultatul opținut este id-ul full al container-ului. Astfel cu comanda ***ps*** verificăm containerele care rulează și cu ***logs*** logurile din container.

![Docker5](images/DockerRunPsLogs.png)

- pentru a opri un anumit container avem o funcție dedicată pentru aceasta, comanda ***stop***.
- practic cu aceasta putem să oprim unul sau mai multe containere care rulează.

##### docker stop id_container

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker stop a5032524b655
```

---

## 7. Port Binding

#### Container Port vs Host Port
- practic, fiecare aplicație rulează într-un anumit port.
- astfel dacă dorim, putem să specificicăm portul dorit cu comanda ***-p*** sau ***-publish*** un port public pentru host.

##### docker run -d -p {Host_port}:{container_port} image:tag

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker run -d -p 9000:80 nginx:1.29
```

![Docker6](images/DockerRunDP.png)

![Docker7](images/LocalHost.png)

- astfel ce se poate observa, este faptul că după ce am rulat comanda ***docker ps***, apare portul pe care l-am selectat.
- totuși este recomandat să folosim pentru portul dedicat host-ului același port ca cel al containerului.
- de exemplu pentru MySQL, dacă portul containerului este 3306, atunci și portul pentru host va fi 3306. Practic este un standard.

![Docker8](images/MySQLPort.png)

---

## 8. Start and  stop container

- Atenție!! Cu comanda ***run*** practic putem să rulă un container, dar de ficecare dată când alegem să rulăm acel container, practic noi creăm un al container nou, cum un alt id, astfel ducând la umplerea memoriei de pe dispozitivul nostru.
- nu refolosește același container de fiecare dată.
- astfel dacă folosim comanda ***ps -a*** ne va lista toate containerele existente, oprite sau pornite.

##### docker ps -a or -all

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker ps -a
```

![Docker9](images/DockerPsAAll.png)

- astfel dacă dorim să pornim același Docker container, ne folosim de comanda ***start*** cu care putem să pornim unul sau mai multe containere.

##### docker start {container_id / nume container}

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker start 6c5e76295c5f
```

- o altă comandă, ar fi ***--name*** care ne permite să asignăm un nume pentru containerul nostru.
##### docker run --name nume -d -p {Host_port}:{container_port} image:tag


```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker run --name web-app -d -p 9000:80 nginx:1.29
```


![Docker9](images/DockerRunNameDP.png)

---

## 9. Public and Private Docker Registries

### Public

- un exemplu bun ar putea fi chiar Docker Hub

![Docker10](images/DockerHub.png)

- de ce este o variantă public? Pentru că toată lumea are acces, având permisiunea să caute și să descarce fără vreo problemă.

### Private
- un exemplu bun ar putea să fie furnizorii mari de cloud, aceștia oferă private registeries.
- practic ai nevoie de un cont, cu care să te autentifici înainte de a accesa registrul.

![Docker11](images/Cloud.png)


---

## 10. Create own Images
