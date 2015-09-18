#!/bin/bash

qmake project.pro -config lin && make all install
# && rm -r ./build/lin/.moc ./build/lin/.obj
