import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn import metrics

mh1 = 436
sh1 = 38
mm1 = 397
sm1 = 47.6

mh2 = 421
sh2 = 31.9
mm2 = 381
sm2 = 35.6

mh3 = 404
sh3 = 24.7
mm3 = 380
sm3 = 23.8

plt.xlim(200, 600)
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)

h1 = norm.pdf(x, mh1, sh1)
plt.plot(x, h1, 'b-', linewidth=2, label="1kHz Hits")

m1 = norm.pdf(x, mm1, sm1)
plt.plot(x, m1, 'b--', linewidth=2, label="1kHz Misses")

h2 = norm.pdf(x, mh2, sh2)
plt.plot(x, h2, 'r-', linewidth=2, label="10kHz Hits")

m2 = norm.pdf(x, mm2, sm2)
plt.plot(x, m2, 'r--', linewidth=2, label="10kHz Misses")

h3 = norm.pdf(x, mh3, sh3)
plt.plot(x, h3, 'g-', linewidth=2, label="100kHz Hits")

m3 = norm.pdf(x, mm3, sm3)
plt.plot(x, m3, 'g--', linewidth=2, label="100kHz Misses")


plt.legend()
plt.show()