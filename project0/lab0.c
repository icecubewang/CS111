/*
NAME: Feilan Wang
EMAIL: wangfeilan@hotmail.com
ID: 104796844
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

	//sighandler
	void sighandler(int signum) {
		if (signum == 11) {
			fprintf(stderr, "Catch Segfault error: %d, message: %s. \n", errno, strerror(errno));
			_exit(4);
		}
	}

int main(int argc, char *argv[]) {
	int c = 0;
	bool haveInput = false;
	bool haveOutput = false;
	bool haveSegfault = false; 
	bool haveCatch = false;
	char* inputPath;
	char* outputFilename;
	int fd0;
	int fd1;

	while (1) {
		static struct option long_options[] = {
			{"input",	required_argument,	0,	'i'},
			{"output",	required_argument,	0,	'o'},
			{"segfault",	no_argument,	0,	's'},
			{"catch",	no_argument,	0,	'c'},
			{0,	0,	0,	0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "sci:o:", long_options, &option_index);
		if (c == -1) break;
		switch(c) {
			case 0:
         		/* If this option set a flag, do nothing else now. */
        		if (long_options[option_index].flag != 0)
         			break;
          		printf ("option %s", long_options[option_index].name);
          		if (optarg)
            		printf (" with arg %s", optarg);
          		printf ("\n");
          		break;
          	case 'i':
          		haveInput = true;
          		// printf("Input path is : ");
          		// printf(optarg);
          		if (strcmp("", optarg) == 0) {
          			printf("Please input a filename.\n");
          			printf("Usage: ./lab0 --input=filename --output=filename --segfault --catch\n");
          			exit(1);
          		}
          		inputPath = optarg;
          		// fd0 = open(optarg, O_RDONLY);
          		// printf("%d\n", fd0);
          		break;
          	case 'o':
          		haveOutput = true;
          		outputFilename = optarg;
          		break;
          	case 's':
          		haveSegfault = true;
          		//if (!haveSegfault)
          			//printf("true\n");
          		break;
          	case 'c':
          		haveCatch = true;
          		//if (!haveCatch)
          			//printf("true\n");
          		//signal(SIGSEGV, sighandler);
          		break;
          	default:
          		printf("Please input a filename.\n");
          		printf("Usage: ./lab0 --input=filename --output=filename --segfault --catch\n");
          		exit(1);
		}
	}

	int inDup;
	int outDup;

	//handle input
	if (haveInput) {
		fd0 = open(inputPath, O_RDONLY);
		if (fd0 < 0) {
			//print error message
			fprintf(stderr, "Error with input file: %d, message: %s. Please enter a correct filename. \n", errno, strerror(errno));
			exit(2);
		}
		//printf("%d\n", fd0);
		inDup = dup2(fd0, 0);
		if (inDup < 0) {
			fprintf(stderr, "Error with dup2.\n");
			exit(-1);
		}
		close(fd0);
	}

	//handle output
	if (haveOutput) {
		fd1 = creat(outputFilename, 0666);
		if (fd1 < 0) {
			//print error message
			fprintf(stderr, "Error with output file: %d, message: %s. Please enter a correct filename. \n", errno, strerror(errno));
			exit(3);
		}
		//printf("%d\n", fd1);
		//printf("%s\n", outputFilename);
		outDup = dup2(fd1, 1);
		if (outDup < 0) {
			fprintf(stderr, "Error with dup2.\n");
			exit(-1);
		}
		close(fd1);
	}

	//catch
	if (haveCatch) {
		if (SIG_ERR == signal(11, sighandler)) {
			fprintf(stderr, "Signal Error encountered. \n");
			exit(-1);
		}
		fprintf(stderr, "Segmentation Fault.\n");
		exit(4);
	}

	//handle segmentation fault
	if (haveSegfault) {
		int* trouble = NULL;
		*trouble = 1;
	}

	//read & write
	unsigned char buffer[4096];
	int bytes_read;
	int bytes_write;
	do {
		bytes_read = read(0, buffer, 4096);
		bytes_write = write(1, buffer, bytes_read);
		if (bytes_read != bytes_write) {
			printf("Output not equal to input. \n");
			exit(-1);
		}
	}
	while (bytes_read != 0);
	exit(0);
}