#!/bin/bash
# Ask the user for pynq number
echo "PYNQ ##:"
read pynqnum
ssh "xilinx@pynq$pynqnum.ee.byu.edu"
