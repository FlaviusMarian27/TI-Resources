#ex1 with optimizer and mseloss

import torch
from torch import nn
import numpy as np 

def synthetic_data(w, b, num_examples):  
    """Generate y = Xw + b + noise."""
    X = torch.normal(-3, 3, (num_examples, len(w)))
    y = X @ w + b
    y += torch.normal(0, 0.01, y.shape)#adauga zgomot element-wise
    return X, y.reshape(-1, 1)

# 1 * X + 1(-1) + noise
true_w = torch.tensor([1.0])#1 din inainte de X
true_b = -1 # bias din formula
features, labels = synthetic_data(true_w, true_b, 60) #60 ii num examples
#print('Features:', features[0], '\nLabel:', labels[0])

def load_array(data_arrays, batch_size, is_train=True): 
    """Construct a PyTorch data iterator."""
    dataset = torch.utils.data.TensorDataset(*data_arrays)
    return torch.utils.data.DataLoader(dataset, batch_size, shuffle=is_train)


batch_size = 10
data_iter = load_array((features, labels), batch_size)
#next(iter(data_iter))

net = nn.Sequential(nn.Linear(1, 1)) # pentru ca avem o intrare si o iesire. daca aveam 2 si 1 avem 2 intrari si o iesire
net[0].weight.data.normal_(1.0)# initializarea cu pentru weight si bias cum spune in enunt
net[0].bias.data.fill_(1.0)# initializarea cu pentru weight si bias cum spune in enunt

loss = nn.MSELoss()
optimizer = torch.optim.SGD(net.parameters(), lr=0.05)
num_epochs = 25
for epoch in range(num_epochs):
    for X, y in data_iter:
        l = loss(net(X), y)
        optimizer.zero_grad()
        l.backward()
        optimizer.step()
    l = loss(net(features), labels)
    print(f'Epoch {epoch + 1}, Loss {l:f}')