#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 104796844

#!/bin/bash
rm -f lab2b_list.csv

#lab2b_1.png (line 11 - line 25)
#lab2b_2.png (line 11 - line 17)

./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

#lab2b_3.png (line 31 - line 111)
#See the number of iterations that takes to reliably fail
#--iterations=1, 2, 4, 8, 16, 32

./lab2_list --threads=1 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=4 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=8 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=12 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=16 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

#sync=s
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

#sync=m
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

#lab2b_45.png (line 115 - line 133)
#sync=s
# ./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

# ./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

# ./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

# ./lab2_list --threads=16 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

#lab2b_41.png (line 137 - line 155)
#sync=m
# ./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv

# ./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv

# ./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv

# ./lab2_list --threads=16 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv