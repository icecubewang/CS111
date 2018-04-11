#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 

.SILENT:clean
.SILENT:check
CC = gcc
CFLAGS = -Wall -Wextra -g

all: lab0

lab0: lab0.c
	${CC} ${CFLAGS} -o $@ $^

check: lab0
	#1. See if they detect and report invalid arguments
	@./lab0 --bogus &>/dev/null; \
	if [ $$? -eq 1 ]; then \
		echo "TEST1 PASSED: Able to detect and report invalid arguments."; \
	else \
		echo "TEST1 FAILED: Unable to detect and report invalid arguments."; \
	fi \

	#2. See if they detect and report non-existent file
	#@echo "input for test 2" > test2Input.txt
	@./lab0 --input=non-existent-file &>/dev/null; \
	if [ $$? -eq 2 ]; then \
		echo "TEST2 PASSED: Able to detect and report non-existent file."; \
	else \
		echo "TEST2 FAILED: Unable to detect and report non-existent file."; \
	fi \

	#3. See if it is able to read from stdin and write to stdout
	@echo "input for test3" > test3input;
	@touch test3output;
	@./lab0 < test3input > test3output; \
	if [ $$? -eq 0 ]; then \
		echo "TEST3 PASSED: Able to read from stdin and write to stdout."; \
	else \
		echo "TEST3 FAILED: Unable to read from stdin and write to stdout."; \
	fi \

	#4. See if the output file has correct content
	@echo "input for test4" > test4input;
	@touch test4output;
	@./lab0 < test4input > test4output;
	@cmp -s test4input test4output; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo "TEST4 PASSED: Correct content for stdin and stdout."; \
	else \
		echo "TEST4 FAILED: Different files for stdin and stdout."; \
	fi \

	#5. See if able to read from --input=filename and write to --output=filename
	@echo "input for test5" > test5input;
	@touch test5output;
	@./lab0 --input=test5input --output=test5output;
	@cmp -s test5input test5output; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo "TEST5 PASSED: Correct content for --input and --output."; \
	else \
		echo "TEST5 FAILED: Different files for --input and --output."; \
	fi \

	#6. See if able to cause segfault
	@rm -f test*;
	@./lab0 --segfault &>/dev/null; \
	if [ $$? -eq 139 ]; then \
		echo "TEST6 PASSED: Able to cause segfault."; \
	else \
		echo "TEST6 FAILED: Unable to cause segfault."; \
	fi \

	#7. See if able to catch segfualt
	@./lab0 --catch --segfault &>/dev/null; \
	if [ $$? -eq 4 ]; then \
		echo "TEST7 PASSED: Able to catch segfault."; \
	else \
		echo "TEST7 FAILED: Unable to catch segfault."; \
	fi \


dist: 
	tar -czvf lab0-XXXXXXXXX.tar.gz lab0.c Makefile README backtrace.png breakpoint.png

#This prevents make from getting confused by an actual file called clean
#and causes it to continue in spite of errors from rm.
.PHONY : clean
clean:
	-rm -f test* lab0 lab0-XXXXXXXXX.tar.gz
	#'-' means ignores the return status
