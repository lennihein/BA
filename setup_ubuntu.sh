apt update
apt install cmake python3-pip -y
pip3 install numpy matplotlib sklearn
cmake .
make
taskset 0x1 ./meassure_send 1000 &
taskset 0x1 ./meassure_recv 1000 10000
pkill meassure_send
python evaluate_threshholds.py