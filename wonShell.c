#include "common.h"

int main(void) {
	char args[MAX_LENGTH / 2 + 1];
	int ptr;
	int status = 0;
	int run = 1;
	command com;

	initParser();
	initExcute();

	while (run) {
		prompt();

		// Get input
		if (gets(args) == NULL) {
			printf("Command is too long(MAX:%d)\n", MAX_LENGTH);
			while (getchar() != '\n');
			continue;
		}

		// Parse input
		status = parse(args);

		// Get command
		ptr = getHisPtr();
		com = getHis((ptr - 1 + hisBuf) % hisBuf);

		// Normal excute
		if (status == 0) {
			status = excute(com.argc, com.argv);
		}
		// exit, quit
		else if (status == 1) {
			run = 0;
		}
		// Reserved excute
		else {
			status = excuteR(com.argc, com.argv, status);
		}

		if (status != 0) {

		}
	}

	freeParser();
	freeExcute();
}