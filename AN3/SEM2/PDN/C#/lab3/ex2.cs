/*
Creați o metodă care primește un număr întreg și returnează inversul 
său (ex: 123 -> 321).
*/
using System;
public static class Exercitiul2{
    public static int BuildTheInvers(int number){
        int inv = 0;
        int temp = number;
        while(temp != 0){
            inv = inv * 10 + temp % 10;
            temp = temp / 10;
        }

        return inv;
    }

    public static void Rezolvare(){
        Console.Write("Alege un numar: ");
        int number = Convert.ToInt32(Console.ReadLine());

        int result = BuildTheInvers(number);

        Console.WriteLine($"Inversul lui {number} este {result}");
    }
}