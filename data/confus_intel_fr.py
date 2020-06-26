import matplotlib.pyplot as plt

#(tn, fp, fn, tp)

ff_intel = [[[5080, 0], [ 0, 4920]],
[[4784, 476], [ 474 ,4266]],
[[3234, 1706], [1657, 3403]]]

freqs = [1,10, 100]

tn = [x[0][0] for x in ff_intel]
fp = [x[0][1] for x in ff_intel]
fn = [x[1][0] for x in ff_intel]
tp = [x[1][1] for x in ff_intel]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

print(tp)

ax.plot(freqs, fp, 'r--', linewidth=2, label="False Positives")
ax.plot(freqs, tp, 'r-', linewidth=2, label="True Positives")
ax.plot(freqs, fn, 'b--', linewidth=2, label="False Negatives")
ax.plot(freqs, tn, 'b-', linewidth=2, label="True Netatives")

ax.set_xscale('log')
ax.legend()
plt.show()