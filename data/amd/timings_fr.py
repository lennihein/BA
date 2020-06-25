import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn import metrics

from matplotlib.ticker import NullFormatter  # useful for `logit` scale

mh1 = 43
sh1 = 1.4
mm1 = 266
sm1 = 2.2

mh2 = 43
sh2 = 1.9
mm2 = 266
sm2 = 2.6

mh3 = 43
sh3 = 1.4
mm3 = 266
sm3 = 2.53

plt.xlim(0, 300)
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

f,(ax,ax2) = plt.subplots(1,2,sharey=True, facecolor='w')

ax.plot(x, h1, 'b-', linewidth=2, label="1kHz Hits")
ax2.plot(x, m1, 'b--', linewidth=2, label="1kHz Misses")
ax.plot(x, h2, 'r-', linewidth=2, label="10kHz Hits")
ax2.plot(x, m2, 'r--', linewidth=2, label="10kHz Misses")
ax.plot(x, h3, 'g-', linewidth=2, label="100kHz Hits")
ax2.plot(x, m3, 'g--', linewidth=2, label="100kHz Misses")

ax.set_xlim(30,55)
ax2.set_xlim(255,280)

ax.spines['right'].set_visible(False)
ax2.spines['left'].set_visible(False)
ax.yaxis.tick_left()
ax.tick_params(labelright='off')
ax2.yaxis.tick_right()

plt.legend()
plt.show()