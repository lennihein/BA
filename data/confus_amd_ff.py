import matplotlib.pyplot as plt

#(tn, fp, fn, tp)

ff_intel = [
     [[3089, 1991],[ 844, 4076]],
    [[2578, 2502], [1578, 3342]] ,
 [[2893, 2347] ,[1760, 3000]]
]

freqs = [1,10, 100]

tn = [x[0][0] for x in ff_intel]
fp = [x[0][1] for x in ff_intel]
fn = [x[1][0] for x in ff_intel]
tp = [x[1][1] for x in ff_intel]

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

ax.plot(freqs, fp, 'r--', linewidth=2, label="False Positives")
ax.plot(freqs, tp, 'r-', linewidth=2, label="True Positives")
ax.plot(freqs, fn, 'b--', linewidth=2, label="False Negatives")
ax.plot(freqs, tn, 'b-', linewidth=2, label="True Netatives")

ax.set_xscale('log')
ax.legend()
plt.show()