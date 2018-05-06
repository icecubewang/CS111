#!/bin/bash

#add-none
./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv

./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv

./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv

./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv

./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv

#add-yield-none
./lab2_add --yield --threads=1 --iterations=10 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=20 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=40 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=80 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=1000 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=10000 >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100000 >> lab2_add.csv

./lab2_add --yield --threads=2 --iterations=10 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=20 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=40 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=80 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100000 >> lab2_add.csv

./lab2_add --yield --threads=4 --iterations=10 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=20 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=40 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=80 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=1000 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=10000 >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100000 >> lab2_add.csv

./lab2_add --yield --threads=8 --iterations=10 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=20 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=40 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=80 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100000 >> lab2_add.csv

./lab2_add --yield --threads=12 --iterations=10 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=20 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=40 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=80 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=1000 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=10000 >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100000 >> lab2_add.csv

#add-m
./lab2_add --threads=1 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --threads=1 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=1 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --threads=2 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=2 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --threads=4 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --threads=8 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --threads=12 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=100000 --sync=m >> lab2_add.csv

#add-s
./lab2_add --threads=1 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --threads=1 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=1 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --threads=2 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --threads=4 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --threads=8 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --threads=12 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=100000 --sync=s >> lab2_add.csv

#add-c
./lab2_add --threads=1 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --threads=1 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=1 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --threads=2 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --threads=4 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --threads=8 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --threads=12 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=100000 --sync=c >> lab2_add.csv

#add-yield-m
./lab2_add --yield --threads=1 --iterations=10 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=20 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=40 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=80 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --yield --threads=2 --iterations=10 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=20 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=40 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=80 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --yield --threads=4 --iterations=10 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=20 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=40 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=80 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --yield --threads=8 --iterations=10 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=20 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=40 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=80 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100000 --sync=m >> lab2_add.csv

./lab2_add --yield --threads=12 --iterations=10 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=20 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=40 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=80 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=1000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100000 --sync=m >> lab2_add.csv

#add-yield-s
./lab2_add --yield --threads=1 --iterations=10 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=20 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=40 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=80 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --yield --threads=2 --iterations=10 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=20 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=40 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=80 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --yield --threads=4 --iterations=10 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=20 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=40 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=80 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --yield --threads=8 --iterations=10 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=20 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=40 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=80 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100000 --sync=s >> lab2_add.csv

./lab2_add --yield --threads=12 --iterations=10 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=20 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=40 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=80 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=1000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100000 --sync=s >> lab2_add.csv

#add-yield-c
./lab2_add --yield --threads=1 --iterations=10 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=20 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=40 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=80 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=1 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --yield --threads=2 --iterations=10 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=20 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=40 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=80 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=2 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --yield --threads=4 --iterations=10 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=20 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=40 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=80 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=4 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --yield --threads=8 --iterations=10 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=20 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=40 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=80 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=8 --iterations=100000 --sync=c >> lab2_add.csv

./lab2_add --yield --threads=12 --iterations=10 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=20 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=40 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=80 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=1000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --yield --threads=12 --iterations=100000 --sync=c >> lab2_add.csv