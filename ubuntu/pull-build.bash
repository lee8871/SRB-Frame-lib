#!/bin/bash
echo "Now pull from github and build SRB"
cd ~/srb_ws/srb-frame-lib
git pull
cd ./build
cmake -GNinja ~/srb_ws/srb-frame-lib
ninja
