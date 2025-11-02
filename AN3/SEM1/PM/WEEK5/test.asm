.model small
.stack 100h
.data
msg db "Salut din Assembly!$"

.code
main proc
    mov ax, @data
    mov ds, ax
    mov ah, 9
    mov dx, offset msg
    int 21h

    mov ah, 4Ch
    int 21h
main endp
end main
