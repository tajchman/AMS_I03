#! /bin/bash

mkdir -p build
cd build
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../src
make
cd -

