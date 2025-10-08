import torch

x = torch.tensor([-10,10,8])
sig = torch.sigmoid(x)
dsig = sig * (1 - sig)

print("\nSigmoid(x):", sig)
print("Sigmoid'(x):", dsig)