#!/bin/bash
clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c test/$1.c -o test/$1.bc
llvm-dis test/$1.bc
opt -mem2reg -S test/$1.ll -o test/$1-mem2reg.ll