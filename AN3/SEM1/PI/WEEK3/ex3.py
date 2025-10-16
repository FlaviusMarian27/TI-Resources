#ex3

import torchvision
from torchvision import transforms
import torch
import numpy as np
from torch import nn

def load_data_fashion_mnist(batch_size, resize=None):
    """Download the Fashion-MNIST dataset and then load it into memory."""
    trans = [transforms.ToTensor()]
    if resize:
        trans.insert(0, transforms.Resize(resize))
    trans = transforms.Compose(trans)
    mnist_train = torchvision.datasets.MNIST(
        root="../data", train=True, transform=trans, download=True)
    #MNIST pt 0 - 9------------------------------------------------------------------------------------
    mnist_test = torchvision.datasets.MNIST(
        root="../data", train=False, transform=trans, download=True)
    mnist_train, mnist_val = torch.utils.data.random_split(mnist_train, [45000, 15000],#aici------------------------------------------------------------------------
                                                           generator=torch.Generator().manual_seed(42))
    #the validation dataset consists of 30000 images and the rest represents the training dataset -> pentru ca 45000 is pt data si restul de 15000 sunt pentru train
    return (torch.utils.data.DataLoader(mnist_train, batch_size, shuffle=True,
                            num_workers=2),
            torch.utils.data.DataLoader(mnist_val, batch_size, shuffle=False,
                            num_workers=2),
            torch.utils.data.DataLoader(mnist_test, batch_size, shuffle=False,
                            num_workers=2))

batch_size = 512
train_iter, val_iter, test_iter = load_data_fashion_mnist(batch_size)


net = nn.Sequential(nn.Flatten(),
        nn.Linear(784, 256),
        nn.Tanh(),
        # Add a dropout layer after the first fully connected layer
        nn.Dropout(0.4),
        nn.Linear(256, 256),
        nn.Tanh(),
        # Add a dropout layer after the second fully connected layer
        nn.Dropout(0.4),
        nn.Linear(256, 10))

def xavier(m):
    if type(m) == nn.Linear:
        nn.init.xavier_uniform_(m.weight)
        nn.init.zeros_(m.bias)

net.apply(xavier)

loss = nn.CrossEntropyLoss()
lr = 0.15
optimizer = torch.optim.SGD(net.parameters(), lr=lr)

def evaluate_accuracy(net, data_iter, loss):
    net.eval()
    total_loss, total_correct, total_samples = 0, 0, 0
    with torch.no_grad():
        for X, y in data_iter:
            y_hat = net(X)
            l = loss(y_hat, y)
            total_loss += l.item()
            total_correct += (y_hat.argmax(dim=1) == y).sum().item()
            total_samples += y.size(0)
    avg_loss = total_loss / len(data_iter)
    accuracy = 100 * total_correct / total_samples
    return avg_loss, accuracy


def train_epoch(net, train_iter, loss, optimizer):
    net.train()
    total_loss, total_correct, total_samples = 0, 0, 0
    for X, y in train_iter:
        y_hat = net(X)
        l = loss(y_hat, y)
        optimizer.zero_grad()
        l.backward()
        optimizer.step()
        total_loss += l.item()
        total_correct += (y_hat.argmax(dim=1) == y).sum().item()
        total_samples += y.size(0)
    avg_loss = total_loss / len(train_iter)
    accuracy = 100 * total_correct / total_samples
    return avg_loss, accuracy


def train(net, train_iter, val_iter, loss, optimizer, num_epochs=10):
    train_loss_all, train_acc_all, val_loss_all, val_acc_all = [], [], [], []

    for epoch in range(num_epochs):
        train_loss, train_acc = train_epoch(net, train_iter, loss, optimizer)
        val_loss, val_acc = evaluate_accuracy(net, val_iter, loss)
        train_loss_all.append(train_loss)
        train_acc_all.append(train_acc)
        val_loss_all.append(val_loss)
        val_acc_all.append(val_acc)
        print(f"Epoch {epoch+1:2d}: "
              f"Train loss {train_loss:.3f}, Train acc {train_acc:.2f}%, "
              f"Val loss {val_loss:.3f}, Val acc {val_acc:.2f}%")

    return train_loss_all, train_acc_all, val_loss_all, val_acc_all

num_epochs = 10
train_loss_all, train_acc_all, val_loss_all, val_acc_all = train(
    net, train_iter, val_iter, loss, optimizer, num_epochs
)