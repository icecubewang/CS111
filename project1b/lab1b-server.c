/*
NAME: Feilan Wang
EMAIL: wangfeilan@hotmail.com
ID: 
*/

/* SERVER */
#define _POSIX_SOURCE
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
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <zlib.h>

int pipe_in[2];
int pipe_out[2];
char buffer[2048];
int status = 0;
pid_t pid;
bool bogus = false;
bool compressFlag = false;

z_stream shell_to_client;
z_stream client_to_shell;

/* Signal handler */
void sighandler(int sig) {
	if (sig == SIGPIPE) {
		char *errmsg = "Error: Caught SIGPIPE with signal number. \r\n";
		if (write(2, errmsg, strlen(errmsg)) < 0) {
			fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
			exit(1);
		}
		_exit(0);
	}
}

/* Reset function */
void reset(void) {
	if (compressFlag) {
		deflateEnd(&shell_to_client);
		inflateEnd(&client_to_shell);
	}
	//Do last read from shell
	if (!bogus) {
		int bytes_read = 0;
		bytes_read = read(pipe_in[0], &buffer, 2048);
		if (bytes_read < 0) {
			fprintf(stderr, "Error during read. %s\r\n", strerror(errno));
			exit(1);
		}
		for (int i = 0; i < bytes_read; i++) {
			if (buffer[i] == '\n') {
				char* crlf = "\r\n";
				if (write(1, crlf, 2) < 0) {
					fprintf(stderr, "Error during read. %s\r\n", strerror(errno));
					exit(1);
				}
			}
			else {
				if (write(1, &buffer[i], 1) < 0) {
					fprintf(stderr, "Error during read. %s\r\n", strerror(errno));
					exit(1);
				}
			}
		}
		//Message
		if (waitpid(pid, &status, 0) < 0) {
			fprintf(stderr, "Error: with waitpid. %s\r\n", strerror(errno));
			exit(1);
		}
		int p_status = 0;
		int p_signal = 0;

		if (WIFEXITED(status)) {
			p_status = WEXITSTATUS(status);
		}
		if (WIFSIGNALED(status)) {
			p_signal = WTERMSIG(status);
		}
		fprintf(stderr, "SHELL EXIT SIGNAL=%d, STATUS=%d\r\n", p_signal, p_status);
	}
}

int main(int argc, char* argv[]) {
	/* Call reset before exit */
	atexit(reset);
	/* Start: Parse arguments */
	bool port = false;
	int portValue;
	bool debug = false;
	int c;
	struct option long_options[] = {
		{"port",		required_argument,	0,	'p'},
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
					fprintf(stderr, "Port numbers should be between 2048 and 65535.\r\n");
					exit(1);
				}
				if (debug) {
					printf("Port ? %d, string = %d\n", port, portValue);
				}
				break;
			case'c':
				compressFlag = true;
				/* Start: Compression stuff */

				shell_to_client.zalloc = Z_NULL;
				shell_to_client.zfree = Z_NULL;
				shell_to_client.opaque = Z_NULL;

				if (deflateInit(&shell_to_client, Z_DEFAULT_COMPRESSION) < 0) {
					fprintf(stderr, "Error during deflateInit. %s\r\n", strerror(errno));
					exit(1);
				}

				client_to_shell.zalloc = Z_NULL;
				client_to_shell.zfree = Z_NULL;
				client_to_shell.opaque = Z_NULL;

				if (inflateInit(&client_to_shell) < 0) {
					fprintf(stderr, "Error during inflateInit. %s\r\n", strerror(errno));
					exit(1);
				}
				/* End: Compression stuff */
				break;
			default:
				fprintf(stderr, "Usage: ./lab1b-server --port=[enter] --compress\r\n");
				bogus = true;
				exit(1);
		}
	}
	if (!port) {
		fprintf(stderr, "--port= is mandatory.\n");
		exit(1);
	}
	/* End: Parse arguments */

	/* Start: Insert signal handler */
	if (signal(SIGPIPE, sighandler) == SIG_ERR) {
		fprintf(stderr, "Error with signal.\r\n");
		exit(1);
	}
	/* End: Insert signal handler */

	/* Start: Set up socket connection */
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "Error opending socket. %s\r\n", strerror(errno));
		exit(1);
	}
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portValue);
	if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Error on binding. %s\r\n", strerror(errno));
		exit(1);
	}
	listen(sockfd, 1);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
	if (newsockfd < 0) {
		fprintf(stderr, "Error on accept. %s\r\n", strerror(errno));
		exit(1);
	}
	if (debug) printf("Connected.\r\n");
	/* End: Set up socket connection */

	/* Start: Pipe construction */
	if (pipe(pipe_in) == -1 || (pipe(pipe_out) == -1)) {
		fprintf(stderr, "Error building pipes. %s\r\n", strerror(errno));
		exit(1);
	}
	/* End: Pipe construction */

	/* Start: Fork a child process */
	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Error during fork. %s\r\n", strerror(errno));
		exit(1);
	}
	/* End: Fork a child process */

	/* Start: Child process */
	else if (pid == 0) {
		/* Start: Pipe handling in child process */
		if (close(pipe_in[0]) < 0) {
			fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
			exit(1);
		}
		if (close(pipe_out[1]) < 0) {
			fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
			exit(1);
		}
		if (dup2(pipe_out[0], STDIN_FILENO) < 0) {
			fprintf(stderr, "Error: dup2. %s\r\n", strerror(errno));
			exit(1);
		}
		if (dup2(pipe_in[1], STDOUT_FILENO) < 0) {
			fprintf(stderr, "Error: dup2. %s\r\n", strerror(errno));
			exit(1);
		}
		if (dup2(pipe_in[1], STDERR_FILENO) < 0) {
			fprintf(stderr, "Error: dup2. %s\r\n", strerror(errno));
			exit(1);
		}
		if (close(pipe_in[1]) < 0) {
			fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
			exit(1);
		}
		if (close(pipe_out[0]) < 0) {
			fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
			exit(1);
		}
		/* End: Pipe handling in child process */

		/* Start: Run exec() */
		char* execvp_argv[2];
		char execvp_filename[] = "/bin/bash";
		execvp_argv[0] = execvp_filename;
		execvp_argv[1] = NULL;
		if (execvp(execvp_filename, execvp_argv) < 0) {
			fprintf(stderr, "Error: Exec /bin/bash. %s\r\n", strerror(errno));
			exit(1);
		}
		/* End: Run exec() */
	}
	/* End: Child process */

	/* Start: Parent process */
	else {
		/* Start: Pipe handling in parent process */
		if (close(pipe_in[1]) < 0) {
			fprintf(stderr, "Error: Close pipe_in[1]. %s\r\n", strerror(errno));
			exit(1);
		}
		if (close(pipe_out[0]) < 0) {
			fprintf(stderr, "Error: Close pipe_out[0]. %s\r\n", strerror(errno));
			exit(1);
		}
		/* End: Pipe handling in parent process */

		/* Start: Construct poll */
		struct pollfd pfds[2];
		pfds[0].fd = newsockfd;
		pfds[0].events = POLLIN;
		pfds[1].fd = pipe_in[0];
		pfds[1].events = POLLIN;
		/* End: Construct poll */

		while(1) {
			int p = poll(pfds, 2, -1);
			if (p < 0) {
				fprintf(stderr, "Error during poll. %s\r\n", strerror(errno));
				exit(1);
			}
			if (p == 0) {
				continue;
			}

			char buffer[2048];
			int bytes_read = 0;

			/* Start: Send input from socket to shell */
			if (pfds[0].revents & POLLIN) {
				bytes_read = read(newsockfd, &buffer, 2048);
				if (bytes_read < 0) {
					fprintf(stderr, "Error during read. %s\r\n", strerror(errno));
					exit(1);
				}

				/* Decompression */
				int new_bytes_read = bytes_read;
				if (compressFlag) {
					char decompress_buffer[2048];
					memset(decompress_buffer, 0, sizeof(char) * 2048);
					memcpy(decompress_buffer, buffer, bytes_read);
					memset(buffer, 0, 2048);
					client_to_shell.avail_in = bytes_read;
					client_to_shell.next_in = (Bytef *) decompress_buffer;
					client_to_shell.avail_out = 2048;
					client_to_shell.next_out = (Bytef *) buffer;
					do {
						if (inflate(&client_to_shell, Z_SYNC_FLUSH) < 0) {
							fprintf(stderr, "Error during inflate. %s\r\n", strerror(errno));
							exit(1);
						}
					} while (client_to_shell.avail_in > 0);
					new_bytes_read = 2048 - client_to_shell.avail_out;
				}

				for (int i = 0; i < new_bytes_read; i++) {
					if (buffer[i] == '\004') {
						if (close(pipe_out[1]) < 0) {
							fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
							exit(1);
						}
						exit(0);
					}
					else if (buffer[i] == '\003') {
						if (kill(pid, SIGINT) < 0) {
							fprintf(stderr, "Error during kill. %s\r\n", strerror(errno));
							exit(1);
						}
						if (write(pipe_out[1], "\n", 1) < 0) {
							fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
							exit(1);
						}
					}
					else if (buffer[i] == '\r' || buffer[i] == '\n') {
						char lf = '\n';
						if (write(pipe_out[1], &lf, 1) < 0) {
							fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
							exit(1);
						}
					}
					else {
						if (write(pipe_out[1], &buffer[i], 1) < 0) {
							fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
							exit(1);
						}
					}
				}
			}
			/* End: Send input to shell */

			/* Start: Send input from shell to socket */
			if (pfds[1].revents & POLLIN) {
				bytes_read = read(pipe_in[0], &buffer, 2048);
				if (bytes_read < 0) {
					fprintf(stderr, "Error during read. %s\r\n", strerror(errno));
					exit(1);
				}

				/* Compression */
				if (compressFlag) {
					char compress_buffer[2048];
					memset(compress_buffer, 0, sizeof(char) * 2048);
					shell_to_client.avail_in = bytes_read;
					shell_to_client.next_in = (Bytef *) buffer;
					shell_to_client.avail_out = 2048;
					shell_to_client.next_out = (Bytef *) compress_buffer;
					do {
						if (deflate(&shell_to_client, Z_SYNC_FLUSH) < 0) {
							fprintf(stderr, "Error during deflate. %s\r\n", strerror(errno));
							exit(1);
						}
					} while (shell_to_client.avail_in > 0);
					if (write(newsockfd, compress_buffer, 2048 - shell_to_client.avail_out) < 0) {
						fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
						exit(1);
					}
				}
				else {
					for (int i = 0; i < bytes_read; i++) {
						if (write(newsockfd, &buffer[i], 1) < 0) {
							fprintf(stderr, "Error during write. %s\r\n", strerror(errno));
							exit(1);
						}
					}
				}
			}
			/* End: Send input from shell to socket */

			/* Start: POLLHUP & POLLERR case */
			if ((pfds[0].revents & POLLHUP) || (pfds[0].revents & POLLERR)) {
				if (close(pipe_out[1]) < 0) {
					fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
					exit(1);
				}
				exit(0);
			}
			if ((pfds[1].revents & POLLHUP) || (pfds[1].revents & POLLERR)) {
				if (close(pipe_out[1]) < 0) {
					fprintf(stderr, "Error closing pipe. %s\r\n", strerror(errno));
					exit(1);
				}
				exit(0);
			}
			/* End: POLLHUP & POLLERR case */
		}
	}
}
