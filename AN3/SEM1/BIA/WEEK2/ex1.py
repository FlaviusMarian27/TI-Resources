import matplotlib.pyplot as plt
import numpy as np
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression

X = (2 - 3*np.random.normal(0,1,200)).reshape(200,1)
y = 0.5*X**3 - 2*X**2 + X + np.random.normal(-3,3,200).reshape(-1,1)

#creeam polinom de gradul 3
poly_features = PolynomialFeatures(degree=3, include_bias=False)
X_poly = poly_features.fit_transform(X)

#regresia lineara
lin_reg = LinearRegression()
lin_reg.fit(X_poly, y)
lin_reg.intercept_, lin_reg.coef_

#generarea datelor de test pt predictii
X_new = np.linspace(-5, 10, 200).reshape(200, 1)
X_new_poly = poly_features.transform(X_new)
y_new = lin_reg.predict(X_new_poly)

#plottul
plt.plot(X, y, "b.")
plt.plot(X_new, y_new, "r-", linewidth=2, label="Predictions")
plt.xlabel("$x_1$", fontsize=18)
plt.ylabel("$y$", rotation=0, fontsize=18)
plt.legend(loc="upper left", fontsize=14)
plt.axis([-5, 10, -200, 200])
plt.show()
