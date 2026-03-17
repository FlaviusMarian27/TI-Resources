using System;
using System.Diagnostics;

bool ruleaza = true;

while (ruleaza){
    Console.WriteLine("1. Exercitiul 1");
    Console.WriteLine("2. Exercitiul 2");
    Console.WriteLine("3. Exercitiul 3");
    Console.WriteLine("4. Exercitiul 5");
    Console.WriteLine("5. Ieșire din program");

    int optiune = Convert.ToInt32(Console.ReadLine());
    Console.WriteLine("------------------------");

    switch (optiune){
        case 1:
            /*
                Modificați programele C# și F# de la punctul 2 pentru a calcula suma a 
                douănumere introduse de utilizator. Afișați rezultatul sub forma: "Suma 
                numerelor <a> și <b> este <suma>.", unde <a>, <b> și <suma> vor fi 
                înlocuite cu valorile efective.
            */
            Console.Write("Inroduceti primul numar: ");
            int a = Convert.ToInt32(Console.ReadLine());

            Console.Write("Inroduceti al doilea numar: ");
            int b = Convert.ToInt32(Console.ReadLine());

            int suma = a + b;

            Console.WriteLine($"Suma numerelor {a} și {b} este {suma}.");
            break;

        case 2:
            /*
                Creați un program C# care afișează versiunea curentă a .NET SDK folosită.
                (Indiciu: utilizați comanda dotnet --version în terminal).
            */
            Process.Start("dotnet", "--version").WaitForExit();
            break;

        case 3:
            /*
                Scrieți un program C# care cere utilizatorului să introducă raza unui cerc și
                calculează și afișează aria și circumferința cercului, folosind valorile introduse.
                (Considerați π = 3.1415926)
            */
            Console.Write("Alege raza unui cerc: ");
            double raza = Convert.ToDouble(Console.ReadLine());

            double pi = 3.1415926;

            double aria = pi * raza * raza;

            double circumferinta = 2 * pi * raza;

            Console.WriteLine($"Aria cercului este: {aria}");
            Console.WriteLine($"Circumferinaa cercului este: {circumferinta}");
            break;

        case 4:
            /*
            (Mediu) Scrieți un program C# care cere utilizatorului să introducă două șiruri de
            caractere și verifică dacă primul șir apare în al doilea șir. Afișați un mesaj
            corespunzător.
            */

            Console.Write("String 1: ");
            string str1 = Console.ReadLine();

            Console.Write("String 2: ");
            string str2 = Console.ReadLine();

            int index = str2.IndexOf(str1);

            if(index != -1){
                Console.WriteLine("Am gasit!");
            }else{
                Console.WriteLine("Nu am gasit!");   
            }

            break;

        case 5:
            ruleaza = false;
            Console.WriteLine("Iesire...");
            break;
        
        default:
            Console.WriteLine("Invalid!");
            break;
    }
}