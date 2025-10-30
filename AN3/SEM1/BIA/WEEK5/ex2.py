#ex2
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.datasets import load_digits
from sklearn.ensemble import BaggingClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
import time
from sklearn.decomposition import PCA

digits = datasets.load_digits()

X_digits = digits.data
y_digits = digits.target

X_train, X_test, y_train, y_test = train_test_split(X_digits, y_digits, test_size = 0.25, random_state=42)

svm_clf = SVC(kernel = "linear", C = 5)

t0 = time.time()
svm_clf.fit(X_train, y_train)
t1 = time.time()

print("Training took {:.2f}s".format(t1 - t0))
print("SVM score(64): ", svm_clf.score(X_test, y_test))

pca = PCA(n_components=20)
X_reduced = pca.fit_transform(X_train)

svm_clf = SVC(kernel = "linear", C = 5)

t0 = time.time()
svm_clf.fit(X_reduced, y_train)
t1 = time.time()

print("Training took(20) {:.2f}s".format(t1 - t0))
X_test_reduced = pca.transform(X_test)
print("SVM score(20): ", svm_clf.score(X_test_reduced, y_test))