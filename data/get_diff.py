from sklearn import metrics
from scipy.stats import wasserstein_distance
import matplotlib.pyplot as plt
import numpy as np

print("Results for the Transmission:")

pred_file = open('pred.txt', 'r')
pred = pred_file.readlines()
pred = [int(i) for i in pred]

act_file = open('act.txt', 'r')
act = act_file.readlines()
act = [int(i) for i in act]

STREAM_LENGTH = len(pred)

datasets = len(pred)/len(act)

if not datasets.is_integer():
    print("ERROR: len(pred) is not multiple of len(act). (" + str(len(act)) + "-" + str(len(pred)) + ")")

act = act*int(datasets)

diff = [0] * STREAM_LENGTH

for i in range(STREAM_LENGTH):
    diff[i] = act[i] ^ pred[i]

x = np.array(diff)

barprops = dict(aspect='auto', cmap='binary', interpolation='nearest')

fig = plt.figure()

ax2 = fig.add_axes([0.1, 0.375, 0.8, 0.125])
ax2.axes.get_yaxis().set_visible(False)
#ax2.imshow(x.reshape((10, -10)), **barprops)
ax2.imshow(x.reshape((1, -1)), **barprops)

plt.show()


# pred = act.copy()
# pred = pred[:100] + pred[101:] + [1]
# pred = pred[:1000] + pred[1001:] + [1]
# pred = pred[:1100] + pred[1101:] + [1]
# for i in range(STREAM_LENGTH):
#     diff[i] = act[i] ^ pred[i]

acc = (10000-diff.count(1))/10000
print("Accuracy: " + str(acc))
print("Wassersteind: " +str(wasserstein_distance(pred, act)))
import random

_tmp1 = []
for _tmp2 in range(10000):
    _x = [random.random() > 0.5 for _ in range(10000)]
    _y = [i if random.random() < acc else i^1 for i in _x]
    # _diff = [_x[i]^_y[i] for i in range(len(_x))]
    # print((10000 - sum(_diff))/10000)
    _tmp1 += [wasserstein_distance(_x, _y)]
print("Normal Distributed projected Wassersteind: " + str(np.mean(_tmp1)))

# for i in range(STREAM_LENGTH):
#     if diff[i] == 1:

#         pred_ = pred.copy()
#         diff_ = diff.copy()
#         pred_ = pred[:i] + pred[i+1:] + [1]
#         for j in range(i+1, STREAM_LENGTH):
#             diff_[j] = act[j] ^ pred_[j]

#         diff__ = diff.copy()
#         pred__ = pred[:i] + [act[i]] + pred[i:]
#         for j in range(i+1, STREAM_LENGTH):
#             diff__[j] = act[j] ^ pred__[j]

#         if sum(diff_) < min(sum(diff), sum(diff__)):
#             diff = diff_
#             pred = pred_
#             # insertion error
#             diff[i] = -1

#         if sum(diff__) < min(sum(diff), sum(diff_)):
#             diff = diff__
#             pred = pred__
#             # deletion error
#             diff[i] = -2

# print("Insetion Error: " + str(diff.count(-1)))
# print("Deletion Error: " + str(diff.count(-2)))
# print("Flip Error: " + str(diff.count(1)))

bins = [0] * 1000
counter = 0
mx = 0

for i in diff:
    if i == 1:
        counter +=1
    else:
        bins[counter] += 1
        mx = max(mx, counter)
        counter = 0

bins[0]=0
plt.bar(range(mx), bins[:mx])
plt.show()