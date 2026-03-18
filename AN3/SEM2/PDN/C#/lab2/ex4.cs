/*Creați un program care calculează suma cifrelor unui număr 
introdus de utilizator.*/
using System;

public static class Exercitiul4{
    public static void Rezolvare(){
        Console.Write("Alege un numar: ");

        int number = Convert.ToInt32(Console.ReadLine());
        int suma = 0;
        int x = number;
        while(x != 0){
            suma = suma + x % 10;
            x = x / 10;
        }   

        Console.WriteLine($"Suma cifrelor lui {number} este {suma}");
    }
}