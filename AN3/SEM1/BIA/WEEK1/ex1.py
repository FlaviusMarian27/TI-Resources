'''
Creează un array aleatoriu NumPy cu cinci rânduri și patru coloane.
Folosind indexare și slicing, afișează elementele din rândurile de la al treilea 
până la ultimul și coloanele de la a doua până la ultima.
'''

import numpy as np

array = np.random.randint(1,100,size=(5,4))

print("Array original:\n", array)

print("\nElemente selectate (row 3:end, col 2:end):\n", array[2:, 1:])