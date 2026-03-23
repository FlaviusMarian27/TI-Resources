/*
Creați o aplicație care gestionează o listă de angajați. Fiecare angajat are un 
nume, o funcție și un salariu. Utilizați LINQ pentru a efectua operații precum 
filtrarea  angajaților  după  funcție,  calcularea  salariului  mediu  și  găsirea 
angajatului cu cel mai mare salariu.
*/

using System;
using System.Collections.Generic;
using System.Linq;

public class Angajat{
    public string Nume;
    public string Functie;
    public double Salariu;

    public Angajat(string nume, string functie, double salariu){
        Nume = nume;
        Functie = functie;
        Salariu = salariu;
    }
}

public static class Exercitiul1{
    public static void Rezolvare(){
        List<Angajat> echipa = new List<Angajat>
        {
            new Angajat("Flavius", "Programator", 8500),
            new Angajat("Ana", "Manager", 12000),
            new Angajat("Andrei", "Programator", 7000),
            new Angajat("Maria", "HR", 5500)
        };

        Console.WriteLine("=== Baza de date Angajati ===");

        //A filtram dupa o functie
        var programatori = echipa.Where(a => a.Functie == "Programator").ToList();
        Console.WriteLine("\n--- Programatorii din echipa ---");

        foreach(var p in programatori){
            Console.WriteLine($"{p.Nume} - {p.Salariu} RON");
        }

        //B 
        double salariu = echipa.Average(a => a.Salariu);
        Console.WriteLine($"\n--- Salariul mediu in companie ---");
        Console.WriteLine($"{salariu:F2} RON");

        //C
        var max_sal = echipa.OrderByDescending(a => a.Salariu).First();
        Console.WriteLine("\n--- Cel mai bine platit angajat ---");
        Console.WriteLine($"{max_sal.Nume} ({max_sal.Functie}) cu un salariu de {max_sal.Salariu} RON");
    }
}