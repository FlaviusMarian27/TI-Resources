using System;
using System.Diagnostics;
using System.Data.Common;
using System.Xml.Linq;

// =============================================================
// LABORATORUL 1
// =============================================================

/*
    2) Creați un program C# care afișează versiunea curentă a .NET SDK 
    folosită.(Indiciu: utilizați comanda dotnet --version în terminal).
*/
Console.WriteLine("====================LABORATORUL 1====================\n");

Console.WriteLine("PROBLEMA 2:");
Console.Write("Versiune dotnet: ");
Process.Start("dotnet", "--version").WaitForExit();

/*
    3) Scrieți un program C# care cere utilizatorului să introducă raza unui cerc și
    calculează și afișează aria și circumferința cercului, folosind valorile introduse.
    (Considerați π = 3.1415926)
*/
Console.WriteLine("\nPROBLEMA 3:");
Console.Write("Alege raza unui cerc: ");
double raza = Convert.ToDouble(Console.ReadLine());
double pi = 3.1415926;
double aria = pi * raza * raza;
double circumferinta = 2 * pi * raza;

Console.WriteLine($"Aria cercului este: {aria}");
Console.WriteLine($"Circumferinaa cercului este: {circumferinta}");

// =============================================================
// LABORATORUL 2
// =============================================================

Console.WriteLine("====================LABORATORUL 2====================\n");
/*
    4)Creați un program care calculează suma cifrelor unui număr 
    introdus de utilizator.
*/
Console.WriteLine("PROBLEMA 4:");
Console.Write("Alege un numar pentru suma cifrelor: ");

int number = Convert.ToInt32(Console.ReadLine());
int suma = 0;
int x = number;

while(x != 0){
    suma = suma + x % 10;
    x = x / 10;
}   

Console.WriteLine($"Suma cifrelor lui {number} este {suma}");

/*
5) Scrieți un program care afișează toate numerele prime mai mici decât 
un număr dat de la tastatură.
*/

Console.WriteLine("\nPROBLEMA 5:");
Ex5Lab2.Rezolvare();

// =============================================================
// LABORATORUL 3
// =============================================================

Console.WriteLine("====================LABORATORUL 3====================\n");

/*
1) Scrieți o metodă care primește un șir de caractere și returnează 
numărul de vocale din șir.
*/
Console.WriteLine("PROBLEMA 1:");
Ex1Lab3.Rezolvare();

/*
Creați o metodă care primește un număr întreg și returnează inversul 
său (ex: 123 -> 321).
*/
Console.WriteLine("\nPROBLEMA 2:");
Ex2Lab3.Rezolvare();

// =============================================================
// LABORATORUL 4
// =============================================================

Console.WriteLine("====================LABORATORUL 4====================\n");
/*
    1)Creați o clasă Student cu proprietăți precum Name, Grade și StudentId.
Adăugați metode pentru a calcula media notelor și pentru a afișa detaliile
studentului.
*/
Console.WriteLine("PROBLEMA 1:");
int[] noteFlavius = { 9, 8, 10 };
Student student1 = new Student("Flavius", noteFlavius, 101);

student1.AfiseazaDetalii();

Student student2 = new Student("Ana", new int[] { 7, 9, 8}, 102);
student2.AfiseazaDetalii();

/*
    2)Implementați o clasă Book cu proprietăți precum Title, Author și ISBN.
Adăugați o metodă pentru a afișa informațiile despre carte într-un format specific.
*/
Console.Write("\nPROBLEMA 2:");
Book carte1 = new Book("Ion", "Liviu Rebreanu", "978-973-104-515-4");
        
carte1.AfiseazaDetalii();

Book carte2 = new Book("Clean Code", "Robert C. Martin", "978-0132350884");
carte2.AfiseazaDetalii();

// =============================================================
// LABORATORUL 5
// =============================================================
Console.WriteLine("====================LABORATORUL 5====================\n");
/*
    1)Creați o aplicație care gestionează o listă de angajați. Fiecare angajat are un 
nume, o funcție și un salariu. Utilizați LINQ pentru a efectua operații precum 
filtrarea  angajaților  după  funcție,  calcularea  salariului  mediu  și  găsirea 
angajatului cu cel mai mare salariu.
*/
Console.WriteLine("PROBLEMA 1:");
Ex1Lab5.Rezolvare();

/*
    2) Implementați o aplicație care gestionează un inventar de produse. Fiecare produs 
are un nume, o categorie și un preț. Utilizați LINQ pentru a efectua operații precum 
gruparea produselor după categorie, calcularea prețului mediu pentru fiecare 
categorie  și  găsirea  celui  mai  scump  și  celui  mai  ieftin  produs  din  fiecare 
categorie. 
*/
Console.WriteLine("\nPROBLEMA 2:");
Ex2Lab5.Rezolvare();

// =============================================================
// LABORATORUL 6
// =============================================================
Console.WriteLine("====================LABORATORUL 6====================\n");

/*
    1) Creați o aplicație care efectuează operații  aritmetice pe baza intrărilor de  la 
utilizator. Gestionați excepțiile pentru intrări nevalide, cum ar fi împărțirea la zero 
și depășirea limitelor de valori întregi. 
*/
Console.WriteLine("PROBLEMA 1:");
Ex1Lab6.Rezolvare();

/*
    2) Implementați o aplicație care citește informații despre un angajat (nume, vârstă, 
salariu) din fișiere separate. Gestionați excepțiile pentru fișiere lipsă, formate 
nevalide și valori incorecte ale datelor.
*/
Console.WriteLine("\nPROBLEMA 2:");
Ex2Lab6.Rezolvare();

// =============================================================
// LABORATORUL 7
// =============================================================
Console.WriteLine("====================LABORATORUL 7====================\n");

/*
    1) Creați un fișier CSV care conține informații despre produse (nume, categorie, preț) 
și  utilizați  LINQ  pentru  a  calcula  prețul  mediu  al  produselor  pentru  fiecare 
categorie. 
*/
Console.WriteLine("PROBLEMA 1:");
Ex1Lab7.Rezolvare();

/*
    2) Având  un  fișier  XML  care  conține  date  despre  angajați  (nume,  departament, 
salariu), utilizați LINQ to XML pentru a afișa numele și salariul angajaților din 
departamentul "IT".
*/
Console.WriteLine("\nPROBLEMA 2:");
Ex2Lab7.Rezolvare();

// =============================================================
// METODELE SI CLASELE NECESARE
// =============================================================

public static class Ex5Lab2{
    public static bool isPrime(int number){
        if(number == 0 || number == 1){
            return false;
        }

        for(int i = 2; i <= number / 2; i = i + 1){
            if(number % i == 0){
                return false;
            }
        }

        return true;
    }
    public static void Rezolvare(){
        Console.Write("Alege un numar(pentru a determina cate prime avem): ");
        int number = Convert.ToInt32(Console.ReadLine());

        for(int i = number - 1; i >= 0 ; i = i - 1){
            if(isPrime(i) == true){
                Console.Write(i + " ");
            }
        }
        
        Console.WriteLine("");
    }
}

public static class Ex1Lab3{
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
        Console.Write("Introduceti un text(numar vocale): ");
        string input_str = Console.ReadLine()!;

        int number = VocalCount(input_str);

        Console.WriteLine($"Sirul introdus contine {number} vocale.");
    }
}

public static class Ex2Lab3{
    public static int BuildTheInvers(int number){
        int inv = 0;
        int temp = number;
        while(temp != 0){
            inv = inv * 10 + temp % 10;
            temp = temp / 10;
        }

        return inv;
    }

    public static void Rezolvare(){
        Console.Write("Alege un numar(invers): ");
        int number = Convert.ToInt32(Console.ReadLine());

        int result = BuildTheInvers(number);

        Console.WriteLine($"Inversul lui {number} este {result}");
    }
}

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
            Console.Write(Grade[i] + " ");
        }

        Console.WriteLine("");
        Console.WriteLine($"Media: {CalculeazaMedia():F2}");
    }
}

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

public class Angajat{
    public string Nume;
    public string Functie;
    public double Salariu;

    public Angajat(string nume, string functie, double salariu){
        Nume = nume;
        Functie = functie;
        Salariu = salariu;
    }
}

public static class Ex1Lab5{
    public static void Rezolvare(){
        List<Angajat> echipa = new List<Angajat>
        {
            new Angajat("Flavius", "Programator", 8500),
            new Angajat("Ana", "Manager", 12000),
            new Angajat("Andrei", "Programator", 7000),
            new Angajat("Maria", "HR", 5500)
        };

        Console.WriteLine("=== Baza de date Angajati ===");

        //A filtram dupa o functie
        var programatori = echipa.Where(a => a.Functie == "Programator").ToList();
        Console.WriteLine("\n--- Programatorii din echipa ---");

        foreach(var p in programatori){
            Console.WriteLine($"{p.Nume} - {p.Salariu} RON");
        }

        //B 
        double salariu = echipa.Average(a => a.Salariu);
        Console.WriteLine($"\n--- Salariul mediu in companie ---");
        Console.WriteLine($"{salariu:F2} RON");

        //C
        var max_sal = echipa.OrderByDescending(a => a.Salariu).First();
        Console.WriteLine("\n--- Cel mai bine platit angajat ---");
        Console.WriteLine($"{max_sal.Nume} ({max_sal.Functie}) cu un salariu de {max_sal.Salariu} RON");
    }
}

public class Produs{
    public string Nume;
    public string Categorie;
    public double Pret;

    public Produs(string nume, string categorie, double pret){
        Nume = nume;
        Categorie = categorie;
        Pret = pret;
    }
}

public static class Ex2Lab5{
    public static void Rezolvare(){
        List<Produs> inventar = new List<Produs>
        {
            new Produs("Laptop ASUS", "Electronice", 4500),
            new Produs("Telefon Samsung", "Electronice", 3200),
            new Produs("Căști Sony", "Electronice", 800),
            new Produs("Birou", "Mobilier", 1200),
            new Produs("Scaun ergonomic", "Mobilier", 600),
            new Produs("Tastatură mecanică", "Electronice", 450),
            new Produs("Dulap", "Mobilier", 1500)    
        };

        Console.WriteLine("=== Analiza Inventar Produse ===");

        var produseGrupate = inventar.GroupBy(p => p.Categorie);
        foreach(var grup in produseGrupate){
            Console.WriteLine($"\n--- Categoria: {grup.Key} ---");

            double pretMediu = grup.Average(p => p.Pret);
            Console.WriteLine($"Pret mediu: {pretMediu:F2} RON");

            var celMaiScump = grup.OrderByDescending(p => p.Pret).First();
            Console.WriteLine($"Cel mai scump: {celMaiScump.Nume} ({celMaiScump.Pret} RON)");

            var celMaiIeftin = grup.OrderBy(p => p.Pret).First();
            Console.WriteLine($"Cel mai ieftin: {celMaiIeftin.Nume} ({celMaiIeftin.Pret} RON)");
        }
    }
}

public static class Ex1Lab6{
    public static void Rezolvare(){

        try{
            Console.Write("Numarul 1: ");
            int number1 = Convert.ToInt32(Console.ReadLine());

            Console.Write("Numarul 2: ");
            int number2 = Convert.ToInt32(Console.ReadLine());

            Console.Write("Alege operatia(+,-,*,/): ");
            string operand = Console.ReadLine()!;

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

public static class Ex2Lab6
{
    public static void Rezolvare()
    {
        try
        {
            string nume = File.ReadAllText("nume.txt").Trim();

            string varstaText = File.ReadAllText("varsta.txt").Trim();
            int varsta = Convert.ToInt32(varstaText);

            string salariuText = File.ReadAllText("salariu.txt").Trim();
            double salariu = Convert.ToDouble(salariuText);

            if(varsta < 18 || varsta > 100)
            {
                throw new Exception($"Varsta {varsta} ani este invalida");
            }

            if(salariu < 0)
            {
                throw new Exception("Nu putem sa avem salariu negativ");
            }

            Console.WriteLine("Datele validate\n");
            Console.WriteLine($"Nume: {nume}");
            Console.WriteLine($"Varsta: {varsta} ani");
            Console.WriteLine($"Salatiu: {salariu} lei");
        }
        catch (FileNotFoundException)
        {
            Console.WriteLine("Fisierul nu este gasit!");
        }
        catch (FormatException)
        {
            Console.WriteLine("Format nevalid!");
        }
        catch(Exception)
        {
            Console.WriteLine("Date incorecte!");
        }
        finally
        {
            Console.WriteLine("Incheiat cu succes!\n");
        }
    }
}

public class ProdusLab7
{
    public string nume = "";
    public string categorie = "";
    public double pret = 0;
}

public class Ex1Lab7
{
    public static void Rezolvare()
    {
        Console.WriteLine("============Pretul mediu============");

        string[] linii = File.ReadAllLines("produse.csv");
        List<ProdusLab7> listaProduse = new List<ProdusLab7>();

        for(int i = 1; i < linii.Length; i++)
        {
            string[] coloana = linii[i].Split(',');
            ProdusLab7 p = new ProdusLab7();
            p.nume = coloana[0];
            p.categorie = coloana[1];
            p.pret = double.Parse(coloana[2]);

            listaProduse.Add(p);
        }

        var produseGrupate = listaProduse.GroupBy(p => p.categorie);

        foreach(var grup in produseGrupate)
        {
            double medie = grup.Average(p => p.pret);
            Console.WriteLine($"Categoria: {grup.Key} -> Pret Mediu: {medie:F2} RON");
        }
    }
}

public class AngajatLab7
{
    public string nume = "";
    public string departament = "";
    public double salariu = 0;
}

public class Ex2Lab7
{
    public static void Rezolvare()
    {
        Console.WriteLine("========Angajati in IT========");

        XDocument doc = XDocument.Load("angajati.xml");
        List<AngajatLab7> listaAngajati = new List<AngajatLab7>();

        foreach(XElement element in doc.Descendants("Angajat"))
        {
            AngajatLab7 a = new AngajatLab7();

            a.nume = element.Element("Nume")!.Value;
            a.departament = element.Element("Departament")!.Value;
            a.salariu = double.Parse(element.Element("Salariu")!.Value);

            listaAngajati.Add(a);
        }

        var angajatiIT = listaAngajati.Where(a => a.departament == "IT");
        foreach(var angajat in angajatiIT)
        {
            Console.WriteLine($"- Nume: {angajat.nume} | Salariu: {angajat.salariu} RON");
        }
    }
}
