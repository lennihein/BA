# BA

## Prerequisites on Arch

as root:

```bash
sudo pacman -Syu python python-pip gcc cmake make
pip install numpy scipy matplotlib sklearn
```

## Prerequisites on Ubuntu

as root:

```bash
apt update
apt install cmake python3-pip
pip3 install numpy scipy matplotlib sklearn
```

## Instructions for prototype

```bash
cmake .
make
./threshholds
# 'python3' on Ubuntu
python evaluate_threshholds
# edit threshholds in poc_sync.c
make
./poc_sync
# 'python3' on Ubuntu
python evaluate_transmission
```
