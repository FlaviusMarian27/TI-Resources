using System;
using Microsoft.FSharp.Collections;  
using FSharpLib2;                   

namespace CSharpApp2
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("\n-------------Problema 2-------------");

            var cursuriFlavius = ListModule.OfSeq(new[] { "Matematica", "Programare" });
            var cursuriAndrei = ListModule.OfSeq(new[] { "Fizica", "Istorie" });
            var cursuriMaria = ListModule.OfSeq(new[] { "Design", "Baze de date" });

            var s1 = new Student("Flavius", 95, cursuriFlavius);
            var s2 = new Student("Andrei", 75, cursuriAndrei);
            var s3 = new Student("Maria", 88, cursuriMaria);

            var listaStudenti = ListModule.OfSeq(new[] { s1, s2, s3 });

            var studentiDeTop = StudentUtils.GetHonorStudents(listaStudenti);

            Console.WriteLine("Studentii cu nota peste 80 sunt:");
            foreach (var student in studentiDeTop)
            {
                Console.WriteLine($"- {student.Name} (Nota: {student.Grade})");
            }
        }
    }
}