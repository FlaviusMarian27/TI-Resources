namespace FSharpLib2

type Student = {
    Name: string
    Grade: int
    Courses: string list
}

module StudentUtils =
    let GetHonorStudents (students: Student list) =
        students |> List.filter (fun s -> s.Grade > 80)