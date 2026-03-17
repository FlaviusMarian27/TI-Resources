open System;

printf "Inroduceti primul numar: "
let a = Convert.ToInt32(Console.ReadLine());

printf "Inroduceti al doilea numar: "
let b = Convert.ToInt32(Console.ReadLine());

let suma = a + b;

printfn $"Suma numerelor {a} și {b} este {suma}."

//ex4
// Creați un program F# care convertește o temperatură introdusă de utilizator din
// grade Celsius în grade Fahrenheit, folosind formula: fahrenheit = celsius
// * 9 / 5 + 32.
printf "Alege temperatura in grade celsius: "
let c = Convert.ToInt32(Console.ReadLine());

let fahrenheit = c * 9 / 5 + 32

printfn $"{fahrenheit}"