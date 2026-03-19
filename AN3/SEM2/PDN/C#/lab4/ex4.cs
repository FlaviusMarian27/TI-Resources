/*
 Implementați o clasă Point care reprezintă un punct în sistemul de coordonate
2D. Adăugați metode pentru a calcula distanța dintre două puncte și pentru a
determina poziția unui punct față de un alt punct (stânga, dreapta, sus, jos).
*/

using System;

public class Point{
    public double X;
    public double Y;

    public Point(double x, double y){
        X = x;
        Y = y;
    }

    public double CalculeazaDistanta(Point altPunct){
        double deltaX = X - altPunct.X;
        double deltaY = Y - altPunct.Y;

        return Math.Sqrt(Math.Pow(deltaX, 2) + Math.Pow(deltaY, 2));
    }

    public void DeterminaPozitia(Point altPunct){
        Console.Write($"Fata de punctul ({altPunct.X}, {altPunct.Y}), punctul curent ({X}, {Y}) se afla: ");

        if (X < altPunct.X){
            Console.Write("la STANGA");
        }else if (X > altPunct.X){
            Console.Write("la DREAPTA");
        }else{
            Console.Write("la ACEEASI pozitie orizontala");   
        }

        Console.Write(" si ");

        if (Y < altPunct.Y){
            Console.WriteLine("mai JOS.");
        }else if (Y > altPunct.Y){
            Console.WriteLine("mai SUS.");
        }else{
            Console.WriteLine("la ACEEASI pozitie verticala.");
        }
    }
}