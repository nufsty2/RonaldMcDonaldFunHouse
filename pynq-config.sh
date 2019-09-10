#!/bin/bash
echo "PYNQ ##:"
read pynqnum
umount ~/pynq_mnt
rmdir ~/pynq_mnt
mkdir ~/pynq_mnt
sshfs xilinx@pynq$pynqnum.ee.byu.edu:/home/xilinx ~/pynq_mnt
ssh "xilinx@pynq$pynqnum.ee.byu.edu"
cd ECEN427
