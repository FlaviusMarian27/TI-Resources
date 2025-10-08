import torch

t1 = torch.rand(5)
t2 = torch.rand(3,4)
t3 = torch.rand(2,3,4)

print("t1 shape:", t1.shape)
print("t2 shape:", t2.shape)
print("t3 shape:", t3.shape)

print("\nT1 =", t1)
print("\nT2 =\n", t2)
print("\nT3 =\n", t3)