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

static struct termios terminal_old;
static struct termios terminal_new;
//Put keyboard (the file open on fd0) into terminal
int terminal_descriptor = 0;

//In non-canonical mode, input is not assembled into lines and input processing does not occur.
//c_cc[VTIME] (character timer) and c_cc[VMIN] (minimum number of characteres to receive before satsfying the read) controls the behaviour of this mode.

/* Reset Input mode */
void reset_input_mode(void) {
	tcsetattr(terminal_descriptor, TCSANOW, &terminal_old);	//Restore
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
	tcsetattr(terminal_descriptor, TCSANOW, &terminal_new);		//TCSANOW: make the change immediately
}


int main(int argc, char *argv[]) {

	/* Parse arguments */
	int c;
	bool shell = false;
	bool debug = false;
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
				break;
		}
	}

	init_terminal();

	if (!shell) { 
		/* NON-SHELL MODE */
		int res = 0;
		char buf[256];
		do {
			res = read(terminal_descriptor, &buf, 256);
			if (res < 0) {
				fprintf(stderr, "Error: Read in non-shell mode. \n");
			}
			if (buf[0] == '\r' || buf[0] == '\n') {	//'\013' == /r and '\010' == /n
				buf[0] = '\r';
				buf[1] = '\n';
				write(1, &buf, res+1);
			} 
			else {
				write(1, &buf, res);
			}
		} while (buf[0] != '\004');	//'\004' == Ctrl-D (need to include stdlib.h)
	}
	else {
		/* SHELL MODE */

		/* Declare vairables */
		char buf_p[256];
		char buf_c[256];
		pid_t pid;	//child's pid
		int pipe_in[2];
		int pipe_out[2];

		/* Create the pipe */
		if (pipe(pipe_in) || pipe(pipe_out)) {
			fprintf(stderr, "Error: Building pipe failed.\n");
			exit(-1);
		}

		/* Fork process */
		pid = fork();
		if (pid == -1) {
			printf("Error: Folk.\n");
			exit(-1);
		}
		
		/* CHILD PROCESS */
		else if (pid == 0) {

			if (close(pipe_in[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[0].\n");
				exit(-1);
			}
			if (close(pipe_out[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[1].\n");
				exit(-1);
			}
			if (dup2(pipe_in[1], STDERR_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(-1);
			}
			if (dup2(pipe_in[1], STDOUT_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(-1);
			}
			if (dup2(pipe_out[0], STDIN_FILENO) < 0) {
				fprintf(stderr, "Error: dup2.\n");
				exit(-1);
			}
			if (close(pipe_in[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(-1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(-1);
			}

			char* execvp_argv[2];
			char execvp_filename[] = "/bin/bash";
			execvp_argv[0] = execvp_filename;
			execvp_argv[1] = NULL;
			if (execvp(execvp_filename, execvp_argv) < 0) {
				fprintf(stderr, "Error: Exec /bin/bash.\n");
				exit(-1);
			}
		}
		/* PARENT PROCESS */
		else {

			if (close(pipe_in[1]) < 0) {
				fprintf(stderr, "Error: Close pipe_in[1].\n");
				exit(-1);
			}
			if (close(pipe_out[0]) < 0) {
				fprintf(stderr, "Error: Close pipe_out[0].\n");
				exit(-1);
			}

			struct pollfd pfds[2];
			/* pfds[0] is keyboard stdin */
			pfds[0].fd = terminal_descriptor;
			pfds[0].events = POLLIN | POLLHUP | POLLERR;
			/* pfds[1] is output return from shell */
			pfds[1].fd = pipe_in[0];
			pfds[1].events = POLLIN | POLLHUP | POLLERR;
			while (1) {
				if (poll(pfds, 2, 0) < 0) {
					fprintf(stderr, "Error: poll() failed.\n");
				}
				if (pfds[0].revents & (POLLIN | POLLHUP | POLLERR)) {
					int count = 0;
					count = read(terminal_descriptor, &buf_p, 256);
					for (int i = 0; i < count; i++) {
						if (buf_p[i] == '\004') {
							close(pipe_in[0]);
							exit(1);
						}
						if (buf_p[i] == '\r' || buf_p[i] == '\n') {
							char* crlf = "\r\n";
							char lf = '\n';
							write(1, crlf, 2);					//echo to stdout
							write(pipe_out[1], &lf, 1);			//go to shell
						}
						else {
							write(1, &buf_p[i], 1);				//echo to stdout
							write(pipe_out[1], &buf_p[i], 1);	//go to shell
						}
					}
				}
				if (pfds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
					int count = 0;
					if (debug) {
						printf("shell output \n");
					}
					count = read(pipe_in[0], &buf_c, 256);
					for (int i = 0; i < count; i++) {
						if (buf_c[i] == '\n') {
							char* crlf = "\r\n";
							write(1, crlf, 2);
						}
						else {
							write(1, &buf_c[i], 1);
						}
					}
				}
			}


		// 	pfds[0].fd = terminal_descriptor;
		// 	pfds[0].events = POLLIN | POLLHUP | POLLERR;
		// 	pfds[1].fd = pipe_in[0];
		// 	pfds[1].events = POLLIN | POLLHUP | POLLERR;
		// 	poll(pfds, 2, 0);

			// do {
			// 	int count = 0;
			// 	count = read(terminal_descriptor, &buf_p, 256);
			// 	if (buf_p[0] == '\r' || buf_p[0] == '\n') {	//'\013' == /r and '\010' == /n
			// 		buf_p[0] = '\r';
			// 		buf_p[1] = '\n';
			// 		buf_p[2] = 0;
			// 		write(1, &buf_p, count+1);
			// 		//Forward it to the shell
			// 		write(pipe_out[1], &buf_p[1], count);
			// 		read(pipe_out[0], &buf_p[1], count);
			// 		read(pipe_in[0], &buf_c[1], count);
			// 		write(1, &buf_c, count+1);
			// 	} 
			// 	else {
			// 		buf_p[count] = 0;
			// 		write(1, &buf_p, count);
			// 		//Forward it to the shell
			// 		write(pipe_out[1], &buf_p, count);
			// 		read(pipe_out[0], &buf_p, count);
			// 		read(pipe_in[0], &buf_c, count);
			// 		write(1, &buf_c, count);
			// 	}
			// } while (buf_p[0] != '\004');	//'\004' == Ctrl-D (need to include stdlib.h)
		}

				// int readcount = 0;
				// readcount = read(terminal_descriptor, &buf_p, 256);
				// for (int i = 0; i < readcount; i++) {
				// 	printf("reading\r\n");
				// 	if (buf_p[i] == '\004')
				// 		exit(0);
				// 	else if (buf_p[i] == '\r' || buf_p[i] == '\n') {
				// 		char* crlf = "\r\n";
				// 		char lf = '\n';
				// 		write(1, crlf, 2);
				// 		write(pipe_out[1], &lf, 1);
				// 		read(pipe_out[0], &lf, 1);
				// 	}
				// 	else {
				// 		printf("before write\r\n");
				// 		write(1, &buf_p[i], 1);
				// 		printf("after write to screen \r\n");
				// 		write(pipe_out[1], &buf_p[i], 1);
				// 		printf("after write to pipe \r\n");
				// 		read(pipe_out[0], &buf_p[i], 1);
				// 		printf("after write\r\n");
				// 	}
				// }

				// printf("out of first loop \r\n");
				// readcount = read(pipe_in[0], &buf_c, 256);
				// printf("readcount = %d\r\n", readcount);

				// for (int i = 0; i < readcount; i++) {
				// 	if (buf_c[i] == '\n') {
				// 		char* crlf = "\r\n";
				// 		write(1, crlf, 2);
				// 	}
				// 	else {
				// 		write(1, &buf_c[i], 1);
				// 	}
				// }
				// printf("after writing from shell\r\n");




			// 	if (buf_p[0] == '\r' || buf_p[0] == '\n') {	//'\013' == /r and '\010' == /n
			// 		buf_p[0] = '\r';
			// 		buf_p[1] = '\n';
			// 		buf_p[2] = 0;
			// 		write(1, &buf_p, count+1);
			// 		//Forward it to the shell
			// 		write(pipe_out[1], &buf_p[1], count);
			// 		read(pipe_out[0], &buf_p[1], count);
			// 		read(pipe_in[0], &buf_c[1], count);
			// 		write(1, &buf_c, count+1);
			// 	} 
			// 	else {
			// 		buf_p[count] = 0;
			// 		write(1, &buf_p, count);
			// 		//Forward it to the shell
			// 		write(pipe_out[1], &buf_p, count);
			// 		read(pipe_out[0], &buf_p, count);
			// 		read(pipe_in[0], &buf_c, count);
			// 		write(1, &buf_c, count);
			// 	}
			// } while (buf_p[0] != '\004');	//'\004' == Ctrl-D (need to include stdlib.h)
		
		// struct pollfd pfds[2];

		// while(1) {
		// 	pfds[0].fd = terminal_descriptor;
		// 	pfds[0].events = POLLIN | POLLHUP | POLLERR;
		// 	pfds[1].fd = pipe_in[0];
		// 	pfds[1].events = POLLIN | POLLHUP | POLLERR;
		// 	poll(pfds, 2, -1);
		// 	if (pfds[0].revents & (POLLIN | POLLHUP | POLLERR)) {
		// 		do {
		// 			int count = 0;
		// 			count = read(terminal_descriptor, &buf_p, 256);
		// 			if (buf_p[0] == '\r' || buf_p[0] == '\n') {	//'\013' == /r and '\010' == /n
		// 				buf_p[0] = '\r';
		// 				buf_p[1] = '\n';
		// 				buf_p[2] = 0;
		// 				write(1, &buf_p, count+1);
		// 				//Forward it to the shell
		// 				write(pipe_out[1], &buf_p[1], count);
		// 				read(pipe_out[0], &buf_p[1], count);
		// 				// read(pipe_in[1], &buf_c[1], count);
		// 				// write(1, &buf_c, count+1);
		// 			} 
		// 			else {
		// 				buf_p[count] = 0;
		// 				write(1, &buf_p, count);
		// 				//Forward it to the shell
		// 				write(pipe_out[1], &buf_p, count);
		// 				read(pipe_out[0], &buf_p, count);
		// 				// read(pipe_in[1], &buf_c, count);
		// 				// write(1, &buf_c, count);
		// 			}
		// 		} while (buf_p[0] != '\004');	//'\004' == Ctrl-D (need to include stdlib.h)

		// 	}
		// 	if (pfds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
		// 		do {
		// 			int count = 0;
		// 			count = read(pipe_in[1], &buf_c, count);
		// 			for (int i = 0; i < count; i++) {
		// 				if (buf_c[i] == '\n') {
		// 					char* temp = "\r\n";
		// 					write(1, &temp, 2);
		// 				} 
		// 				else {
		// 					write(1, &buf_c, 1);
		// 				}
		// 			}
		// 		} while(buf_c[0] != '\004');
		// 	}
		// }
	}

	exit(0);
}