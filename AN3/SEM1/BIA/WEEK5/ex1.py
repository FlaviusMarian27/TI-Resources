#ex1
import numpy as np
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split

iris_dataset = load_iris()

X, y = iris_dataset.data, iris_dataset.target
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=0)

from sklearn.decomposition import PCA

pca = PCA(n_components=3) #aici
X3D = pca.fit_transform(X) #aici

print("Principal components:")
print(pca.components_)

print("\nExplained variance ratio:")
print(pca.explained_variance_ratio_)

import matplotlib.pyplot as plt

axes = [-3, 3, -1, 1, -1, 1]#aici

fig = plt.figure(figsize=(6, 5))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(X3D[:, 0], X3D[:, 1], X3D[:, 2], c=y, cmap=plt.cm.viridis)#aici
ax.view_init(10, -60)
ax.set_xlabel("$x_1$", fontsize=18)
ax.set_ylabel("$x_2$", fontsize=18)
ax.set_zlabel("$x_3$", fontsize=18)
ax.set_xlim(axes[0:2])
ax.set_ylim(axes[2:4])
ax.set_zlim(axes[4:6])

plt.show()

from sklearn.manifold import TSNE

tsne = TSNE(n_components=3, random_state=42)#aici
X_reduced_tsne = tsne.fit_transform(X)


axes = [-80, 80, -80, 80, -80, 80]#aici

fig = plt.figure(figsize=(6, 5))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(X_reduced_tsne[:, 0], X_reduced_tsne[:, 1], X_reduced_tsne[:, 2], c=y, cmap=plt.cm.viridis)#aici
ax.view_init(10, -60)
ax.set_xlabel("$x_1$", fontsize=18)
ax.set_ylabel("$x_2$", fontsize=18)
ax.set_zlabel("$x_3$", fontsize=18)
ax.set_xlim(axes[0:2])
ax.set_ylim(axes[2:4])
ax.set_zlim(axes[4:6])

plt.show()