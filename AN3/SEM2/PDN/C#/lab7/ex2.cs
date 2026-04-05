/*Având  un  fișier  XML  care  conține  date  despre  angajați  (nume,  departament, 
salariu), utilizați LINQ to XML pentru a afișa numele și salariul angajaților din 
departamentul "IT".*/

using System;
using System.Data.Common;
using System.Xml.Linq;

public class Angajat
{
    public string nume = "";
    public string departament = "";
    public double salariu = 0;
}

public class Exercitiul2
{
    public static void Rezolvare()
    {
        Console.WriteLine("========Angajati in IT========");

        XDocument doc = XDocument.Load("angajati.xml");
        List<Angajat> listaAngajati = new List<Angajat>();

        foreach(XElement element in doc.Descendants("Angajat"))
        {
            Angajat a = new Angajat();

            a.nume = element.Element("Nume")!.Value;
            a.departament = element.Element("Departament")!.Value;
            a.salariu = double.Parse(element.Element("Salariu")!.Value);

            listaAngajati.Add(a);
        }

        var angajatiIT = listaAngajati.Where(a => a.departament == "IT");
        foreach(var angajat in angajatiIT)
        {
            Console.WriteLine($"- Nume: {angajat.nume} | Salariu: {angajat.salariu} RON");
        }
    }
}

