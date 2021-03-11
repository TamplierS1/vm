#! /bin/bash

cd ../build
cmake ..
make -j12 vm_tests
cp compile_commands.json ..
