## 🔹 Ce este un Dockerfile?

Dockerfile este un fișier text care conține instrucțiuni pentru construirea unei imagini Docker.

Este „rețeta” după care Docker construiește un mediu complet, automatizat, pentru aplicația ta.

💡 Imaginează-ți:

- Dockerfile = rețetă

- Imagine = mâncarea gătită

- Container = farfuria în care servești

---

## 🔹 1. Creează un proiect de test

```bash
mkdir -p ~/docker-learning/cap2
cd ~/docker-learning/cap2
```
## 🔹 2. Creează o aplicație simplă în Python

```bash
cat << 'EOF' > app.py
print("Salut, Docker! Rulez dintr-un container Python 🚀")
EOF
```

---

## 🔹 3. Scrie fișierul Dockerfile

```bash
cat << 'EOF' > Dockerfile
# Aleg imaginea de bază (Python minimal)
FROM python:3.10-slim

# Setez directorul de lucru
WORKDIR /app

# Copiez fișierul aplicației în container
COPY app.py .

# Definim comanda care va rula automat
CMD ["python", "app.py"]
EOF
```

---

## 🔹 4. Construiește imaginea

```bash
docker build -t myapp .
```

- `-t myapp` → numele imaginii create
- `.` → indică folderul curent (unde este Dockerfile-ul)

---

## 🔹 5. Verifică imaginile existente

```bash
docker images
```

---

## 🔹 6. Rulează containerul

```bash 
docker run myapp
```

🟢 Output așteptat:

```bash
Salut, Docker! Rulez dintr-un container Python 🚀
```

---

## 🔹 7. Listează containerele existente

```bash
docker ps -a
```

📋 Vezi starea lor (`Up`, `Exited`, etc.)

---

## 🔹 8. Repornește sau șterge containerele

```bash
# Repornește un container oprit
docker start -ai <container_id>

# Șterge containerele oprite
docker rm $(docker ps -aq)
```

---

## 🔹 9. Curățare generală (opțional)

```bash
# Șterge imaginea creată
docker rmi myapp

# Curăță toate resursele neutilizate
docker system prune -a
```

---

## ✅ Recapitulare

- **Dockerfile** → definește imaginea
- **docker build** → creează imaginea
- **docker run** → pornește containerul
- **docker ps -a** → verifică containerele
- **docker system prune -a** → curăță tot ce e nefolosit