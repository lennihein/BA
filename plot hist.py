# how do I do python??

from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as pp

with open('threshholds_out.txt', 'r') as f:
    lines = f.readlines()[0:]
    x = [int(line.split()[0][0:-1]) for line in lines]
    y = [int(line.split()[1]) for line in lines]
    z = [int(line.split()[2]) for line in lines]

total = sum(y)
if total != sum(z):
    print('wtf')
y = [i / total for i in y]
z = [i / total for i in z]

glob_min = -1
glob_max = -1

for i in range(0, 600):
    if (y[i]) > 0 or z[i] > 0:
        if glob_min == -1:
            glob_min = i
    else:
        if y[i - 1] > 0.01 or z[i - 1] > 0.01:
            glob_max = i - 1

fig = pp.figure()
ax1 = fig.add_subplot(111)
ax1.set_title('Cycle duration based on cache hits and misses')
ax1.set_xlabel('Cycles')
ax1.set_ylabel('Number of meassurements')

# ax1.set_yscale('log')


ax1.bar(range(glob_min, glob_max + 1), height=y[glob_min:glob_max + 1], alpha=0.5, label='Hits')
ax1.bar(range(glob_min, glob_max + 1), height=z[glob_min:glob_max + 1], alpha=0.5, label='Misses')

leg1 = ax1.legend()

ax1.yaxis.set_major_formatter(FuncFormatter(lambda y, _: '{:.0%}'.format(y)))

pp.show()

print(glob_min)
print(glob_max)
