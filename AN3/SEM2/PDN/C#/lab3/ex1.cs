/*Scrieți o metodă care primește un șir de caractere și returnează 
numărul de vocale din șir.*/

using System;
using System.Diagnostics.Metrics;
public static class Exercitiul1{
    public static int VocalCount(string str){
        int counter_vocal = 0;
        for(int i = 0; i < str.Length; i = i + 1){
            if(str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || 
            str[i] == 'o' || str[i] == 'u' || str[i] == 'A' ||
            str[i] == 'E' || str[i] == 'I' || str[i] == 'O' ||
            str[i] == 'U'){
                counter_vocal = counter_vocal + 1;
            }
        }

        return counter_vocal;
    }

    public static void Rezolvare(){
        Console.Write("Introduceti un text: ");
        string input_str = Console.ReadLine()!;

        int number = VocalCount(input_str);

        Console.WriteLine($"Sirul introdus contine {number} vocale.");
    }
}