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