import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn import metrics

from matplotlib.ticker import NullFormatter  # useful for `logit` scale

mh1 = 42.62
sh1 = 1.4
mm1 = 266
sm1 = 2.2

mh2 = 43.37
sh2 = 1.87
mm2 = 266
sm2 = 2.6

mh3 = 43.15
sh3 = 1.35
mm3 = 266
sm3 = 2.53

plt.xlim(0, 300)
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 500)

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

f,(ax,ax2) = plt.subplots(1,2,sharey=True, facecolor='w')

ax.plot(x, h1, 'b-', linewidth=2, label="'1' 1kHz")
ax2.plot(x, m1, 'b--', linewidth=2, label="'0' 1kHz")
ax.plot(x, h2, 'r-', linewidth=2, label="'1' 10kHz")
ax2.plot(x, m2, 'r--', linewidth=2, label="'0' 10kHz")
ax.plot(x, h3, 'g-', linewidth=2, label="'1' 100kHz")
ax2.plot(x, m3, 'g--', linewidth=2, label="'0' 100kHz")

ax.set_xlim(30,55)
ax2.set_xlim(255,280)

ax.spines['right'].set_visible(False)
ax2.spines['left'].set_visible(False)
ax.yaxis.tick_left()
ax.tick_params(labelright='off')
ax2.yaxis.tick_right()

ax.legend()
ax2.legend()

plt.show()