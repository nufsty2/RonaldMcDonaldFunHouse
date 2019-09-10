#!/bin/bash
echo "PYNQ ##:"
read pynqnum
rm -r -f ~/pynq_mnt
mkdir ~/pynq_mnt
sshfs xilinx@pynq$pynqnum.ee.byu.edu:/home/xilinx ~/pynq_mnt
ssh "xilinx@pynq$pynqnum.ee.byu.edu"
cd ECEN427
