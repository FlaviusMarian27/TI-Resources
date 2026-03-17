open System;

printf "Inroduceti primul numar: "
let a = Convert.ToInt32(Console.ReadLine());

printf "Inroduceti al doilea numar: "
let b = Convert.ToInt32(Console.ReadLine());

let suma = a + b;

printfn $"Suma numerelor {a} și {b} este {suma}."