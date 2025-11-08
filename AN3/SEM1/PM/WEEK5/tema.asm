;if (A > B)
;   R = A % B;
;   Q = A / B;
;else
;   R = B % A;
;   Q = B / A;

;afis(R + Q);

dosseg
.model small ; prin această instrucțiune se selectează modelul de memorie folosit
.stack 100h ;declararea segmentului stivă
.data ; reprezintă secțiunea de declarare a datelor
 A DB 0
 B DB 0
 RES DB 0
 X DW 7
 n1 DB "Introduceti primul numar:$"
 n2 DB "Introduceti al doilea numar:$"
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

    MOV AH,9
    MOV DX, OFFSET n2
    INT 21H

    MOV AH, 1
    INT 21H
    SUB AL, 30H
    MOV B, AL

    CALL new_line

    ; === Comparare A si B
    MOV AL,A
    CMP AL, B
    JS B_MARE
    JNS A_MARE

A_MARE:
    MOV AL, A
    MOV BL, B
    MOV AH, 0
    DIV BL 

    ADD AL, AH
    JMP PRINT

B_MARE:
    MOV AL, B
    MOV BL, A
    MOV AH, 0
    DIV BL 

    ADD AL, AH
    JMP PRINT

PRINT:
    MOV RES, AL

    MOV AH, 9
    MOV DX, OFFSET result
    INT 21H

    MOV AL,RES
    ADD AL, 30H

    MOV AH, 2
    MOV DL, AL
    INT 21H
    JMP FINAL

FINAL:
    MOV AH, 4CH
    INT 21H

main endp ; endp este cuvântul cheie care semnifică finalul unei proceduri
end main