# Versuchsaufbau:

## Threshhold bestimmen:

10 000 Datenpunkte -> GroupA.txt & GroupB.txt

```
taskset 0x1 ./meassure_send [FREQUENCY] &
taskset 0x1 ./meassure_recv [FREQUENCY] 10000
pkill meassure_send
```

## Bits übertragen:

10x 1000Bits -> pred.txt & act.txt

```
taskset 1 ./sender [FREQUENCY] 1000 &
taskset 1 ./receiver [-ff/-fr] [THRESHHOLD] [FREQUENCY] 1000
# ... repeat 9 more times
pkill sender
```