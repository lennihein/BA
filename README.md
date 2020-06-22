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

## Instructions for prototype

```bash
cmake .
make
taskset 0x1 ./meassure_send &
taskset 0x1 ./meassure_recv
pkill meassure_send
# 'python3' on Ubuntu Versions < 20
python evaluate_threshholds
# edit threshholds in receiver.c
make
taskset 0x1 ./sender &
# choose a reasonable THRESHHOLD
taskset 0x1 ./receiver [THRESHHOLD]
pkill sender
# 'python3' on Ubuntu Versions < 20
python evaluate_transmission
```
