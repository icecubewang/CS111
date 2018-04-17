/*
NAME: Feilan Wang
EMAIL: wangfeilan@hotmail.com
ID: 104796844
*/
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <poll.h>
#include <sys/wait.h>
#define _POSIX_SOURCE

static struct termios terminal_old;
static struct termios terminal_new;
//Put keyboard (the file open on fd0) into terminal
int terminal_descriptor = 0;
bool debug = false;
bool shell = false;
pid_t pid = -1;	//child's pid
int status = 0;

/* Declare vairables */
char buf_p[256];
char buf_c[256];
int pipe_in[2];
int pipe_out[2];

//In non-canonical mode, input is not assembled into lines and input processing does not occur.
//c_cc[VTIME] (character timer) and c_cc[VMIN] (minimum number of characteres to receive before satsfying the read) controls the behaviour of this mode.

/* Reset Input mode */
void reset_input_mode(void) {
	tcsetattr(terminal_descriptor, TCSANOW, &terminal_old);	//Restore
	if (shell) {
		//Do last read from shell
		int count = 0;
		count = read(pipe_in[0], &buf_c, 256);
		if (count < 0) {
			fprintf(stderr, "Error: During Read in shell mode. %s\r\n", strerror(errno));
			exit(1);
		}
		for (int i = 0; i < count; i++) {
			if (buf_c[i] == '\n') {
				char* crlf = "\r\n";
				if (write(1, crlf, 2) < 0) {
					fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
					exit(1);
				}
			}
			else {
				if (write(1, &buf_c[i], 1) < 0) {
					fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
					exit(1);
				}
			}
		}
		//message
		if (waitpid(pid, &status, 0) < 0) {
			fprintf(stderr, "Error: with waitpid. %s\r\n", strerror(errno));
			exit(1);
		}
		int p_status = 0;
		int p_signal = 0;

		if(WIFEXITED(status))
		{
			p_status = WEXITSTATUS(status);
		}
		if (WIFSIGNALED(status))
		{
			p_signal = WTERMSIG(status);
		}
		fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", p_signal, p_status);
		//exit(0);
	}
}

/* Initialize a new terminal */
void init_terminal(void) {

	if (!isatty(0)) {
		fprintf(stderr, "Error: Not a terminal. %s\r\n", strerror(errno));
		exit(1);
	}
	tcgetattr(terminal_descriptor, &terminal_old);				//Obtain terminal settings (save current port setting)
	atexit(reset_input_mode);									//This is to prevent the program exits half way and didn't reset

	//bzero(&terminal_new, sizeof(terminal_new));					//Erase the data in terminal_new

	terminal_new.c_iflag = ISTRIP; 	/* only lower 7 bits */
	terminal_new.c_oflag = 0;		/* no processing */
	terminal_new.c_lflag = 0;		/* no processing */

	tcsetattr(terminal_descriptor, TCSANOW, &terminal_new);	//TCSANOW: make the change immediately
}

/* Signal Handler */
void sighandler(int sig) {
	if (sig == SIGPIPE) {
		char *errmsg = "Error: Caught SIGPIPE with signal number.\r\n";
		write(2, errmsg, strlen(errmsg));
		_exit(0);
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
	while ((c = getopt_long(argc, argv, "sd", long_options, &option_index)) != -1) {
		switch(c) {
			case 's':
				shell = true;
				break;
			case 'd':
				debug = true;
				break;
			default:
				fprintf(stderr, "Usage: ./lab1a OR ./lab1a --shell.\r\nDo not enter anything else.\r\n");
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
				fprintf(stderr, "Error: During Read in non-shell mode. %s\r\n", strerror(errno));
				exit(1);
			}
			for (int i = 0; i < res; i++) {
				if (buf[i] == '\004') {		//'\004' == Ctrl-D (need to include stdlib.h)
					char* terminate = "^D";
					if (write(1, terminate, 2) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode. %s\r\n", strerror(errno));
						exit(1);
					}
					exit(0);
				}
				if (buf[i] == '\r' || buf[i] == '\n') {
					char* crlf = "\r\n";
					if (write(1, crlf, 2) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode. %s\r\n", strerror(errno));
						exit(1);
					}
				}
				else {
					if (write(1, &buf[i], 1) < 0) {
						fprintf(stderr, "Error: During Write in non-shell mode. %s\r\n", strerror(errno));
						exit(1);
					}
				}
			}
		}
	}
	else {
		/* SHELL MODE */
		if (signal(SIGPIPE, sighandler) == SIG_ERR) {
			fprintf(stderr, "Error: with signal. %s\r\n", strerror(errno));
			exit(1);
		}

		/* Create the pipe */
		if ((pipe(pipe_in) == -1) || (pipe(pipe_out) == -1)) {
			fprintf(stderr, "Error: Building pipe failed. %s\r\n", strerror(errno));
			exit(1);
		}

		/* Fork process */
		pid = fork();
		if (pid == -1) {
			fprintf(stderr, "Error: Folk. %s\r\n", strerror(errno));
			exit(1);
		}
		
		/* CHILD PROCESS */
		else if (pid == 0) {

			if (close(pipe_in[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[0]. %s\r\n", strerror(errno));
				exit(1);
			}
			if (close(pipe_out[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[1]. %s\r\n", strerror(errno));
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
				fprintf(stderr, "Error: Close pipe_in[1]. %s\r\n", strerror(errno));
				exit(1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1]. %s\r\n", strerror(errno));
				exit(1);
			}

			char* execvp_argv[2];
			char execvp_filename[] = "/bin/bash";
			execvp_argv[0] = execvp_filename;
			execvp_argv[1] = NULL;
			if (execvp(execvp_filename, execvp_argv) < 0) {
				fprintf(stderr, "Error: Exec /bin/bash. %s\r\n", strerror(errno));
				exit(1);
			}
		}
		/* PARENT PROCESS */
		else {

			if (close(pipe_in[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1]. %s\r\n", strerror(errno));
				exit(1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[0]. %s\r\n", strerror(errno));
				exit(1);
			}

			struct pollfd pfds[2];
			/* pfds[0] is keyboard stdin */
			pfds[0].fd = terminal_descriptor;
			pfds[0].events = POLLIN;
			/* pfds[1] is output return from shell */
			pfds[1].fd = pipe_in[0];
			pfds[1].events = POLLIN;

			while (1) {
				int num = poll(pfds, 2, -1);
				if (num < 0) {
					fprintf(stderr, "Error: poll() failed. %s\r\n", strerror(errno));
					exit(1);
				}
				if (num == 0) {
					continue;
				}
				if (pfds[0].revents & POLLIN) {
					int count = 0;
					count = read(terminal_descriptor, buf_p, 256);
					if (count < 0) {
						fprintf(stderr, "Error: During Read in shell mode. %s\r\n", strerror(errno));
						exit(1);
					}
					for (int i = 0; i < count; i++) {
						if (buf_p[i] == '\004') {
							char* terminate = "^D";
							if (write(1, terminate, 2) < 0) {
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
							if (close(pipe_out[1]) < 0) {
								fprintf(stderr, "Error: Close pipe_out[1]. %s\r\n", strerror(errno));
								exit(1);
							}
							exit(0);
						}
						else if (buf_p[i] == '\003') {
							//write(pipe_out[1], "\n", 1);
							if (kill(pid, SIGINT) == -1) {
								fprintf(stderr, "Error: kill not success. %s\r\n", strerror(errno));
								exit(1);
							} 
							if (write(pipe_out[1], "\n", 1) < 0) {
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
							if (debug) {
								char* interrupt = "^C";
								if (write(1, interrupt, 2) < 0) {
									fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
									exit(1);
								}
							}
						}
						else if (buf_p[i] == '\r' || buf_p[i] == '\n') {
							char* crlf = "\r\n";
							char lf = '\n';
							if (write(1, crlf, 2) < 0) {					//echo to stdout
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
							if (write(pipe_out[1], &lf, 1) < 0) {			//go to shell
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
						}
						else {
							if (write(1, &buf_p[i], 1) < 0) {				//echo to stdout
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
							if (write(pipe_out[1], &buf_p[i], 1) < 0) {	//go to shell
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
						}
					}
				}
				if (pfds[1].revents & POLLIN) {
					int count = 0;
					count = read(pipe_in[0], &buf_c, 256);
					if (count < 0) {
						fprintf(stderr, "Error: During Read in shell mode. %s\r\n", strerror(errno));
						exit(1);
					}
					for (int i = 0; i < count; i++) {
						if (buf_c[i] == '\n') {
							char* crlf = "\r\n";
							if (write(1, crlf, 2) < 0) {
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
						}
						else {
							if (write(1, &buf_c[i], 1) < 0) {
								fprintf(stderr, "Error: During Write in shell mode. %s\r\n", strerror(errno));
								exit(1);
							}
						}
					}
				}
				if ((pfds[0].revents & POLLHUP) || (pfds[0].revents & POLLERR)) {
					if (close(pipe_out[1]) < 0) {
						fprintf(stderr, "Error: Close pipe_out[1]. %s\r\n", strerror(errno));
						exit(1);
					}
					exit(0);
				}
				if ((pfds[1].revents & POLLHUP) || (pfds[1].revents & POLLERR)) {
					exit(0);
				}
			}
		}
		exit(0);
	}
	exit(0);
}