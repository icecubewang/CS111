#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/poll.h>
#include <sys/wait.h>

static struct termios terminal_old;
static struct termios terminal_new;
//Put keyboard (the file open on fd0) into terminal
int terminal_descriptor = 0;
bool debug = false;
bool shell = false;
pid_t pid;	//child's pid
int status = 0;

//In non-canonical mode, input is not assembled into lines and input processing does not occur.
//c_cc[VTIME] (character timer) and c_cc[VMIN] (minimum number of characteres to receive before satsfying the read) controls the behaviour of this mode.

/* Reset Input mode */
void reset_input_mode(void) {
	tcsetattr(terminal_descriptor, TCSANOW, &terminal_old);	//Restore
	if (shell) {
		waitpid(pid, &status, 0);
		fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
	}
}

/* Initialize a new terminal */
void init_terminal(void) {
	tcgetattr(terminal_descriptor, &terminal_old);				//Obtain terminal settings (save current port setting)
	atexit(reset_input_mode);									//This is to prevent the program exits half way and didn't reset

	bzero(&terminal_new, sizeof(terminal_new));					//Erase the data in terminal_new

	terminal_new.c_iflag = ISTRIP; 	/* only lower 7 bits */
	terminal_new.c_oflag = 0;		/* no processing */
	terminal_new.c_lflag = 0;		/* no processing */

	terminal_new.c_cc[VTIME] = 0;	/* inter-character timer unused */
	terminal_new.c_cc[VMIN] = 1;	/* one character at a time */

	tcflush(terminal_descriptor, TCIFLUSH);						//tcflush() discards data written to the object referred to by fd but not transmitted,
																//or data received but not read, depending on the value of queue_selector
	tcsetattr(terminal_descriptor, TCSANOW, &terminal_new);	//TCSANOW: make the change immediately
}

/* Signal Handler */
void sighandler(int signal) {
	if (signal == SIGPIPE) {
		if (debug) printf("Received sigpipe.\n");
		exit(0);
	}
}

int main(int argc, char *argv[]) {

	/* Parse arguments */
	int c;
	struct option long_options[] = {
		{"shell", no_argument, 0, 's'},
		{"debug", no_argument, 0, 'd'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	while ((c = getopt_long(argc, argv, "s", long_options, &option_index)) != -1) {
		switch(c) {
			case 's':
				shell = true;
				break;
			case 'd':
				debug = true;
				break;
			default:
				fprintf(stderr, "Usage: ./lab1a OR ./lab1a --shell.\nDo not enter anything else.\n");
				exit(1);
				break;
		}
	}

	init_terminal();

	if (!shell) { 
		/* NON-SHELL MODE */
		int res = 0;
		char buf[256];
		while(1) {
			res = read(terminal_descriptor, &buf, 256);
			if (res < 0) {
				fprintf(stderr, "Error: During Read in non-shell mode.\n");
				exit(1);
			}
			for (int i = 0; i < res; i++) {
				if (buf[i] == '\004') {		//'\004' == Ctrl-D (need to include stdlib.h)
					char* terminate = "^D";
					if (write(1, terminate, 2) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode.\n");
						exit(1);
					}
					exit(0);
				}
				if (buf[i] == '\r' || buf[i] == '\n') {
					char* crlf = "\r\n";
					if (write(1, crlf, 2) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode.\n");
						exit(1);
					}
				}
				else {
					if (write(1, &buf[i], 1) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode.\n");
						exit(1);
					}
				}
			}
		}
	}
	else {
		/* SHELL MODE */
		signal(SIGPIPE, sighandler);

		/* Declare vairables */
		char buf_p[256];
		char buf_c[256];
		int pipe_in[2];
		int pipe_out[2];

		/* Create the pipe */
		if (pipe(pipe_in) || pipe(pipe_out)) {
			fprintf(stderr, "Error: Building pipe failed.\n");
			exit(1);
		}

		/* Fork process */
		pid = fork();
		if (pid == -1) {
			printf("Error: Folk.\n");
			exit(1);
		}
		
		/* CHILD PROCESS */
		else if (pid == 0) {

			if (close(pipe_in[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[0].\n");
				exit(1);
			}
			if (close(pipe_out[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[1].\n");
				exit(1);
			}
			if (dup2(pipe_in[1], STDERR_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(1);
			}
			if (dup2(pipe_in[1], STDOUT_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(1);
			}
			if (dup2(pipe_out[0], STDIN_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(1);
			}
			if (close(pipe_in[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(1);
			}

			char* execvp_argv[2];
			char execvp_filename[] = "/bin/bash";
			execvp_argv[0] = execvp_filename;
			execvp_argv[1] = NULL;
			if (execvp(execvp_filename, execvp_argv) < 0) {
				fprintf(stderr, "Error: Exec /bin/bash.\n");
				exit(1);
			}
		}
		/* PARENT PROCESS */
		else {

			if (close(pipe_in[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[0].\n");
				exit(1);
			}

			struct pollfd pfds[2];
			/* pfds[0] is keyboard stdin */
			pfds[0].fd = terminal_descriptor;
			pfds[0].events = POLLIN;
			/* pfds[1] is output return from shell */
			pfds[1].fd = pipe_in[0];
			pfds[1].events = POLLIN | POLLHUP | POLLERR;
			while (1) {
				if (poll(pfds, 2, 0) < 0) {
					fprintf(stderr, "Error: poll() failed.\n");
					exit(1);
				}
				if (pfds[0].revents & (POLLIN | POLLHUP | POLLERR)) {
					int count = 0;
					count = read(terminal_descriptor, &buf_p, 256);
					if (count < 0) {
						fprintf(stderr, "Error: During Read in shell mode.\n");
						exit(1);
					}
					for (int i = 0; i < count; i++) {
						if (buf_p[i] == '\003') {
							char* interrupt = "^C";
							if (write(1, interrupt, 2) < 0) {
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
							kill(pid, SIGINT);
							exit(0);
						}
						if (buf_p[i] == '\004') {
							char* terminate = "^D";
							if (write(1, terminate, 2) < 0) {
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
							// if (write(pipe_out[1], &buf_p[i], 1) < 0) {
							// 	fprintf(stderr, "Error: During Write in shell mode.\n");
							// 	exit(1);
							// }
							if (close(pipe_out[1]) < 0) {
								fprintf(stderr, "Error: Close pipe_out[1].\n");
								exit(1);
							}
							//Do last read from shell
							int count = 0;
							count = read(pipe_in[0], &buf_c, 256);
							if (count < 0) {
								fprintf(stderr, "Error: During Read in shell mode. \n");
								exit(1);
							}
							for (int i = 0; i < count; i++) {
								if (buf_c[i] == '\n') {
									char* crlf = "\r\n";
									if (write(1, crlf, 2) < 0) {
										fprintf(stderr, "Error: During Write in shell mode.\n");
										exit(1);
									}
								}
								if (buf_c[i] == '\004') {
									if (close(pipe_in[1] < 0)) {
										fprintf(stderr, "Error: Close pipe_in[1].\n");
									}
								}
								else {
									if (write(1, &buf_c[i], 1) < 0) {
										fprintf(stderr, "Error: During Write in shell mode.\n");
										exit(1);
									}
								}
							}
							// waitpid(pid, &status, 0);
							// fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
							exit(0);
						}
						if (buf_p[i] == '\r' || buf_p[i] == '\n') {
							char* crlf = "\r\n";
							char lf = '\n';
							if (write(1, crlf, 2) < 0) {					//echo to stdout
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
							if (write(pipe_out[1], &lf, 1) < 0) {			//go to shell
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
						}
						else {
							if (write(1, &buf_p[i], 1) < 0) {				//echo to stdout
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
							if (write(pipe_out[1], &buf_p[i], 1) < 0) {	//go to shell
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
						}
					}
				}
				if (pfds[1].revents & POLLIN) {
					int count = 0;
					count = read(pipe_in[0], &buf_c, 256);
					if (count < 0) {
						fprintf(stderr, "Error: During Read in shell mode. \n");
						exit(1);
					}
					for (int i = 0; i < count; i++) {
						if (buf_c[i] == '\n') {
							char* crlf = "\r\n";
							if (write(1, crlf, 2) < 0) {
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
						}
						if (buf_c[i] == '\004') {
							if (close(pipe_in[1] < 0)) {
								fprintf(stderr, "Error: Close pipe_in[1].\n");
							}
						}
						else {
							if (write(1, &buf_c[i], 1) < 0) {
								fprintf(stderr, "Error: During Write in shell mode.\n");
								exit(1);
							}
						}
					}
				}
				if (pfds[1].revents & (POLLHUP | POLLERR)) {
					if (close(pipe_out[1]) < 0) {
						fprintf(stderr, "Error: Close pipe_out[1].\n");
						exit(1);
					}
				}
			}
		}
		exit(0);
	}

	exit(0);
}