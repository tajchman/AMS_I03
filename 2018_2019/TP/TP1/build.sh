#! /bin/bash

DIR=`pwd`

module load intel
CC=icc
CXX=icpc
FC=gfortran

mkdir -p $DIR/build
cd $DIR/build
cmake -G "Eclipse CDT4 - Unix Makefiles" ../src
make