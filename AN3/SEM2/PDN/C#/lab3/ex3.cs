/*
Implementați o metodă care primește un tablou de întregi și returnează valoarea
minimă și valoarea maximă din tablou.
*/
using System;
public static class Exercitiul3{
    public static (int minim, int maxim) FindMinMax(int[] table){
        int minim = table[0];
        int maxim = table[0];

        for(int i = 1; i < table.Length; i = i + 1){
            if(table[i] > maxim){
                maxim = table[i];
            }else if(table[i] < minim){
                minim = table[i];
            }
        }

        return (minim,maxim);
    }

    public static void Rezolvare(){
        int[] numere = { 15, 42, 7, 89, 23, -4, 12 };
        
        Console.WriteLine("\nTabloul este: 15, 42, 7, 89, 23, -4, 12");

        var rezultat = FindMinMax(numere);

        Console.WriteLine($"Valoarea minima din tablou este: {rezultat.minim}");
        Console.WriteLine($"Valoarea maxima din tablou este: {rezultat.maxim}");
    }
}