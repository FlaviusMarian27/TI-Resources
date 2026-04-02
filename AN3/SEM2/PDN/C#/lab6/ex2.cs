/*
Implementați o aplicație care citește informații despre un angajat (nume, vârstă, 
salariu) din fișiere separate. Gestionați excepțiile pentru fișiere lipsă, formate 
nevalide și valori incorecte ale datelor.
*/

using System;
using System.IO;

public static class Exercitiul2
{
    public static void Rezolvare()
    {
        try
        {
            string nume = File.ReadAllText("nume.txt").Trim();

            string varstaText = File.ReadAllText("varsta.txt").Trim();
            int varsta = Convert.ToInt32(varstaText);

            string salariuText = File.ReadAllText("salariu.txt").Trim();
            double salariu = Convert.ToDouble(salariuText);

            if(varsta < 18 || varsta > 100)
            {
                throw new Exception($"Varsta {varsta} ani este invalida");
            }

            if(salariu < 0)
            {
                throw new Exception("Nu putem sa avem salariu negativ");
            }

            Console.WriteLine("Datele validate\n");
            Console.WriteLine($"Nume: {nume}");
            Console.WriteLine($"Varsta: {varsta} ani");
            Console.WriteLine($"Salatiu: {salariu} lei");
        }
        catch (FileNotFoundException)
        {
            Console.WriteLine("Fisierul nu este gasit!");
        }
        catch (FormatException)
        {
            Console.WriteLine("Format nevalid!");
        }
        catch(Exception)
        {
            Console.WriteLine("Date incorecte!");
        }
        finally
        {
            Console.WriteLine("Incheiat cu succes!\n");
        }
    }
}