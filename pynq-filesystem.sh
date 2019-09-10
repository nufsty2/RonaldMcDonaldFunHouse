#!/bin/bash
echo "PYNQ ##:"
read pynqnum
rm -r ~/pynq_mnt
mkdir ~/pynq_mnt
sshfs xilinx@pynq$pynqnum.ee.byu.edu:/home/xilinx ~/pynq_mnt

