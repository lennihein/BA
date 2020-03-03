import numpy as np
from scipy.stats import norm
import matplotlib.pyplot as plt
from sklearn import metrics


# https://stackoverflow.com/questions/22579434/python-finding-the-intersection-point-of-two-gaussian-curves
def solve(m1, m2, std1, std2):
    a = 1 / (2 * std1 ** 2) - 1 / (2 * std2 ** 2)
    b = m2 / (std2 ** 2) - m1 / (std1 ** 2)
    c = m1 ** 2 / (2 * std1 ** 2) - m2 ** 2 / (2 * std2 ** 2) - np.log(std2 / std1)
    return np.roots([a, b, c])


with open('threshholds_out_new.txt', 'r') as f:
    lines = f.readlines()
    NUMBEROFPOINTS = int(lines[0])
    if lines[1] == "ff\n":
        method = "ff"
        print("Evaluating FLUSH+FLUSH data")
    elif lines[1] == "fr\n":
        method = "fr"
        print("Evaluating FLUSH+RELOAD data")
    else:
        print("WTF1: method is ", lines[1])
        exit(1)

    if len(lines) != NUMBEROFPOINTS * 2 + 1 + 1:
        print("WTF2")
        exit(1)

    hits = [int(i) for i in lines[1 + 1:(NUMBEROFPOINTS + 1 + 1)]]
    misses = [int(i) for i in lines[(NUMBEROFPOINTS + 1 + 1):(NUMBEROFPOINTS * 2 + 1 + 1)]]

misses = [min(1000, i) for i in misses]
hits = [min(1000, i) for i in hits]

# Keep only the "good" points
# "~" operates as a logical not operator on boolean numpy arrays

# fit for Misses
mu_miss, std_miss = norm.fit(misses)

# fit for Hits
mu_hit, std_hit = norm.fit(hits)

plt.hist(hits, bins=200, density=True, alpha=0.6, color='g')
plt.hist(misses, bins=200, density=True, alpha=0.6, color='r')

if method == "ff":
    plt.xlim(120, 220)
else:
    plt.xlim(50, 1000)
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)
p1 = norm.pdf(x, mu_hit, std_hit)
plt.plot(x, p1, 'k', linewidth=2)
print("Fit results Hits: mu = %.2f,  std = %.2f" % (mu_hit, std_hit))

p2 = norm.pdf(x, mu_miss, std_miss)
plt.plot(x, p2, 'k', linewidth=2)
print("Fit results Misses: mu = %.2f,  std = %.2f" % (mu_miss, std_miss))

result = solve(mu_hit, mu_miss, std_hit, std_miss)

# get confusion matrix for threshhold
# Hits are to be seen as 1
# Misses are to be seen as 0
# therefore everything  <= threshhold is Miss
#           everything > threshhold is Hit

max_acc = 0
best_threshhold = 0
for threshhold in result:
    inputs = hits + misses
    if method == "ff":
        pred = ["Hit" if i > threshhold else "Miss" for i in inputs]
    elif method == "fr":
        pred = ["Hit" if i <= threshhold else "Miss" for i in inputs]
    else:
        print("WTF3")
        pred = None
        exit(1)

    act = ["Hit"] * NUMBEROFPOINTS + ["Miss"] * NUMBEROFPOINTS
    accuracy = metrics.accuracy_score(act, pred)
    print("\nThreshhold: ", threshhold, "\nAccuracy: ", accuracy,)
    if accuracy > max_acc:
        max_acc = accuracy
        best_threshhold = threshhold

threshhold = best_threshhold
print("\n\nTHRESHHOLD: ", threshhold)
inputs = hits + misses
if method == "ff":
    pred = ["Hit" if i > threshhold else "Miss" for i in inputs]
elif method == "fr":
    pred = ["Hit" if i <= threshhold else "Miss" for i in inputs]
else:
    print("WTF3")
    pred = None
    exit(1)

act = ["Hit"] * NUMBEROFPOINTS + ["Miss"] * NUMBEROFPOINTS
print("\n", metrics.confusion_matrix(act, pred, labels=["Hit", "Miss"]), "\n")
report = metrics.classification_report(act, pred, labels=["Hit", "Miss"])
print(report)
print("Accuracy: ", metrics.accuracy_score(act, pred))

plt.show()
