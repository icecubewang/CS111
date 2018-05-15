#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 

#!/bin/bash
rm -f lab2_add.csv lab2_list.csv

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
./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv

#add-s
./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv

#add-c
./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv

#add-yield-m
./lab2_add --threads=2 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv

#add-yield-s
./lab2_add --threads=1 --iterations=10000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield --sync=s >> lab2_add.csv

#add-yield-c
./lab2_add --threads=1 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv

#list-none-none
./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

#list-i-none
./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

#list-d-none
./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

#list-il-none
./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

#list-dl-none
./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv

#with sync=c
#list-i-s
./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
#list-d-s
./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
#list-l-s
./lab2_list --threads=12 --iterations=32 --yield=l --sync=s >> lab2_list.csv
#list-id-s
./lab2_list --threads=12 --iterations=32 --yield=id --sync=s >> lab2_list.csv
#list-il-s
./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
#list-dl-s
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
#list-idl-s
./lab2_list --threads=12 --iterations=32 --yield=idl --sync=s >> lab2_list.csv

#sync=m
#list-i-m
./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
#list-d-m
./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
#list-l-m
./lab2_list --threads=12 --iterations=32 --yield=l --sync=m >> lab2_list.csv
#list-id-m
./lab2_list --threads=12 --iterations=32 --yield=id --sync=m >> lab2_list.csv
#list-il-m
./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
#list-dl-m
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
#list-idl-m
./lab2_list --threads=12 --iterations=32 --yield=idl --sync=m >> lab2_list.csv

#list-none-s
./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

#list-none-c
./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
