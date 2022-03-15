#!/bin/bash
opt --enable-new-pm=0 -load ./build/lib/libLocalOpts.so  -algebraic-identity -const-fold-opt -strength-reduction -S test/$1-mem2reg.ll > test/$1-mod.ll