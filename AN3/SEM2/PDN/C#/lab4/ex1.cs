/*
Creați o clasă Student cu proprietăți precum Name, Grade și StudentId.
Adăugați metode pentru a calcula media notelor și pentru a afișa detaliile
studentului.
*/

using System;

public class Student{
    public string Name;
    public int[] Grade;
    public int StudentID;

    public Student(string name, int[] grade, int id){
        Name = name;
        Grade = grade;
        StudentID = id;
    }

    public double CalculeazaMedia(){
        if(Grade == null || Grade.Length == 0){
            return 0;
        }

        int suma = 0;
        for(int i = 0; i < Grade.Length; i = i + 1){
            suma = suma + Grade[i];
        }

        return suma / Grade.Length;
    }

    public void AfiseazaDetalii(){
        Console.WriteLine($"Nume: {Name}");
        Console.WriteLine($"ID: {StudentID}");
        
        Console.Write($"Grades: ");
        for(int i = 0; i < Grade.Length; i = i + 1){
            Console.WriteLine(Grade[i] + " ");
        }

        Console.WriteLine($"Media: {CalculeazaMedia():F2}");
    }
}