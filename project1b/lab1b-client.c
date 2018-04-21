/* CLIENT */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char* argv[]) {
	/* Start: Parse arguments */
	bool port = false;
	int portValue;
	bool log = false;
	char* logString;
	bool compress = false;
	bool debug = false;
	int c;
	struct option long_options[] = {
		{"port",		required_argument,	0,	'p'},
		{"log",			required_argument,	0,	'l'},
		{"compress",	no_argument,		0,	'c'},
		{"debug",		no_argument,		0,	'd'},
		{0,	0,	0,	0}
	};
	int option_index = 0;
	while((c = getopt_long(argc, argv, "cp:l:", long_options, &option_index)) != -1) {
		switch(c) {
			case 'd':
				debug = true;
				break;
			case 'p':
				port = true;
				portValue = atoi(optarg);
				if (portValue <= 1024) {
					fprintf(stderr, "Port numbers below 1024 are reserved.\n");
					exit(1);
				}
				if (debug) {
					printf("Port ? %d, string = %d\n", port, portValue);
				}
				break;
			case 'l':
				log = true;
				if (strcmp("", optarg) == 0) {
					fprintf(stderr, "Please input a filename for log.\n");
					exit(1);
				}
				logString = optarg;
				if (debug) {
					printf("Log ? %d, string = %s\n", log, logString);
				}
				break;
			case'c':
				compress = true;
				break;
			default:
				fprintf(stderr, "Usage: ./lab1b-client --port=[enter] --log=[enter] --compress\n");
				exit(1);
		}
	}
	if (!port) {
		fprintf(stderr, "--port= is mandatory.\n");
		exit(1);
	}
	/* End: Parse arguments */
}