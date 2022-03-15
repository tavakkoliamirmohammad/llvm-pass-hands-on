#!/bin/bash
opt --enable-new-pm=0 -load ./build/lib/libFunctionInfo.so  -function-info -disable-output -S test/$1-mem2reg.ll > test/$1-mod.ll