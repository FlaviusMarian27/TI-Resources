'''
Un număr întreg pozitiv n se numește perfect dacă suma tuturor divizorilor 
săi proprii (numerele mai mici decât n care îl divid exact) este egală cu n.
Exemplu: 28 este perfect deoarece divizorii săi sunt 1, 2, 4, 7 și 14, iar 
1+2+4+7+14=28.
Scrie o funcție care primește un parametru 
n și returnează True dacă este un număr perfect, altfel False.
'''

def perfect_number(number: int) -> bool:
    if number <= 1:
        return False
    sum_of_value = 0
    for i in range(1,number//2 + 1):
        if number % i == 0:
            sum_of_value = sum_of_value + i
    
    if sum_of_value == number:
        return True
    else:
        return False
    
print(perfect_number(6))  
print(perfect_number(28))  
print(perfect_number(12))