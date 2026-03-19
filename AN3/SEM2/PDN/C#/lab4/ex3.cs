/*
Creați o clasă Employee cu proprietăți precum Name, Position și Salary.
Adăugați metode pentru a calcula salariul anual și pentru a mări salariul cu un
anumit procent.
*/

using System;

public class Employee{
    public string Name;
    public string Position;
    public double Salary;

    public Employee(string name, string position, double salary){
        Name = name;
        Position = position;
        Salary = salary;
    }

    public double CalculeazaSalariulAnual(){
        return Salary * 12;
    }

    public void MaresteSalariul(double procent){
        double marire = Salary * (procent / 100);

        Salary = Salary + marire;

        Console.WriteLine($"\n[INFO] Salariul lui {Name} a fost marit cu {procent}%!");
    }

    public void AfiseazaDetalii(){
        Console.WriteLine("\n--- Fisa Angajat ---");
        Console.WriteLine($"Nume: {Name}");
        Console.WriteLine($"Functie: {Position}");
        Console.WriteLine($"Salariu lunar curent: {Salary} RON");
        Console.WriteLine($"Salariu anual proiectat: {CalculeazaSalariulAnual()} RON");
        Console.WriteLine("--------------------");
    }
}