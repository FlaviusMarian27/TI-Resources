/*
Creați o aplicație care procesează un fișier CSV și extrage informații specifice. 
Gestionați excepțiile pentru fișiere corupte, formate incorecte și coloane lipsă.
*/

using System;
using System.IO;

public static class Exercitiul3
{
    public static void Rezolvare()
    {
        try
        {
            string[] linii = File.ReadAllLines("date.csv");
            Console.WriteLine("Datele extrase din csv:");
            for(int i = 0; i < linii.Length; i = i + 1)
            {
                string linie = linii[i];
                if (string.IsNullOrWhiteSpace(linie))
                {
                    continue;
                }    

                string[] coloane = linie.Split(',');
                if(coloane.Length < 3)
                {
                    throw new IndexOutOfRangeException($"Randul {i + 1} este incomplet! (Are doar {coloane.Length} coloane din 3).");
                }

                string nume = coloane[0].Trim();
                string departament = coloane[1].Trim();
                double salariu = double.Parse(coloane[2].Trim());
                Console.WriteLine($"- {nume} din {departament} are salariul {salariu} RON");
            }   
        }
        catch(FileNotFoundException)
        {
            Console.WriteLine("Fisier nu exista!");           
        }
        catch (IndexOutOfRangeException)
        {
            Console.WriteLine("Coloana nu exista!");
        }
        catch (FormatException)
        {
            Console.WriteLine("Format gresit!");
        }
        catch(IOException)
        {
            Console.WriteLine("Fisierul este corupt!");
        }
        catch(Exception ex)
        {
            Console.WriteLine($"Eroare necunoscuta {ex.Message}");
        }
        finally
        {
            Console.WriteLine("S-a finalizat!");
        }
    }
}