#! /bin/bash

cd build
cmake ..
make -j12 vm
cp compile_commands.json ..
