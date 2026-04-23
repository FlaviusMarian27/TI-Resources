open System
open CSharpLib

[<EntryPoint>]
let main argv =
    printfn ""
    printfn "-------------Problema 1-------------"
    
    let textulMeu = "Salutari din C# si F#!"
    
    let textInversat = StringUtils.ReverseString(textulMeu)
    
    printfn "Textul original: %s" textulMeu
    printfn "Textul inversat: %s" textInversat
    
    0