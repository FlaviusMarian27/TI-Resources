/*
Folosind un fișier CSV care conține date despre studenți (nume, notă1, notă2, 
notă3), utilizați LINQ pentru a calcula media notelor pentru fiecare student și 
afișați numele și media obținută.
*/

using System;
using System.Text.Json.Serialization;

public class Student
{
    public string nume = "";
    public int nota1 = 0;
    public int nota2 = 0;
    public int nota3 = 0;
}

public class Exercitiul3
{
    public static void Rezolvare()
    {
        Console.WriteLine("========Lista Studenti========");
        
        string[] linii = File.ReadAllLines("studenti.csv");
        List<Student> listaStudenti = new List<Student>();
        for(int i = 1; i < linii.Length; i++)
        {
            string[] coloane = linii[i].Split(',');

            Student s = new Student();
            s.nume = coloane[0];
            s.nota1 = int.Parse(coloane[1]);
            s.nota2 = int.Parse(coloane[2]);
            s.nota3 = int.Parse(coloane[3]);

            listaStudenti.Add(s);
        }

        var result = listaStudenti.Select(s => new
        {
            NumeStudent = s.nume,
            Media = (s.nota1 + s.nota2 + s.nota3)/3.0   
        });

        foreach(var r in result)
        {
            Console.WriteLine($"Student: {r.NumeStudent} | Media: {r.Media:F2}");
        }
    }
}