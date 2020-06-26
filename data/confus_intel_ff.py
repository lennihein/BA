import matplotlib.pyplot as plt

#(tn, fp, fn, tp)

ff_intel = [[[4986, 4], [ 0, 5010]],
[[4554, 206], [ 207, 5033]],
[[3317 ,1853], [1907, 2923]]]

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