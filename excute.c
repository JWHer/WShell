#include "common.h"

char *path;
char *lastPath;
int notWait;

int excute(int argc, char *argv[]) {

	// Fork a child process
	pid_t pid;
	pid = fork();

	// Fork fail
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		//printf("Error msg");
	}
	// Child process
	else if (pid == 0) {
		// v->argv, p->environment variables path
		execvp(argv[0], argv);
		fprintf(stderr, "Invalid command\n");
		exit(1);
		//printf("Error msg");
	}
	// Parent process
	else {
		if(notWait==0)
			wait(NULL);
		notWait = 0;
	}

	return 0;
}

// Reserved excute
int excuteR(int argc, char* argv[], int status) {
	command bfrCom;
	// Before Command
	bfrCom = getHis((getHisPtr() - (status / 10) - 1 + hisBuf) % hisBuf);

	switch (status % 10)
	{
	case 2:
		// History
		printHist();
		break;
	case 3:
		if (bfrCom.argc == 0) {
			printf("Nothing to excute\n");
		}
		else {
			printf(">");
			echoCom(bfrCom);

			status = reservedWord(bfrCom.argv[0]);
			if (status != 0) {
				excuteR(bfrCom.argc, bfrCom.argv, status);
			}
			else {
				excute(bfrCom.argc, bfrCom.argv);
			}
		}
		break;
	case 4:
		if (changeDir(argc, argv) != 0) return -1;
		break;
	default:
		break;
	}
	return 0;
}

int changeDir(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Wrong Input\n");
		return -1;
	}

	// Get now
	if (strcmp(argv[1], ".") == 0) {
		printf("> %s\n", path);
	}
	// Up one level
	else if (strcmp(argv[1], "..") == 0) {
		// Error
		if (chdir("../") != 0) {
			fprintf(stderr, "Already Root\n");
			return -1;
		}
		// Copy the path
		if (lastPath != NULL)
			free(lastPath);
		lastPath = (char*)malloc(sizeof(char)*strlen(path));
		strcpy(lastPath, path);
	}
	// Go home
	else if (strcmp(argv[1], "~") == 0 ||
		strcmp(argv[1], "home") == 0 ||
		strcmp(argv[1], "HOME") == 0) {
		if (chdir("/") != 0) {
			fprintf(stderr, "Unkown Error. Can't move to home directory\n");
			return -1;
		}
		// Copy the path
		if (lastPath != NULL)
			free(lastPath);
		lastPath = (char*)malloc(sizeof(char)*strlen(path));
		strcpy(lastPath, path);
	}
	// Last path
	else if (strcmp(argv[1], "-") == 0) {
		if (lastPath == NULL) {
			printf("There is no last path\n");
			return -1;
		}
		if (chdir(lastPath) != 0) {
			fprintf(stderr, "Unkown Error. Can't move to last directory\n");
			return -1;
		}
		// Copy the path
		free(lastPath);
		lastPath = (char*)malloc(sizeof(char)*strlen(path));
		strcpy(lastPath, path);
	}
	// Go path
	else {
		if (chdir(argv[1]) != 0) {
			fprintf(stderr, "Invalid path\n");
			return -1;
		}
		// Copy the path
		if (lastPath != NULL)
			free(lastPath);
		lastPath = (char*)malloc(sizeof(char)*strlen(path));
		strcpy(lastPath, path);
	}

	return 0;
}

void prompt()
{
	if (path == NULL)
		path = (char*)malloc(sizeof(char)*MAX_LENGTH);

	// Read Path
	if (getcwd(path, MAX_LENGTH) == NULL) {
		fprintf(stderr, "Current directory error\n");
	}
	else
		printf("%s", path);

	printf("$wsh>");
}

void setWait(int nw) {
	notWait = nw;
}

void initExcute() {
	notWait = 0;
}

void freeExcute() {
	free(path);
	free(lastPath);
}

/*
	char **new = (char**)malloc(sizeof(char*)*(argc + 1));
	int i;
	for (i = 0; i < argc - 1; i++) new[i] = argv[i + 1];

	// While end of line
		while (*ps != '\0') {
			// Info changed
			if (*ps == '\\') {
				ps++;
				// User name
				if (*ps == 'u') {
					printf("%s", getenv("USER"));
				}
				// Host name
				else if (*ps == 'h') {
					printf("%s", getenv("HOSTNAME"));
				}
				// Directory
				else if (*ps == 'w') {
					printf("%s", get_current_dir_name());
				}
			}

			// not changed
			else {
				printf("%c", *ps);
				ps++;
			}
		}// End of while
*/
