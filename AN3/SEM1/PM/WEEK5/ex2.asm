dosseg
.model small ; prin această instrucțiune se selectează modelul de memorie folosit
.stack 100h ;declararea segmentului stivă
.data ; reprezintă secțiunea de declarare a datelor
 A DB 0
 B DB 0
 C DB 0
 X DW 7
 n1 DB "Introduceti primul numar:$"
 n2 DB "Introduceti al doilea numar:$"
 n3 DB "Introduceti al treilea numar:$"
 result DB "Rezultatul este:$"
.code ; reprezintă secțiunea în care se scrie codul sursă al programului
new_line proc
 MOV AH, 2
 MOV DL, 10 ; valoarea hexa 0x0A (în zecimal 10) corespunde codului ASCII al
;caracterului special LF – line feed, corespunzător lui `\n` (new line character) din C
 INT 21H
 ret
new_line endp
main proc ; proc este cuvântul cheie pentru începerea unei proceduri
    MOV AX, @data ; instrucțiuni implicite pentru încărcarea segmentului de date
    MOV DS, AX
    
    ;Citim A
    MOV AH, 9
    MOV DX, OFFSET n1
    INT 21h 

    MOV AH, 1
    INT 21H
    SUB AL, 30H
    MOV A, AL

    CALL new_line

    ;Citim B
    MOV AH, 9
    MOV DX, OFFSET n2
    INT 21h 

    MOV AH, 1
    INT 21H
    SUB AL, 30H
    MOV B, AL

    CALL new_line

    ;Citim C
    MOV AH, 9
    MOV DX, OFFSET n3
    INT 21h 

    MOV AH, 1
    INT 21H
    SUB AL, 30H
    MOV C, AL

    CALL new_line
    ;IF (A > B && A > C)
    MOV AL, A
    
    CMP AL, B
    JS CHECK_B
    JZ CHECK_B

    CMP AL, C 
    JS CHECK_B
    JZ CHECK_B

    ; -> A este cel mai mare
    MOV AH, 9
    MOV DX, OFFSET result
    INT 21h

    MOV AH, 2
    MOV DL, A
    ADD DL, 30h 
    INT 21H
    JMP FINAL

    ; ELSE IF (B > A && B > C)
CHECK_B:
    MOV AL, B
    CMP AL, A
    JS CHECK_C 
    JZ CHECK_C

    CMP AL, C
    JS CHECK_C
    JZ CHECK_C

    ; -> B este cel mai mare
    MOV AH, 9
    MOV DX, OFFSET result
    INT 21h

    MOV AH, 2
    MOV DL, B
    ADD DL, 30h  
    INT 21H
    JMP FINAL 

    ; ELSE (C e cel mai mare) 
CHECK_C:
    MOV AH, 9
    MOV DX, OFFSET result
    INT 21h

    MOV AH, 2
    MOV DL, C
    ADD DL, 30h 
    INT 21H
    JMP FINAL 
    
FINAL:
    MOV AH, 4CH
    INT 21H

main endp ; endp este cuvântul cheie care semnifică finalul unei proceduri
end main

