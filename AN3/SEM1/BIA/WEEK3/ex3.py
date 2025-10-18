#ex3
from sklearn.svm import LinearSVR

np.random.seed(42)
n = 50
X = 6 * np.random.rand(n,1)
y = (1 - 2 * X + np.random.randn(n,1)).ravel()

svm_reg1 = LinearSVR(epsilon=0.8, random_state=42)
svm_reg1.fit(X, y)

def find_support_vectors(svm_reg, X, y):
    y_pred = svm_reg.predict(X)
    off_margin = (np.abs(y - y_pred) >= svm_reg.epsilon)
    return np.argwhere(off_margin)

svm_reg1.support_ = find_support_vectors(svm_reg1, X, y)

def plot_svm_regression(svm_reg, X, y, axes):
    x1s = np.linspace(axes[0], axes[1], 100).reshape(100, 1)
    y_pred = svm_reg.predict(x1s)
    plt.plot(x1s, y_pred, "k-", linewidth=2, label=r"$\hat{y}$")
    plt.plot(x1s, y_pred + svm_reg.epsilon, "k--")
    plt.plot(x1s, y_pred - svm_reg.epsilon, "k--")
    plt.scatter(X[svm_reg.support_], y[svm_reg.support_], s=200, facecolors='#FFAAAA')
    plt.plot(X, y, "bo")
    plt.xlabel(r"$x_1$", fontsize=18)
    plt.legend(loc="upper left", fontsize=18)
    plt.axis(axes)

fig, axes = plt.subplots(ncols=1, figsize=(9, 4))#aici ncols ii 1 pentru ca vrem un singur grafic
plt.sca(axes)#aici inaite era axes[0] dar eu am nevoie de un singur grafic
plot_svm_regression(svm_reg1, X, y, [0, 6, -15, 10])
plt.title(r"$\epsilon = {}$".format(svm_reg1.epsilon), fontsize=18)
plt.ylabel(r"$y$", fontsize=18, rotation=0)