#!/usr/bin/bash

qmake-qt5 -o Makefile trinitrotoluol.pro
make -j $(nproc)

if [ $? -eq 0 ]
then
    ./debug/trinitrotoluol
fi
