import numpy as np
import matplotlib.pyplot as plt
from sklearn import metrics


# https://stackoverflow.com/questions/22579434/python-finding-the-intersection-point-of-two-gaussian-curves
def solve(m1, m2, std1, std2):
    a = 1 / (2 * std1 ** 2) - 1 / (2 * std2 ** 2)
    b = m2 / (std2 ** 2) - m1 / (std1 ** 2)
    c = m1 ** 2 / (2 * std1 ** 2) - m2 ** 2 / (2 * std2 ** 2) - np.log(std2 / std1)
    return np.roots([a, b, c])


with open('groupA.txt', 'r') as f1:
    hits = f1.readlines()
    hits = [int(i) for i in hits]

with open('groupB.txt') as f2:
    misses = f2.readlines()
    misses = [int(i) for i in misses]


# Keep only the "good" points
# misses = [min(1000, i) for i in misses]
# hits = [min(1000, i) for i in hits]

hits_cleaned = np.array(hits)
misses_cleaned = np.array(misses)

deviations = 10

d_hits_cleaned = np.abs(hits_cleaned - np.median(hits_cleaned))
med_abs_dev_hits_cleaned = np.median(d_hits_cleaned)
if med_abs_dev_hits_cleaned != 0:
    s_hits_cleaned = d_hits_cleaned / med_abs_dev_hits_cleaned
    hits_cleaned = hits_cleaned[s_hits_cleaned < deviations]

d_misses_cleaned = np.abs(misses_cleaned - np.median(misses_cleaned))
med_abs_dev_misses_cleaned = np.median(d_misses_cleaned)
if med_abs_dev_misses_cleaned != 0:
    s_misses_cleaned = d_misses_cleaned / med_abs_dev_misses_cleaned
    misses_cleaned = misses_cleaned[s_misses_cleaned < deviations]

# fit for Misses
mu_miss, std_miss = np.mean(misses_cleaned), np.std(misses_cleaned)

# fit for Hits
mu_hit, std_hit = np.mean(hits_cleaned), np.std(hits_cleaned)

if mu_miss > mu_hit:
    (misses, hits) = (hits, misses)
    (misses_cleaned, hits_cleaned) = (hits_cleaned, misses_cleaned)

    # fit for Misses
    mu_miss, std_miss = norm.fit(misses_cleaned)

    # fit for Hits
    mu_hit, std_hit = norm.fit(hits_cleaned)
    if mu_miss > mu_hit:
        print("WTF")
        exit(1)

plt.hist(hits, density=True, align="mid", alpha=0.6, color='g', label="Hits")
plt.hist(misses, density=True, align="mid", histtype="barstacked", alpha=0.6, color='r', label="Misses")

plt.xlim(min(min(hits_cleaned), min(misses_cleaned)) - 5, max(max(hits_cleaned), max(misses_cleaned)) + 5)
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)
p1 = norm.pdf(x, mu_hit, std_hit)
plt.plot(x, p1, 'k', linewidth=2)
print("Fit results Hits: mu = %.2f,  std = %.2f" % (mu_hit, std_hit))

p2 = norm.pdf(x, mu_miss, std_miss)
plt.plot(x, p2, 'k', linewidth=2)
print("Fit results Misses: mu = %.2f,  std = %.2f" % (mu_miss, std_miss))

plt.legend()

result = solve(mu_hit, mu_miss, std_hit, std_miss)

# get confusion matrix for threshhold
# Hits are to be seen as 1
# Misses are to be seen as 0
# therefore everything  <= threshhold is Miss
#           everything > threshhold is Hit

method = "ff"
NUMBEROFPOINTS = len(hits)
print(len(hits))
print(len(misses))

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
