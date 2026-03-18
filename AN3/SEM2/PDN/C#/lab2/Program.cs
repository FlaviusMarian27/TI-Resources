using System;
using System.Data;

bool ruleaza = true;

while (ruleaza){
    Console.WriteLine("\n=== MENIU LABORATOR 2 ===");
    Console.WriteLine("1. Exercitiul 1");
    Console.WriteLine("2. Exercitiul 2");
    Console.WriteLine("3. Exercitiul 3");
    Console.WriteLine("4. Exercitiul 4");
    Console.WriteLine("5. Exercitiul 5");
    Console.WriteLine("6. Iesire");

    int optiune = Convert.ToInt32(Console.ReadLine());
    switch (optiune){
        case 1:
            Exercitiul1.Rezolvare();
            break;
        case 2:
            Exercitiul2.Rezolvare();
            break;
        case 3:
            Exercitiul3.Rezolvare();
            break;
        case 4:
            Exercitiul4.Rezolvare();
            break;
        case 5:
            Exercitiul5.Rezolvare();
            break;
        case 6:
            ruleaza = false;
            Console.WriteLine("La revedere!");
            break;
        default:
            Console.WriteLine("Opțiune invalidă!");
            break;
    }
}