#!/bin/bash

make clean
make
echo "**********************************"
echo "************** RUN ***************"
echo "**********************************"
./ifj13 tests/input.php
