#!/bin/bash

rm -rf ./dist/*
mkdir ./dist/qserterm

./build_lin.sh
cp -R -L ./build/lin/* ./dist/qserterm
cd ./dist; zip -r qserterm-lin-`date +%Y%m%d-%H%M%S`.zip qserterm
cd ..
rm -rf ./dist/qserterm/*

cp -R -L ./dll_vc2013/* ./dist/qserterm
cp -R -L ./build/lin/* ./dist/qserterm
rm ./dist/qserterm/qserterm
mv ./dist/qserterm/uni.exe ./dist/qserterm/qserterm.exe
cd ./dist; zip -r qserterm-win-`date +%Y%m%d-%H%M%S`.zip qserterm
cd ..
rm -rf ./dist/qserterm

