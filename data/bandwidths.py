import matplotlib.pyplot as plt
import scipy.stats as stats

def ent2(val):
    return stats.entropy([1-val] + [val], base=2)

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
freqs = [1,10, 100]

ff_amd = [
    [[3089, 1991], [ 844, 4076]],
    [[2578, 2502], [1578, 3342]],
    [[2893, 2347], [1760, 3000]]
]
fr_amd = [
    [[4903, 247], [ 327, 4523]],
    [[2*2455, 2*0], [ 2*156, 2*2389]],
    [[3832, 1168] ,[1185, 3815]]
]
ff_intel = [
    [[4986, 4], [ 0, 5010]],
    [[4554, 206], [ 207, 5033]],
    [[3317 ,1853], [1907, 2923]]
]
fr_intel = [
    [[5080, 0], [ 0, 4920]],
    [[4784, 476], [ 474 ,4266]],
    [[3234, 1706], [1657, 3403]]
]

#####################################
tn = [x[0][0] for x in fr_intel]
fp = [x[0][1] for x in fr_intel]
fn = [x[1][0] for x in fr_intel]
tp = [x[1][1] for x in fr_intel]
bandwidths = []
for i in range(3):
    p = (tn[i]+tp[i]) / (fp[i] + fn[i] + tn[i] + tp[i])
    H = ent2(p)
    N = 1000 / (1-H)
    mxbandwidth = 1000*(10**i) * 1000/(N)
    print("Max Bandwith Intel FR @" + str(10**i) + "kHz: {0:0.2f}".format(mxbandwidth/1000) + "kBit/s")
    bandwidths += [mxbandwidth]
ax.plot(freqs, bandwidths, "b--", linewidth=4, label="Intel Flush+Reload")

#####################################
tn = [x[0][0] for x in fr_amd]
fp = [x[0][1] for x in fr_amd]
fn = [x[1][0] for x in fr_amd]
tp = [x[1][1] for x in fr_amd]
bandwidths = []
for i in range(3):
    p = (tn[i]+tp[i]) / (fp[i] + fn[i] + tn[i] + tp[i])
    H = ent2(p)
    N = 1000 / (1-H)
    mxbandwidth = 1000*(10**i) * 1000/(N)
    print("Max Bandwith AMD FR @" + str(10**i) + "kHz: {0:0.2f}".format(mxbandwidth/1000) + "kBit/s")
    bandwidths += [mxbandwidth]
ax.plot(freqs, bandwidths, "r--", linewidth=4, label="AMD Flush+Reload")

#####################################
tn = [x[0][0] for x in ff_intel]
fp = [x[0][1] for x in ff_intel]
fn = [x[1][0] for x in ff_intel]
tp = [x[1][1] for x in ff_intel]
bandwidths = []
for i in range(3):
    p = (tn[i]+tp[i]) / (fp[i] + fn[i] + tn[i] + tp[i])
    H = ent2(p)
    N = 1000 / (1-H)
    mxbandwidth = 1000*(10**i) * 1000/(N)
    print("Max Bandwith Intel FF @" + str(10**i) + "kHz: {0:0.2f}".format(mxbandwidth/1000) + "kBit/s")
    bandwidths += [mxbandwidth]
ax.plot(freqs, bandwidths, "b-", linewidth=2, label="Intel Flush+Flush")

#####################################
tn = [x[0][0] for x in ff_amd]
fp = [x[0][1] for x in ff_amd]
fn = [x[1][0] for x in ff_amd]
tp = [x[1][1] for x in ff_amd]
bandwidths = []
for i in range(3):
    p = (tn[i]+tp[i]) / (fp[i] + fn[i] + tn[i] + tp[i])
    H = ent2(p)
    N = 1000 / (1-H)
    mxbandwidth = 1000*(10**i) * 1000/(N)
    print("Max Bandwith AMD FF @" + str(10**i) + "kHz: {0:0.2f}".format(mxbandwidth/1000) + "kBit/s")
    bandwidths += [mxbandwidth]
ax.plot(freqs, bandwidths, "r-", linewidth=2, label="AMD Flush+Flush")

ax.set_xscale('log')
ax.legend()
plt.show()