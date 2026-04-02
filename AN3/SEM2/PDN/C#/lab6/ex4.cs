/*
Implementați o aplicație care se conectează la o bază de date și efectuează 
operații  CRUD  (Create,  Read,  Update,  Delete).  Gestionați  excepțiile  pentru 
conexiuni eșuate, interogări invalide și erori de restricții de cheie străină. 
*/

using System;

public static class Exercitiul4
{
    public static void Rezolvare()
    {
        Console.WriteLine("Simulare Crud");
        Console.WriteLine("Alege operatia:");
        Console.WriteLine("1)Create");
        Console.WriteLine("2)Read");
        Console.WriteLine("3)Update");
        Console.WriteLine("4)Delete");
        Console.Write("Alege: ");

        string optiune = Console.ReadLine()!;
        try
        {
            if(optiune == "1")
            {
                Console.WriteLine("Datele au fost salvate.");
            }
            else if(optiune == "2")
            {
                throw new TimeoutException("Serverul nu raspunde");
            }
            else if(optiune == "3")
            {
                throw new FormatException("Sintaxa SQL incorecta");
            }
            else if(optiune == "4")
            {
                throw new InvalidOperationException("Inregistrarea este legata de alta tabela");
            }
            else
            {
                Console.WriteLine("Optiune invalida!");
            }
        }
        catch(TimeoutException ex)
        {
            Console.WriteLine($"[Conexiune Esuata] {ex.Message}");
        }
        catch (FormatException ex)
        {
            Console.WriteLine($"[Interogare Invalida] {ex.Message}");
        }
        catch(InvalidOperationException ex)
        {
            Console.WriteLine($"[Restrictie Cheie Straina] {ex.Message}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[Eroare Generala] {ex.Message}");
        }
    }
}