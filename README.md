# BA

## Prerequisites on Arch

as root:

```bash
sudo pacman -Syu python python-pip gcc cmake make
pip install numpy matplotlib sklearn
```

## Prerequisites on Ubuntu

as root:

```bash
apt update
apt install cmake python3-pip
pip3 install numpy matplotlib sklearn
```

## Instructions: compilation
```bash
cmake .
make
```

## Instructions: finding the threshhold

```bash
taskset 0x1 ./meassure_send [FREQUENCY] &
taskset 0x1 ./meassure_recv [FREQUENCY] [TESTPOINTS]
pkill meassure_send
# 'python3' on Ubuntu Versions < 20
python evaluate_threshholds.py
```

## Instructions: transmitting data

```bash
taskset 1 ./sender [FREQUENCY] [PACKETLENGTH] &
taskset 1 ./receiver [-ff/-fr] [THRESHHOLD] [FREQUENCY] [PACKETLENGTH]
pkill sender
# 'python3' on Ubuntu Versions < 20
python evaluate_transmission.py
```
