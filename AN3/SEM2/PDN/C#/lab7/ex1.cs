/*
Creați un fișier CSV care conține informații despre produse (nume, categorie, preț) 
și  utilizați  LINQ  pentru  a  calcula  prețul  mediu  al  produselor  pentru  fiecare 
categorie. 
*/

using System;
using System.ComponentModel;

public class Produs
{
    public string nume = "";
    public string categorie = "";
    public double pret = 0;
}

public class Exercitiul1
{
    public static void Rezolvare()
    {
        Console.WriteLine("============Pretul mediu============");

        string[] linii = File.ReadAllLines("produse.csv");
        List<Produs> listaProduse = new List<Produs>();

        for(int i = 1; i < linii.Length; i++)
        {
            string[] coloana = linii[i].Split(',');
            Produs p = new Produs();
            p.nume = coloana[0];
            p.categorie = coloana[1];
            p.pret = double.Parse(coloana[2]);

            listaProduse.Add(p);
        }

        var produseGrupate = listaProduse.GroupBy(p => p.categorie);

        foreach(var grup in produseGrupate)
        {
            double medie = grup.Average(p => p.pret);
            Console.WriteLine($"Categoria: {grup.Key} -> Pret Mediu: {medie:F2} RON");
        }
    }
}