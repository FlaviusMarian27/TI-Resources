using System;

int[] noteFlavius = { 9, 8, 10 };
Student student1 = new Student("Flavius", noteFlavius, 101);

student1.AfiseazaDetalii();

Student student2 = new Student("Ana", new int[] { 7, 9, 8}, 102);
student2.AfiseazaDetalii();

Console.WriteLine("------Exercitiul 2---------");

Book carte1 = new Book("Ion", "Liviu Rebreanu", "978-973-104-515-4");
        
carte1.AfiseazaDetalii();

Book carte2 = new Book("Clean Code", "Robert C. Martin", "978-0132350884");
carte2.AfiseazaDetalii();

Console.WriteLine("------Exercitiul 3---------");

Employee angajat = new Employee("Marius", "Inginer Software", 5000);

angajat.AfiseazaDetalii();

angajat.MaresteSalariul(10);

angajat.AfiseazaDetalii();

Console.WriteLine("------Exercitiul 3---------");

Point p1 = new Point(0, 0);

Point p2 = new Point(3, 4);

double distanta = p1.CalculeazaDistanta(p2);
Console.WriteLine($"Distanța dintre puncte este: {distanta}");

p1.DeterminaPozitia(p2);

p2.DeterminaPozitia(p1);