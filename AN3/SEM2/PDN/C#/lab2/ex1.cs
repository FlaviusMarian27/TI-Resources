/*
Scrieți un program care citește trei numere de la tastatură și 
afișează cel mai mare dintre ele.
*/

using System;
using System.ComponentModel.DataAnnotations;
public static class Exercitiul1{
    public static void Rezolvare(){
        Console.Write("Numarul 1: ");
        int n1 = Convert.ToInt32(Console.ReadLine());

        Console.Write("Numarul 2: ");
        int n2 = Convert.ToInt32(Console.ReadLine());

        Console.Write("Numarul 3: ");
        int n3 = Convert.ToInt32(Console.ReadLine());

        int maxim = Math.Max(n1,Math.Max(n2,n3));
        Console.WriteLine($"Cel mare numar dintre {n1}, {n2} si {n3} este {maxim}");
    }
}