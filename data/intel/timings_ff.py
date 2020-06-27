import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn import metrics

mh1 = 157
sh1 = 2.9
mm1 = 127
sm1 = 2.6

mh2 = 158
sh2 = 2.9
mm2 = 126
sm2 = 2.6

mh3 = 139
sh3 = 15.3
mm3 = 142
sm3 = 15.4

plt.xlim(100, 190)
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)

h1 = norm.pdf(x, mh1, sh1)
plt.plot(x, h1, 'b-', linewidth=2, label="'1' 1kHz")

m1 = norm.pdf(x, mm1, sm1)
plt.plot(x, m1, 'b--', linewidth=2, label="'0' 1kHz")

h2 = norm.pdf(x, mh2, sh2)
plt.plot(x, h2, 'r-', linewidth=2, label="'1' 10kHz")

m2 = norm.pdf(x, mm2, sm2)
plt.plot(x, m2, 'r--', linewidth=2, label="'0' 10kHz")

h3 = norm.pdf(x, mh3, sh3)
plt.plot(x, h3, 'g-', linewidth=2, label="'1' 100kHz")

m3 = norm.pdf(x, mm3, sm3)
plt.plot(x, m3, 'g--', linewidth=2, label="'0' 100kHz")


plt.legend()
plt.show()