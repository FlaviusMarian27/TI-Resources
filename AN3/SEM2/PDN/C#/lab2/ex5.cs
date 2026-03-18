/*
Scrieți un program care afișează toate numerele prime mai mici decât un număr
dat de la tastatură.
*/

using System;

public static class Exercitiul5{
    public static bool isPrime(int number){
        if(number == 0 || number == 1){
            return false;
        }

        for(int i = 2; i <= number / 2; i = i + 1){
            if(number % i == 0){
                return false;
            }
        }

        return true;
    }
    public static void Rezolvare(){
        Console.Write("Alege un numar: ");
        int number = Convert.ToInt32(Console.ReadLine());

        for(int i = number - 1; i >= 0 ; i = i - 1){
            if(isPrime(i) == true){
                Console.Write(i + " ");
            }
        }
    }
}