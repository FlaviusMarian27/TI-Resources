import numpy as np

eta = 0.06
n_epochs = 15
batch_size = 10
n = 150
alpha = 0.01

X = 3 * np.random.rand(n,1)
y = -2 + 3*X + np.random.randn(n,1)

w = np.random.randn(2,1) #model initializat random
X_1 = np.c_[np.ones((n, 1)), X]# adaugam un bias term la X

for epoch in range(n_epochs):
    shuffled_indices = np.random.permutation(n)
    X_1_shuffled = X_1[shuffled_indices]
    y_shuffled = y[shuffled_indices]
    for i in range(0, n, batch_size):
        xi = X_1_shuffled[i:i+batch_size]
        yi = y_shuffled[i:i+batch_size]
        gradients = 1/batch_size * xi.T.dot(xi.dot(w) - yi) + alpha * w
        w = w - eta * gradients

print("The final model weights: \n",w)

X_new = np.linspace(-3, 3, n).reshape(n, 1)
X_new_1 = np.c_[np.ones((n, 1)), X_new]
y_new = X_new_1.dot(w)

plt.plot(X, y, "b.")
plt.plot(X_new, y_new, "r-", linewidth=2, label="Predictions")
plt.xlabel("$x_1$", fontsize=18)
plt.ylabel("$y$", rotation=0, fontsize=18)
plt.legend(loc="upper left", fontsize=14)
plt.axis([0, 3, -5, 10])
plt.show()