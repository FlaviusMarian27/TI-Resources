/*
Creați o aplicație care gestionează o bibliotecă de cărți. Fiecare carte are un titlu, 
un autor și un gen literar. Utilizați LINQ pentru a efectua operații precum filtrarea 
cărților după gen, sortarea cărților după autor și calcularea numărului de cărți 
scrise de fiecare autor.
*/

using System;
using System.Collections.Generic;
using System.Linq;

public class Carte{
    public string Titlu;
    public string Autor;
    public string GenLiterar;

    public Carte(string titlu, string autor, string gen){
        Titlu = titlu;
        Autor = autor;
        GenLiterar = gen;
    }
}

public static class Exercitiul3{
    public static void Rezolvare(){
        List<Carte> biblioteca = new List<Carte>
        {
            new Carte("Ion", "Liviu Rebreanu", "Roman"),
            new Carte("Pădurea Spânzuraților", "Liviu Rebreanu", "Roman"),
            new Carte("O scrisoare pierdută", "I. L. Caragiale", "Comedie"),
            new Carte("Enigma Otiliei", "George Călinescu", "Roman"),
            new Carte("Luceafărul", "Mihai Eminescu", "Poezie"),
            new Carte("Scrisoarea I", "Mihai Eminescu", "Poezie"),
            new Carte("D-ale carnavalului", "I. L. Caragiale", "Comedie")  
        };

        Console.WriteLine("=== Gestiune Biblioteca ===");

        var roman = biblioteca.Where(c => c.GenLiterar == "Roman").ToList();
        Console.WriteLine("\n--- Carti din genul 'Roman' ---");
        foreach(var carte in roman)
        {
            Console.WriteLine($"- {carte.Titlu} (de {carte.Autor})");
        }

        var cartiSortate = biblioteca.OrderBy(c => c.Autor).ToList();
        Console.WriteLine("\n--- Toate cartile ordonate dupa Autor ---");
        foreach (var carte in cartiSortate)
        {
            Console.WriteLine($"{carte.Autor}: {carte.Titlu}");
        }

        var cartiPeAutor = biblioteca.GroupBy(c => c.Autor);
        foreach (var grup in cartiPeAutor)
        {
            Console.WriteLine($"{grup.Key} a scris {grup.Count()} carti.");
        }
    }
}