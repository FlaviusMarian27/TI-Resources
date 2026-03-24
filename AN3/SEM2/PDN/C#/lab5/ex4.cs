/*
Implementați  o  aplicație  care  gestionează  un  registru  de  cursuri  și  studenți. 
Fiecare curs are un nume și un profesor, iar fiecare student are un nume și o listă 
de cursuri la care participă. Utilizați LINQ pentru a efectua operații precum găsirea 
studenților înscriși la un anumit curs, calcularea numărului de cursuri pentru 
fiecare student și identificarea profesorilor cu cei mai mulți studenți.
*/

using System;
using System.Collections.Generic;
using System.Linq;

public class Curs
{
    public string Nume;
    public string Profesor;

    public Curs(string nume, string profesor){
        Nume = nume;
        Profesor = profesor;
    }
}

public class Student
{
    public string Nume;
    public List<Curs> Cursuri;
    public Student(string nume, List<Curs> cursuri){
        Nume = nume;
        Cursuri = cursuri;
    }
}

public static class Exercitiul4{
    public static void Rezolvare(){
        Curs cCsharp = new Curs("Programare C#", "Prof. Ionescu");
        Curs cBazeDate = new Curs("Baze de Date", "Prof. Popescu");
        Curs cWeb = new Curs("Tehnologii Web", "Prof. Ionescu");

        List<Student> registru = new List<Student>
        {
            new Student("Flavius", new List<Curs> { cCsharp, cBazeDate }),
            new Student("Ana", new List<Curs> { cBazeDate, cWeb }),
            new Student("Andrei", new List<Curs> { cCsharp }),
            new Student("Maria", new List<Curs> { cCsharp, cBazeDate, cWeb })
        };

        Console.WriteLine("=== Registru Universitar ===\n");
        string cursCautat = "Programare C#";
        var studentiLaCsharp = registru.Where(s => s.Cursuri.Any(c => c.Nume == cursCautat));

        Console.WriteLine($"--- Studentii inscrisi la '{cursCautat}' ---");
        foreach (var s in studentiLaCsharp)
        {
            Console.WriteLine($"- {s.Nume}");
        }

        Console.WriteLine("\n--- Numar de cursuri per student ---");
        foreach (var s in registru)
        {
            Console.WriteLine($"{s.Nume} participa la {s.Cursuri.Count} cursuri.");
        }

        var topProfesor = registru.SelectMany(s => s.Cursuri).GroupBy(c => c.Profesor).OrderByDescending(grup => grup.Count()).First();
        Console.WriteLine("\n--- Profesorul cu cei mai multi studenti ---");
        Console.WriteLine($"{topProfesor.Key} (are {topProfesor.Count()} inscrieri totale la cursurile sale)");
    }
}