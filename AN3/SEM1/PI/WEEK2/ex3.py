import torch

A = torch.tensor([[1,2,-3],[4,5,10]])
B = torch.tensor([[10,11,-2],[13,-3,8]])

hadamard = torch.mul(A,B)
a = 10
B_plus = B + a

A_T = A.T
B_T = B.T

print("\nHadamard Product (A * B):\n", hadamard)
print("\nB + 10:\n", B_plus)
print("\nA Transpose:\n", A_T)
print("\nB Transpose:\n", B_T)