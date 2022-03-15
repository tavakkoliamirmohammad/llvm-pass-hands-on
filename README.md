# LLVM Optimization Pass

This repository is based on [CSCD70 Compiler Optimization](https://uoft-ecosystem.github.io/CSCD70/index.html) course
offered at University of Toronto.

Implementation of some local optimizations with LLVM compiler infrastructure. The local optimizations are as follows:

- Algebraic Simplification
- Strength Reduction
- Constant Folding

## Algebraic Simplification

Binary operation that one of their operands are values that act as an identity element in that operation are simplified and the instruction is removed from basic block. 


## Strength Reduction

In this optimization multiplying and dividing by a power of 2 would be converted to left shift and arithmetic shift,
respectively.

## Constant Folding

The constant folding optimization is implemented for both float and integer constant for binary operations such as Add,
Sub, Div and Mul.
