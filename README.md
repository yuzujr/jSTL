# jSTL

A minimal, educational implementation of selected C++17 standard library components.

> **Note**: This project uses built-in Clang functions and must be compiled with Clang.

## build
First, make sure you have installed `clang`.

Then, you can build the project using either

`cmake`:
```
cmake -S . -B build \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

or `xmake`:
```bash
xmake
```

## test
```
./build/test_*
```

## Reference
1. [libc++](https://libcxx.llvm.org/)
2. [cppreference](https://en.cppreference.com/)