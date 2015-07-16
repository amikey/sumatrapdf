#!/bin/bash

set -o nounset
set -o errexit
set -o pipefail

# temporary script for building parts of code on mac
# there are a lot of changes needed so we go bit-by-bit,
# starting with the simplest, lowest-level code

function build() {
	DST_DIR=$1
	rm -rf $DST_DIR
	mkdir -p $DST_DIR
	clang++ -std=c++11 -g -c src/utils/BaseUtil.cpp -Isrc/utils -o $DST_DIR/BaseUtil.o
	clang++ -std=c++11 -g -c src/utils/StrUtil.cpp -Isrc/utils -o $DST_DIR/StrUtil.o
	clang++ -std=c++11 -g -c src/mac/main.cpp -Isrc/utils -o $DST_DIR/mac.o
	clang++ -g $DST_DIR/*.o -o $DST_DIR/mac_main
	./$DST_DIR/mac_main
	ls -lah ./$DST_DIR
	rm -rf $DST_DIR
}

build rel-mac

# TODO: also need to change compilation flags, like DEBUG=1
#build dbg-mac
