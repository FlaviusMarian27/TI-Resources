module Program

open System

//============================Laboratorul 8===========================

printfn "============================Laboratorul 8==========================="
printfn ""

printfn "-------------Problema 3-------------"
//3. Utilizați o expresie de tip "list comprehension" pentru a genera o 
//listă cu primele 20 de numere din șirul lui Fibonacci.
let rec fibonacci number = 
    if number <= 0 then 0
    elif number <= 1 then 1
    else fibonacci(number - 1) + fibonacci(number - 2)

let nrFib = [for i in 0 .. 19 do yield fibonacci i]

printfn "Primele 20 de numere din sirul lui fibonacci:"
printfn "%A" nrFib

printfn ""
printfn "-------------Problema 5-------------"
//Scrieți o funcție recursivă care primește un număr întreg și 
//returnează suma cifrelor sale. 
let rec sumDigits n = 
    let number = abs(n)

    if number = 0 then 0
    else (number % 10) + sumDigits(number / 10)

let result = sumDigits 345
printfn "Suma cifrelor numarului %d este: %d" 345 result

//============================Laboratorul 9===========================
printfn ""
printfn "============================Laboratorul 9==========================="
printfn ""

printfn "-------------Problema 1-------------"
//1. Scrieți o funcție recursivă care primește un număr 
//întreg și returnează suma cifrelor sale.
let rec sumaDigits n = 
    let number = abs(n)

    if number = 0 then 0
    else (number % 10) + sumaDigits(number / 10)

let result1 = sumaDigits 345
printfn "Suma cifrelor numarului %d este: %d" 345 result1

printfn ""
printfn "-------------Problema 2-------------"
//2. Implementați o funcție care primește o listă de numere întregi și 
//returnează o listă cu pătratele numerelor impare folosind funcția List.map.
let patrateImpare lista = 
    lista
    |> List.filter(fun x -> x % 2 = 0)
    |> List.map(fun x -> x * x)

let listaTest = [1; 2; 3; 4; 5; 6; 7; 8; 9]
let resultList = patrateImpare listaTest

printfn "Lista initiala: %A" listaTest
printfn "Lista modificata: %A" resultList

//============================Laboratorul 10===========================
printfn ""
printfn "============================Laboratorul 10==========================="
printfn ""

printfn "-------------Problema 1-------------"
//1. Definiți un tip record Angajat cu câmpurile Nume, Departament și Salariu. 
//Creați o listă de angajați și calculați media salariilor.
type Angajat = {
    Nume: string
    Departament: string
    Salariu: float
}

let listaAngajati = [
    {Nume = "Flavius"; Departament = "IT"; Salariu = 11000.0}
    {Nume = "Andrei"; Departament = "IT"; Salariu = 8500.0}
    {Nume = "Maria"; Departament = "HR"; Salariu = 5000.0}
    {Nume = "Ioana"; Departament = "Design"; Salariu = 7000.0}
]
let media = 
    listaAngajati
    |> List.map(fun a -> a.Salariu)
    |> List.average

printfn "Lista angajatilor: "
for i in listaAngajati do
    printfn " - %s | Dep: %s | Salariu: %.2f RON" i.Nume i.Departament i.Salariu
printfn "Media salariilor este: %.2f RON" media

printfn ""
printfn "-------------Problema 2-------------"
//2. Definiți un tip discriminat Vehicul cu variantele Masina, Bicicleta  și 
//Camion. Scrieți o funcție care primește o listă de vehicule și returnează 
//numărul total de roți (considerând că o mașină are 4 roți, o bicicletă 
//are 2 roți și un camion are 6 roți). 
type Vehicul = 
    | Masina
    | Bicicleta
    | Camion

let numaraRoti v =
    match v with
    | Masina -> 4
    | Bicicleta -> 2
    | Camion -> 6

let listaVehicule = [Masina; Bicicleta; Camion; Masina; Camion]

let totalRoti = listaVehicule |> List.sumBy numaraRoti
printfn "Vehiculele din parcare sunt: %A" listaVehicule
printfn "Numar total de roti: %d" totalRoti

//============================Laboratorul 11===========================
printfn ""
printfn "============================Laboratorul 11==========================="
printfn ""

printfn "-------------Problema 1-------------"
//1. Scrieți o funcție care primește o listă de numere întregi și 
//returnează o listă cu pătratele numerelor impare.
let patrateImpareL11 lista =
    lista
    |> List.filter (fun x -> x % 2 <> 0) 
    |> List.map (fun x -> x * x)         

let listaEx1Lab11 = [1; 2; 3; 4; 5; 6; 7; 8; 9]
let rezultatEx1Lab11 = patrateImpareL11 listaEx1Lab11

printfn "Lista initiala: %A" listaEx1Lab11
printfn "Patratele numerelor impare sunt: %A" rezultatEx1Lab11

printfn ""
printfn "-------------Problema 3-------------"
//3. Scrieți o funcție care primește o listă de numere întregi și 
//returnează produsul numerelor pare.
let produsulNumerePare lista =
    lista
    |> List.filter (fun x -> x % 2 = 0)
    |> List.fold (fun acumulator x -> acumulator * x) 1

let numereleMele = [1;2;3;4;5;6]
let resultPare = produsulNumerePare numereleMele

printfn "Lista initiala: %A" numereleMele
printfn "Produsul numerelor pare este: %d" resultPare

//============================Laboratorul 12===========================
printfn ""
printfn "============================Laboratorul 12==========================="
printfn ""

printfn "-------------Problema 1-------------"
//1. Implementați o funcție divide care primește doi întregi și returnează 
//un rezultat Result<int, string> reprezentând împărțirea celor două 
//numere. În cazul în care al doilea număr este zero, returnați un mesaj de 
//eroare adecvat.
let divide a b =
    if b = 0 then
        Error "Eroare: Impartire la zero!"
    else
        Ok (a / b)

let resultGood = divide 10 2
let resultBad = divide 10 0
printfn "10 impartit la 2: %A" resultGood
printfn "10 impartit la 0: %A" resultBad

printfn ""
printfn "-------------Problema 5-------------"
//5. Scrieți o funcție parseDate care primește un șir de caractere 
//reprezentând o dată în formatul "YYYY-MM-DD" și returnează un rezultat 
//Result<DateTime,string>. Folosiți funcțiile din modulul System.DateTime 
//pentru a parsa data și returnați un mesaj de eroare adecvat în cazul 
//în care formatul datei este invalid.
let parseDate (input: string) =
    match DateTime.TryParse(input) with
    | true, data -> Ok data
    | false, _   -> Error "Eroare. Format nu este bun"

let dataValida = parseDate "2004-05-27"
let dataInvalida = parseDate "Salut!"

printfn "Corect: %A" dataValida
printfn "Incorect: %A" dataInvalida