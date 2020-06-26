import matplotlib.pyplot as plt

freqs = [1, 10, 100]
ff_acc = [0.76, 0.54, 0.59]
fr_acc = [0.94, 0.97, 0.76]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

ax.plot(freqs, ff_acc, 'b-', linewidth=2, label="F+F Accuracy")
ax.plot(freqs, fr_acc, 'r--', linewidth=2, label="F+R Accuracy")
ax.set_xscale('log')
ax.legend()
plt.show()