/* CLIENT */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <termios.h>
#include <poll.h>
#include <errno.h>
#include <signal.h>
#include <ulimit.h>
#include <zlib.h>
#define h_addr h_addr_list[0] /* for backward compatibility */

// /* Start: Compression stuff */
z_stream stdin_to_shell;
z_stream shell_to_stdout;

char lf = '\n';

static struct termios terminal_old;
static struct termios terminal_new;

/* Signal handler */
void sighandler(int sig) {
	if (sig == SIGPIPE) {
		char *errmsg = "Error: Caught SIGPIPE with swignal number. \r\n";
		write(2, errmsg, strlen(errmsg));
		_exit(0);
	}
}

/* Reset Input mode */
void reset_input_mode(void) {
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal_old);	//Restore
	deflateEnd(&stdin_to_shell);
	inflateEnd(&shell_to_stdout);
}


/* Initialize a new terminal */
void init_terminal(void) {

	if (!isatty(0)) {
		fprintf(stderr, "Error: Not a terminal.\r\n");
		exit(1);
	}
	tcgetattr(STDIN_FILENO, &terminal_old);				//Obtain terminal settings (save current port setting)
	atexit(reset_input_mode);									//This is to prevent the program exits half way and didn't reset

	//bzero(&terminal_new, sizeof(terminal_new));					//Erase the data in terminal_new

	terminal_new.c_iflag = ISTRIP; 	/* only lower 7 bits */
	terminal_new.c_oflag = 0;		/* no processing */
	terminal_new.c_lflag = 0;		/* no processing */

	tcsetattr(STDIN_FILENO, TCSANOW, &terminal_new);	//TCSANOW: make the change immediately
}


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
				if (portValue <= 2048 || portValue >= 66535) {
					fprintf(stderr, "Port numbers should be between 2048 and 65535.\n");
					exit(1);
				}
				if (debug) {
					printf("Port ? %d, string = %d\r\n", port, portValue);
				}
				break;
			case 'l':
				log = true;
				if (strcmp("", optarg) == 0) {
					fprintf(stderr, "Please input a filename for log.\r\n");
					exit(1);
				}
				logString = optarg;
				if (debug) {
					printf("Log ? %d, string = %s\r\n", log, logString);
				}
				break;
			case'c':
				compress = true;
				/* Start: Compression stuff */
				// z_stream stdin_to_shell;
				// z_stream shell_to_stdout;

				stdin_to_shell.zalloc = Z_NULL;
				stdin_to_shell.zfree = Z_NULL;
				stdin_to_shell.opaque = Z_NULL;

				if (deflateInit(&stdin_to_shell, Z_DEFAULT_COMPRESSION) < 0) {
					fprintf(stderr, "Error during deflateInit.\r\n");
					exit(1);
				}

				shell_to_stdout.zalloc = Z_NULL;
				shell_to_stdout.zfree = Z_NULL;
				shell_to_stdout.opaque = Z_NULL;

				if (inflateInit(&shell_to_stdout) < 0) {
					fprintf(stderr, "Error during inflateInit.\r\n");
					exit(1);
				}
				/* End: Compression stuff */
				break;
			default:
				fprintf(stderr, "Usage: ./lab1b-client --port=[enter] --log=[enter] --compress\r\n");
				exit(1);
		}
	}
	if (!port) {
		fprintf(stderr, "--port= is mandatory.\r\n");
		exit(1);
	}
	/* End: Parse arguments */

	/* Start: Set up socket connection */
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "Error opening socket.\r\n");
		exit(1);
	}
	server = gethostbyname("localhost");
	if (server == NULL) {
		fprintf(stderr, "Error, no such host.\r\n");
		exit(1);
	}
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char*) &serv_addr.sin_addr.s_addr, (char*) server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portValue);
	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Error connecting.\r\n");
		exit(1);
	}
	/* End: Set up socket connection */

	/* Start: Set up non-canonical terminal */
	init_terminal();
	/* End: Set up non-canonical terminal */

	/* Start: Create/Open a log file */
	int log_fd;
	if (log) {
		log_fd = creat(logString, 0666);
		if (log_fd < 0) {
			fprintf(stderr, "Error creating log file.\r\n");
			exit(1);
		}
	}
	/* End: Create/Open a log file */

	/* Start: Construct poll */
	struct pollfd pfds[2];
	pfds[0].fd = STDIN_FILENO;
	pfds[0].events = POLLIN;
	pfds[1].fd = sockfd;
	pfds[1].events = POLLIN;
	/* End: Construct poll */

	while(1) {
		int p = poll(pfds, 2, -1);
		if(p < 0) {
			fprintf(stderr, "Error during poll.\r\n");
			exit(1);
		}
		if (p == 0) {
			continue;
		}

		char buffer[2048];
		int bytes_read = 0;

		//if (debug) printf("before polling 1...\r\n");
		/* Start: Send input from keyboard to socket & echo to display */
		if (pfds[0].revents & POLLIN) {
			memset(buffer, 0, sizeof(char) * 2048);
			bytes_read = read(STDIN_FILENO, &buffer, 2048);
			if (bytes_read < 0) {
				fprintf(stderr, "Error during read.\r\n");
				exit(1);
			}

			/* Write to logfile */
			if (log) {
				char message[256];
				if (sprintf(message, "SENT %d bytes: ", bytes_read) < 0) {
					fprintf(stderr, "Error during sprintf.\r\n");
					exit(1);
				}
				if (write(log_fd, message, strlen(message)) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
				if (write(log_fd, buffer, bytes_read) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
				if (write(log_fd, &lf, 1) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
			}

			/* Write to stdout(screen) */
			for (int i = 0; i < bytes_read; i++) {
				if (buffer[i] == '\r' || buffer[i] == '\n') {
					char* crlf = "\r\n";
					if (write(1, crlf, 2) < 0) {
						fprintf(stderr, "Error during write.\r\n");
						exit(1);
					}
				}
				else {
					if (write(1, &buffer[i], 1) < 0) {
						fprintf(stderr, "Error during write.\r\n");
						exit(1);
					}
				}
			}

			/* Porper translation of input CR into NL for shell input */
			for (int i = 0; i < bytes_read; i++) {
				if (buffer[i] == '\r') {
					buffer[i] = '\n';
				}
			}

			/* Compression */
			if (compress) {
				char compress_buffer[2048];
				memset(compress_buffer, 0, sizeof(char) * 2048);
				stdin_to_shell.avail_in = bytes_read;
				stdin_to_shell.next_in = (Bytef *) buffer;
				stdin_to_shell.avail_out = 2048;
				stdin_to_shell.next_out = (Bytef *) compress_buffer;
				do {
					deflate(&stdin_to_shell, Z_SYNC_FLUSH);
				} while (stdin_to_shell.avail_in > 0);
				write(sockfd, compress_buffer, 2048 - stdin_to_shell.avail_out);
			}
			else {
				for (int i = 0; i < bytes_read; i++) {
					if (write(sockfd, buffer, bytes_read) < 0) {
						fprintf(stderr, "Error during write.\r\n");
						exit(1);
					}
				}
			}
		}
		/* End: Send input from keyboard to socket & echo to display */

		//if (debug) printf("before polling 2...\r\n");
		/* Start: Read input from the socket and print to display */
		if (pfds[1].revents & POLLIN) {
			memset(buffer, 0, sizeof(char) * 2048);
			bytes_read = read(sockfd, &buffer, 2048);
			if (bytes_read < 0) {
				fprintf(stderr, "Error during read.\r\n");
				exit(1);
			}

			/* Write to logfile */
			if (log) {
				char message[256];
				if (sprintf(message, "RECEIVED %d bytes: ", bytes_read) < 0) {
					fprintf(stderr, "Error during sprintf.\r\n");
					exit(1);
				}
				if (write(log_fd, message, strlen(message)) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
				if (write(log_fd, buffer, bytes_read) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
				if (write(log_fd, &lf, 1) < 0) {
					fprintf(stderr, "Error during write.\r\n");
					exit(1);
				}
			}

			/* Decompression */
			int new_bytes_read = bytes_read;
			if (compress) {
				char decompress_buffer[2048];
				memset(decompress_buffer, 0, sizeof(char) * 2048);
				memcpy(decompress_buffer, buffer, bytes_read);
				memset(buffer, 0, 2048);

				shell_to_stdout.avail_in = bytes_read;
				shell_to_stdout.next_in = (Bytef *) decompress_buffer;
				shell_to_stdout.avail_out = 2048;
				shell_to_stdout.next_out = (Bytef *) buffer;

				do {
					inflate(&shell_to_stdout, Z_SYNC_FLUSH);
				} while (shell_to_stdout.avail_in > 0);
				new_bytes_read = 2048 - shell_to_stdout.avail_out;
			}

			/* Write to STDOUT (screen) + translate NL into CR NL */
			for (int i = 0; i < new_bytes_read; i++) {
				// if (buffer[i] == '\003' || buffer[i] == '\004') {
				// 	exit(1);
				// }
				if (buffer[i] == '\r' || buffer[i] == '\n') {
					char* crlf = "\r\n";
					if (write(1, crlf, 2) < 0) {
						fprintf(stderr, "Error during write.\r\n");
						exit(1);
					}
				}
				else {
					if (write(1, &buffer[i], 1) < 0) {
						fprintf(stderr, "Error during write.\r\n");
						exit(1);
					}
				}
			}

		}
		/* End: Read input from the socket and print to display */

		if ((pfds[0].revents & POLLHUP) || (pfds[0].revents & POLLERR)) {
			exit(1);
		}
		if ((pfds[1].revents & POLLHUP) || (pfds[1].revents & POLLERR)) {
			exit(1);
		}
	}
}