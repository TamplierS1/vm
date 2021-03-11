# Simple virtual machine that can execute LC-3 instructions. 

**Vm** can parse and execute programs written in [LC-3](https://www.cs.utexas.edu/users/fussell/cs310h/lectures/Lecture_10-310h.pdf) assembly language.

## Dependencies

- [Cmake](https://gitlab.kitware.com/cmake/cmake) - v3.17.0
- Make - use you package manager to install it
- [Catch2](https://github.com/catchorg/Catch2) - light-weight unit test library
- [Clang++](https://github.com/llvm/llvm-project) - c++ compiler

Right now only Linux is supported.

## Build

To build the app itself run this from project root directory:  
1. `mkdir build`
2. `chmod +x build.sh`
3. `./build.sh`

To build the tests run this (from project root):
1. `mkdir build`
2. `cd tests`
3. `chmod +x build_tests.sh`
4. `./build_tests.sh`

All the binaries are located in `build/src/` directory (`vm` and `vm_tests`).

## License

Read [LICENCE](https://github.com/TamplierS1/vm/blob/master/LICENSE)