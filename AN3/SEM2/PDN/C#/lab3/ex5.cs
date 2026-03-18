/*
Creați o metodă care primește un tablou de întregi și un număr țintă și returnează
indicii a două elemente din tablou a căror sumă este egală cu ținta.
*/

using System;
public static class Exercitiul5{
    public static (int index1, int index2) FindIndexSum(int[] table, int target){
        for(int i = 0; i < table.Length - 1; i = i + 1){
            for(int j = i + 1; j < table.Length; j = j + 1){
                if(table[i] + table[j] == target){
                    return (i,j);
                }
            }
        }

        return (-1,-1);
    }

    public static void Rezolvare(){
        int[] table = { 2, 7, 11, 15 };
        int target = 9;

        Console.WriteLine($"\nTabloul este: [2, 7, 11, 15]");
        Console.WriteLine($"Suma tinta pe care o cautam este: {target}");

        var result = FindIndexSum(table,target);
        if (result.index1 != -1){
            Console.WriteLine($"\nSUCCES! Indicii sunt: {result.index1} si {result.index2}");
            Console.WriteLine($"Verificare: tablou[{result.index1}] + tablou[{result.index2}] -> {table[result.index1]} + {table[result.index2]} = {target}");
        }else{
            Console.WriteLine("\nNu s-a gasit nicio pereche care sa formeze suma tinta.");
        }
    }
}