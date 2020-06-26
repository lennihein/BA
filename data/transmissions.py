import matplotlib.pyplot as plt

freqs = [1, 10, 100]
intel_ff_acc = [1,0.9, 0.62 ]
intel_fr_acc = [1, 0.91, 0.66]

amd_ff_acc = [0.72, 0.59,0.59]
amd_fr_acc = [0.94, 0.93, 0.76]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

ax.plot(freqs, intel_ff_acc, 'b-', linewidth=2, label="Intel F+F Accuracy")
ax.plot(freqs, intel_fr_acc, 'b--', linewidth=2, label="Intel F+R Accuracy")
ax.plot(freqs, amd_ff_acc, 'r-', linewidth=2, label="AMD F+F Accuracy")
ax.plot(freqs, amd_fr_acc, 'r--', linewidth=2, label="AMD F+R Accuracy")
ax.set_xscale('log')
ax.legend()
plt.show()