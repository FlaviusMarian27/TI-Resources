;if( (A+B) != 0 ){
;    if( (A+B) > C ){
;        afis((A+B)^3)
;    }else{
;        afis(C++)
;    }
;}else{
;    afis(C--)
;}

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
    INT 21H

    MOV AH, 1
    INT 21H
    SUB AL, 30H 
    MOV A, AL

    CALL new_line

    ;Citim B
    MOV AH, 9
    MOV DX, OFFSET n2
    INT 21H

    MOV AH, 1
    INT 21H
    SUB AL, 30H 
    MOV B, AL

    CALL new_line

    ;Citim C
    MOV AH, 9
    MOV DX, OFFSET n3
    INT 21H

    MOV AH, 1
    INT 21H
    SUB AL, 30H 
    MOV C, AL

    CALL new_line

    ;if(A+B) != 0
    MOV AL, A 
    ADD AL, B 
    CMP AL, 0
    JS ELSE_MAIN
    
    ;if(A+B) > C
    CMP AL, C
    JS ELSE_INNER
    JZ ELSE_INNER

    ;(A+B)^3
    MOV BL, AL
    MOV AH, 0
    MUL BL      ; AX = (A+B)^2
    MOV BL, AL  ; BL = (A+B)^2
    MOV AL, A
    ADD AL, B
    MUL BL      ; AX = (A+B)^3
    MOV B, AL

    MOV AH, 9
    MOV DX, OFFSET result
    INT 21H

    MOV AL, B
    MOV AH, 0
    MOV BL, 10
    DIV BL  

    MOV AH, 2
    MOV DL, B
    ADD DL, 30H
    INT 21H

    JMP FINAL

    ;C++
ELSE_INNER:
    INC C
    MOV AH, 9
    MOV DX, OFFSET result
    INT 21H

    MOV AH, 2
    MOV DL, C
    ADD DL, 30H
    INT 21H

    JMP FINAL    

ELSE_MAIN:
    DEC C
    MOV AH, 9
    MOV DX, OFFSET result
    INT 21H

    MOV AH, 2
    MOV DL, C
    ADD DL, 30H
    INT 21H

    JMP FINAL

FINAL:
    MOV AH, 4CH
    INT 21H
main endp ; endp este cuvântul cheie care semnifică finalul unei proceduri
end main
