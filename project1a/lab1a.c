#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <strings.h>

static struct termios terminal_old;
static struct termios terminal_new;

//In non-canonical mode, input is not assembled into lines and input processing does not occur.
//c_cc[VTIME] (character timer) and c_cc[VMIN] (minimum number of characteres to receive before satsfying the read) controls the behaviour of this mode.

// Initialize terminal for non-echo mode.
static int terminal_init(void) {
	
	//Put keyboard (the file open on fd0) into terminal
	int terminal_descriptor = 0;

	//Obtain terminal settings (save current port setting)
	if (tcgetattr(terminal_descriptor, &terminal_old))
		return errno = ENOTSUP;	//ENOTSUP == operation not supported.

	//Erase the data in terminal_new
	bzero(&terminal_new, sizeof(terminal_new));

	terminal_new.c_iflag = ISTRIP; 	/* only lower 7 bits */
	terminal_new.c_oflag = 0;		/* no processing */
	terminal_new.c_lflag = 0;		/* no processing */

	terminal_new.c_cc[VTIME] = 0;	/* inter-character timer unused */
	terminal_new.c_cc[VMIN] = 1;	/* one character at a time */
}

int main(int argc, char *argv[]) {

}