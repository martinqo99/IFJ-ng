#!/bin/bash

make clean
make
echo "**********************************"
echo "************* RUN 1 **************"
echo "**********************************"
./ifj13 tests/input.php
echo "**********************************"
echo "************* RUN 2 **************"
echo "**********************************"
./ifj13 tests/input2.php
echo "**********************************"
echo "************* RUN 3 **************"
echo "**********************************"
./ifj13 tests/input3.php
