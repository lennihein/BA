from sklearn import metrics
from scipy.stats import wasserstein_distance

print("\nResults for the Transmission:")

pred_file = open('pred.txt', 'r')
pred = pred_file.readlines()

act_file = open('act.txt', 'r')
act = act_file.readlines()

STREAM_LENGTH = len(pred)

if len(act) != STREAM_LENGTH:
    exit(1)

# print confusion matrix
print("\n", metrics.confusion_matrix(act, pred), "\n")
print(metrics.classification_report(act, pred))

# Wasserstein
print(f'Wasserstein-Distance: {wasserstein_distance(pred, act):.8f}')