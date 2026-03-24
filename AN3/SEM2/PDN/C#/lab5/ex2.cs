/*
Implementați o aplicație care gestionează un inventar de produse. Fiecare produs 
are un nume, o categorie și un preț. Utilizați LINQ pentru a efectua operații precum 
gruparea produselor după categorie, calcularea prețului mediu pentru fiecare 
categorie  și  găsirea  celui  mai  scump  și  celui  mai  ieftin  produs  din  fiecare 
categorie. 
*/

using System;
using System.Collections.Generic;
using System.Linq;

public class Produs{
    public string Nume;
    public string Categorie;
    public double Pret;

    public Produs(string nume, string categorie, double pret){
        Nume = nume;
        Categorie = categorie;
        Pret = pret;
    }
}

public static class Exercitiul2{
    public static void Rezolvare(){
        List<Produs> inventar = new List<Produs>
        {
            new Produs("Laptop ASUS", "Electronice", 4500),
            new Produs("Telefon Samsung", "Electronice", 3200),
            new Produs("Căști Sony", "Electronice", 800),
            new Produs("Birou", "Mobilier", 1200),
            new Produs("Scaun ergonomic", "Mobilier", 600),
            new Produs("Tastatură mecanică", "Electronice", 450),
            new Produs("Dulap", "Mobilier", 1500)    
        };

        Console.WriteLine("=== Analiza Inventar Produse ===");

        var produseGrupate = inventar.GroupBy(p => p.Categorie);
        foreach(var grup in produseGrupate){
            Console.WriteLine($"\n--- Categoria: {grup.Key} ---");

            double pretMediu = grup.Average(p => p.Pret);
            Console.WriteLine($"Pret mediu: {pretMediu:F2} RON");

            var celMaiScump = grup.OrderByDescending(p => p.Pret).First();
            Console.WriteLine($"Cel mai scump: {celMaiScump.Nume} ({celMaiScump.Pret} RON)");

            var celMaiIeftin = grup.OrderBy(p => p.Pret).First();
            Console.WriteLine($"Cel mai ieftin: {celMaiIeftin.Nume} ({celMaiIeftin.Pret} RON)");
        }
    }
}