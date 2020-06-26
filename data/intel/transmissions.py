import matplotlib.pyplot as plt

freqs = [1, 10, 100]
ff_acc = [1,0.9, 0.62 ]
fr_acc = [1, 0.91, 0.66]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

ax.plot(freqs, ff_acc, 'b-', linewidth=2, label="F+F Accuracy")
ax.plot(freqs, fr_acc, 'r--', linewidth=2, label="F+R Accuracy")
ax.set_xscale('log')
ax.legend()
plt.show()