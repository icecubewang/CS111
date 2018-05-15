#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 104796844

all: lab2_list

lab2_list: lab2_list.c SortedList.c SortedList.h
	gcc -O1 -Wall -Wextra -pthread -g -o lab2_list lab2_list.c SortedList.c SortedList.h

tests: clean all
	chmod +x gnuplotGenerate.sh
	-./gnuplotGenerate.sh

graphs: tests
	gnuplot lab2b_list.gp

profile: clean lab2_list
	-rm -f ./raw/gperf
	LD_PRELOAD=/u/eng/class/classfei/lib/libprofiler.so.0 CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.out
	pprof --list=listfunctionB ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf

dist: clean lab2_list graphs profile
	tar -czvf lab2b-104796844.tar.gz SortedList.h SortedList.c lab2_list.c Makefile *.csv *.out *.png README gnuplotGenerate.sh *.gp

clean:
	-rm -f lab2b-104796844.tar.gz lab2_list