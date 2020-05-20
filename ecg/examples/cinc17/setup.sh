#!/bin/bash

#url=https://www.physionet.org/challenge/2017/
url=https://archive.physionet.org/challenge/2017/

mkdir data && cd data

curl -O $url/training2017.zip
unzip training2017.zip
curl -O $url/sample2017.zip
unzip sample2017.zip
curl -O $url/REFERENCE-v3.csv

cd ..

python ~/ecg/examples/cinc17/build_datasets.py
