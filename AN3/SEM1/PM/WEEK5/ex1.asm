.model small
.stack 100h
.data
A db 3
B db 2
C db 6
msg1 db "Rezultat IF: $"
msg2 db "Rezultat ELSE: $"

.code
main proc
    MOV AX, @data
    MOV DS, AX

; AL = A + B
    MOV AL, A
    ADD AL, B          ; AL = A+B
    MOV BL, AL         ; BL păstrează (A+B) pentru comparație

    ; compar (A+B) cu C (fără decrement încă)
    CMP BL, C
    JGE ELSE_          ; dacă (A+B) >= C -> ramura ELSE

    ; ===== IF: (A+B) < C =====
    DEC BL             ; (A+B)--
    ; Afișează mesaj + valoarea (salvată în BL)
    MOV AH, 9
    MOV DX, OFFSET msg1
    INT 21H            ; ATENȚIE: AL se strică aici!

    MOV DL, BL         ; refolosim BL, nu AL
    ADD DL, 30h        ; conversie în ASCII
    MOV AH, 2
    INT 21H
    JMP FINAL

ELSE_:
    INC C              ; C++
    MOV AH, 9
    MOV DX, OFFSET msg2
    INT 21H
    MOV DL, C
    ADD DL, 30h
    MOV AH, 2
    INT 21H

FINAL:
    MOV AH, 4Ch
    INT 21H
main endp
end main