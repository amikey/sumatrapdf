#!/bin/bash

# temporary script for building parts of code on mac
# there are a lot of changes needed so we go bit-by-bit,
# starting with the simplest, lowest-level code

mkdir -p obj-mac
clang++ -std=c++11 -g src/utils/BaseUtil.cpp -o obj-mac/BaseUtil.o
 