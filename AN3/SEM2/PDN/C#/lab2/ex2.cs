/*Creați un program care verifică dacă un an introdus de 
utilizator este bisect.*/

using System;

public static class Exercitiul2{
    public static void Rezolvare(){
        Console.Write("Alege un an: ");
        int an = Convert.ToInt32(Console.ReadLine());

        if((an % 4 == 0 && an % 100 != 0) || an % 400 == 0){
            Console.WriteLine($"Anul {an} este bisect!");
        }else{
            Console.WriteLine($"Anul {an} NU este bisect!");   
        }
    } 
}