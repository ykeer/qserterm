#!/bin/bash

rm -rf ./repo/*
cp *.pro ./repo
cp *.sh ./repo
cp -R -L src ./repo
cp -R -L languages ./repo
cp -R -L doc ./repo
cp -R -L static ./repo
cp -R -L db ./repo
