/*
Implementați o clasă Book cu proprietăți precum Title, Author și ISBN.
Adăugați o metodă pentru a afișa informațiile despre carte într-un format specific.
*/

using System;

public class Book{
    public string Title;
    public string Author;
    public string ISBN;

    public Book(string title, string author, string isbn){
        Title = title;
        Author = author;
        ISBN = isbn;
    }

    public void AfiseazaDetalii()
    {
        Console.WriteLine("\n-----------------------------------------");
        Console.WriteLine($"Titlu:  {Title}");
        Console.WriteLine($"Autor:  {Author}");
        Console.WriteLine($"ISBN:   {ISBN}");
        Console.WriteLine("-----------------------------------------");
    }
}