#!/bin/bash
echo "Now pull from github and build SRB"
cd ~/srb_ws/SRB-Frame-lib
git pull
cd ./build
cmake ~/srb_ws/SRB-Frame-lib
make
