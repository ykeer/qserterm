#!/bin/bash

qmake qserterm.pro -config win && sed -i -e 's/-lGL//g' Makefile && make clean all install && rm -r ./build/win/.moc ./build/win/.obj && mv ./build/win/qserterm ./build/win/qserterm.exe
