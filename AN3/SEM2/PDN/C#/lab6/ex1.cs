/*
1. Creați o aplicație care efectuează operații  aritmetice pe baza intrărilor de  la 
utilizator. Gestionați excepțiile pentru intrări nevalide, cum ar fi împărțirea la zero 
și depășirea limitelor de valori întregi. 
*/

using System;

public static class Exercitiul1{
    public static void Rezolvare(){

        try{
            Console.Write("Numarul 1: ");
            int number1 = Convert.ToInt32(Console.ReadLine());

            Console.Write("Numarul 2: ");
            int number2 = Convert.ToInt32(Console.ReadLine());

            Console.Write("Alege operatia(+,-,*,/): ");
            string operand = Console.ReadLine();

            int rezultat = 0;

            checked{
                if(operand == "+"){
                    rezultat = number1 + number2;
                }else if(operand == "-"){
                    rezultat = number1 - number2;
                }else if(operand == "*"){
                    rezultat = number1 * number2;
                }else if(operand == "/"){
                    rezultat = number1 / number2;
                }else{
                    Console.WriteLine("Operatie gresita!");
                    return;   
                }
            }

            Console.WriteLine($"Rezultatul operatie este {rezultat}");
        }catch (FormatException){
            Console.WriteLine("Eroare: Ai introdus caractere invalide!");
        }catch (DivideByZeroException){
            Console.WriteLine("Eroare: Impartirea la 0");   
        }catch(OverflowException){
            Console.WriteLine("Eroare: Numarul introdus sau rezultatul este prea mare");
        }catch(Exception ex){
            Console.WriteLine($"Eroare necunoscuta: {ex.Message}");
        }finally{
            Console.WriteLine("Program incheiat!");   
        }
    }
}