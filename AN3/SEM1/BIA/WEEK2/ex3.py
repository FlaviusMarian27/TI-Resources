#ex3
from sklearn.linear_model import LogisticRegression
from sklearn import datasets
from sklearn.model_selection import train_test_split

digits_dataset = datasets.load_digits()
X_train, X_test, y_train, y_test = train_test_split(digits_dataset['data'], digits_dataset['target'], test_size=0.35,random_state=0)

print(X_train.shape)
print(y_train.shape)
print(X_test.shape)
print(y_test.shape)

softmax_reg = LogisticRegression(max_iter=1000, multi_class="multinomial", solver="lbfgs", C=5, random_state=42)
softmax_reg.fit(X_train, y_train)
softmax_reg.score(X_test, y_test)

softmax_reg.predict_proba([X_test[1]])

#print(digits_dataset.DESCR)

import numpy as np
import matplotlib.pyplot as plt

digit_array = X_test[1]
digit_image = digit_array.reshape((8, 8))

plt.imshow(digit_image, cmap='gray', interpolation='nearest')
plt.title('Handwritten Digit')
plt.colorbar()
plt.show()

print("Forma imaginii:", X_test[1].shape)
print("Eticheta reală (y_test[1]):", y_test[1])