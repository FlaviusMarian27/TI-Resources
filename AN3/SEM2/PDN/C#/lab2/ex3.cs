/*
Scrieți un program care generează tabla înmulțirii pentru un 
număr dat de la tastatură.
*/
using System;

public static class Exercitiul3{
    public static void Rezolvare(){
        Console.Write("Alege un numar pentru a genera tabla inmultirii: ");
        int number = Convert.ToInt32(Console.ReadLine());

        for(int i = 0; i <= 10; i = i + 1){
            int result = number * i;
            Console.WriteLine($"{number} x {i} = {result}");
        }
    }
}