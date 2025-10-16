#ex1 without optimizer and mseloss
import torch
from torch import nn
import numpy as n

def synthetic_data(w, b, num_examples):  
    """Generate y = Xw + b + noise."""
    X = torch.normal(-3, 3, (num_examples, len(w)))
    y = X @ w + b
    y += torch.normal(0, 0.01, y.shape)
    return X, y.reshape(-1, 1)

true_w = torch.tensor([1.0])
true_b = -1
features, labels = synthetic_data(true_w, true_b, 60)

def load_array(data_arrays, batch_size, is_train=True): 
    """Construct a PyTorch data iterator."""
    dataset = torch.utils.data.TensorDataset(*data_arrays)
    return torch.utils.data.DataLoader(dataset, batch_size, shuffle=is_train)

batch_size = 10
data_iter = load_array((features, labels), batch_size)

net = nn.Sequential(nn.Linear(1, 1))
net[0].weight.data.normal_(1.0)
net[0].bias.data.fill_(1.0)

def lossFunction(y_in,y_pred_in):
  return (y_pred_in - y_in).pow(2).mean()

num_epochs = 25
lr = 0.05 #din enunt avem atat lrul
for epoch in range(num_epochs):
    for X, y in data_iter:
        l = lossFunction(net(X), y)

        #optimizer.zero_grad()
        for param in net.parameters():
          param.grad.data.zero_() if param.grad is not None else None

        l.backward()
        
        #optimizer.step()
        for param in net.parameters():
          param.data = param.data -lr * param.grad

    l = lossFunction(net(features), labels)
    print(f'Epoch {epoch + 1}, Loss {l:f}')