#ex1

import numpy as np
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeRegressor
import matplotlib.pyplot as plt

np.random.seed(42)
X = np.random.rand(350,1)
y = (X + 1.2)**3 - 10 + np.random.randn(350,1)/10

tree_clf = DecisionTreeRegressor(max_depth=3, min_samples_leaf = 5, random_state=42)
tree_clf.fit(X, y)

def plot_regression_predictions(tree_reg, X, y, axes=[0, 1, -8.0, 0]):
    x1 = np.linspace(axes[0], axes[1], 500).reshape(-1, 1)
    y_pred = tree_reg.predict(x1)
    plt.axis(axes)
    plt.xlabel("$x_1$", fontsize=18)
    plt.ylabel("$y$", fontsize=18, rotation=0)
    plt.plot(X, y, "b.")
    plt.plot(x1, y_pred, "r.-", linewidth=2, label=r"$\hat{y}$")

plot_regression_predictions(tree_clf, X, y)

plt.show()