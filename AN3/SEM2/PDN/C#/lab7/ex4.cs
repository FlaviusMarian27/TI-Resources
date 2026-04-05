/*
Creați un fișier XML care descrie o bibliotecă de filme (titlu, gen, an) și utilizați 
LINQ to XML pentru a afișa titlurile filmelor din genul "Comedy" lansate după anul 
2010. 
*/

using System;
using System.Xml.Linq;

public class Film
{
    public string titlu = "";
    public string gen = "";
    public int an = 0;
}

public class Exercitiul4
{
    public static void Rezolvare()
    {
        Console.WriteLine("========Filme Comedie 2010========");

        XDocument doc = XDocument.Load("filme.xml");
        List<Film> listaFilme = new List<Film>();
        foreach(XElement element in doc.Descendants("Film"))
        {
            Film f = new Film();
            f.titlu = element.Element("Titlu")!.Value;
            f.gen = element.Element("Gen")!.Value;

            f.an = int.Parse(element.Element("An")!.Value);
            listaFilme.Add(f);
        }

        var comediiNoi = listaFilme.Where(f => f.gen == "Comedy" && f.an > 2010);
        foreach(var film in comediiNoi)
        {
            Console.WriteLine($"- {film.titlu} (Lansat in {film.an})");
        }
    }
}