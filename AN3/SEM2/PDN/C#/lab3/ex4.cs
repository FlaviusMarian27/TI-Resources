/*
Scrieți o metodă care primește un număr întreg 
și returnează suma divizorilor săi.
*/
using System;
public static class Exercitiul4{
    public static int DivSum(int number){
        int sum = 0;

        for(int i = 1; i <= number; i = i + 1){
            if(number % i == 0){
                sum = sum + i;
            }
        }

        return sum;
    }

    public static void Rezolvare(){
        Console.Write("Alege un numar");
        int number = Convert.ToInt32(Console.ReadLine());

        int result = DivSum(number);

        Console.WriteLine($"Suma divizori lui {number} este {result}");
    }
}