from sklearn import metrics
from scipy.stats import wasserstein_distance

print("Results for the Transmission:")

pred_file = open('pred.txt', 'r')
pred = pred_file.readlines()

act_file = open('act.txt', 'r')
act = act_file.readlines()

STREAM_LENGTH = len(pred)

datasets = len(pred)/len(act)

if not datasets.is_integer():
    print("ERROR: len(pred) is not multiple of len(act). (" + str(len(act)) + "-" + str(len(pred)) + ")")

act = act*int(datasets)

# print confusion matrix
print("\n", metrics.confusion_matrix(act, pred), "\n")
print(metrics.classification_report(act, pred))

# Wasserstein
print(f'Wasserstein-Distance: {wasserstein_distance(pred, act):.8f}')
