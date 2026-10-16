/**
 * Project 4 — UNIX Shell and History Feature
 *
 * Add author information here.
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_LINE 80       /* Maximum command size (80 chars), including room for '\0'. */
#define MAX_COMMANDS 9    /* Number of commands stored in history. */

#define SETUP_EXIT 0
#define SETUP_EXECUTE 1
#define SETUP_SKIP -1

/* Stores commands in a form that can be recalled and executed. */
char history[MAX_COMMANDS][MAX_LINE];

/* Number of commands successfully added to history since startup. */
int command_count = 0;

//extra credit funcs
void handle_sigchld(int sig){
	(void)sig; // didnt wanna get error due to -Wall in compile so just casted the sig var to void, hope this is okay (not that it should make a diff)
	int saved_errno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0); //loop and reap all terminated children
	errno = saved_errno;
}

/**
 * Add the most recent command to history.
 *
 * Requirements:
 * - Keep only the MAX_COMMANDS most recent commands.
 * - history should preserve the command in a form that can be executed again.
 * - A circular-array approach is recommended.
 */
void addtohistory(char inputBuffer[]) {
	int index = command_count % MAX_COMMANDS;
	strcpy(history[index], inputBuffer);
	command_count++;	
	return;
}

/**
 * Read and prepare the next command.
 *
 * Return values:
 *   SETUP_EXECUTE - a command is ready for main() to process
 *   SETUP_SKIP    - ignore this input and display another prompt
 *   SETUP_EXIT    - terminate the shell (CTRL-D / end of input)
 *
 * The function should:
 * - read a line from standard input,
 * - handle !! and !N history requests,
 * - add commands to the history,
 * - split the command into null-terminated argument strings,
 * - fill args with pointers to the beginning of those strings, and terminate args with NULL.
 * - set *background
 */
int setup(char inputBuffer[], char *args[], int *background) {
	int length; /* Number of characters read. */
	int i;      /* Index used to examine inputBuffer. */

	/*
	 * Read what the user enters on the command line.
	 *
	 * read() does NOT append '\0', so read at most MAX_LINE - 1 characters
	 * and add the terminator after checking the return value.
	 */
	do {
		printf("osh> ");
		fflush(stdout);

		length = read(STDIN_FILENO, inputBuffer, MAX_LINE - 1);

		if (length == 0) {
			/* CTRL-D / end of input. */
			return SETUP_EXIT;
		}

		if (length < 0) {
			perror("error reading the command");
			exit(EXIT_FAILURE);
		}

		/* Make inputBuffer a valid C string. */
		inputBuffer[length] = '\0';

	/* Ignore a line containing only Enter and display another prompt. */
	} while (length == 1 && inputBuffer[0] == '\n');
	

    /**
     * Part II — History Recall
     *
     * If the command begins with '!', handle !! or !N.
     * If a command is recalled:
     * - replace inputBuffer with the recalled command,
     * - print the recalled command, and
     * - update length to match the recalled command.
     *
     * Invalid history requests should print an error and return SETUP_SKIP.
    */

	int total_stored = command_count < MAX_COMMANDS ? command_count : MAX_COMMANDS;

	if (inputBuffer[0] == '!') {
		if (inputBuffer[1] == '!') {
			if (total_stored == 0) {
				printf("No commands in history.\n");
				return SETUP_SKIP;
			}
			int idx = (command_count - 1) % MAX_COMMANDS;
			strcpy(inputBuffer, history[idx]);
		}
		else if (isdigit(inputBuffer[1])) {
			int n = inputBuffer[1] - '0';
			if (n == 0 || n > total_stored){
				printf("No such command in history.\n");
				return SETUP_SKIP;
			}
			int idx = (command_count - n) % MAX_COMMANDS;
			strcpy(inputBuffer, history[idx]);
		} else {
			printf("No such command in history.\n");
			return SETUP_SKIP;
		}
		printf("%s\n", inputBuffer);
		length = strlen(inputBuffer);
	}

	if (strncmp(inputBuffer, "exit", 4) != 0) {
		addtohistory(inputBuffer);
	}
	
    /**
     * Part II — Add to History
     *
     * Add the command to history here, before parsing modifies inputBuffer.
     * If !! or !N was used, add the recalled command rather than the
     * literal history request.
     */

	/**
	 * Part I — Parse inputBuffer
	 *
	 * Use a separate argument index (for example, arg_index) rather than using
	 * i as both the character index and the args index.
	 *
	 * Spaces and tabs separate arguments. Replace separators with '\0'.
	 * Store the address of the first character of each argument in args.
	 */

	int arg_index = 0;
	int in_arg = 0;

	for (i = 0; i < length; i++) {
		switch (inputBuffer[i]) {
			case ' ':
			case '\t':
			case '\n':
				inputBuffer[i] = '\0';
				in_arg = 0;
				break;
			default:
				if (!in_arg){
					args[arg_index++] = &inputBuffer[i];
					in_arg = 1;
				}
				break;
		}
	}
	args[arg_index] = NULL;

    /*
    * TODO Part I:
    * - return SETUP_SKIP if no arguments were entered
    * - if the final argument is '&', set *background = 1 and
    *   remove '&' from args.
    * - terminate args with NULL,
    */

	if (arg_index == 0){
		return SETUP_SKIP;
	}

	if (arg_index > 0 && strcmp(args[arg_index - 1], "&") == 0){
		*background = 1;
		args[arg_index - 1] = NULL;
		arg_index--;
	}

	return SETUP_EXECUTE;
}

int main(void) {
	char inputBuffer[MAX_LINE];
	char *args[MAX_LINE / 2 + 1]; /* Up to 40 args plus the final NULL. */
	int background;
	int setup_result;
	pid_t child;

	// bonus stuff: register sigchld handler to reap background zombies
	struct sigaction sa;
	sa.sa_handler = handle_sigchld;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, NULL) < 0) {
		perror("sigaction failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		background = 0;

		setup_result = setup(inputBuffer, args, &background);

		if (setup_result == SETUP_EXIT) {
			break;
		}

		if (setup_result == SETUP_SKIP) {
			continue;
		}

		/**
         * Part I — Built-in exit
         *
         * If the command is exactly "exit", terminate the shell normally.
         */

		if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
			break;
		}


		/**
		 * Part II — Built-in history
		 *
		 * If the command is exactly "history": 
        */

		if(args[0] != NULL && strcmp(args[0], "history") == 0) {
			int total_stored = command_count < MAX_COMMANDS ? command_count : MAX_COMMANDS;
			for (int k = 1; k <= total_stored; k++) {
				int idx = (command_count - k) % MAX_COMMANDS;
				printf("%d %s\n", k, history[idx]);
			}
			continue;
		}

		/**
		 * Part I — fork / exec / wait
		 *
		 * child < 0: fork failed; print an error.
		 * child == 0: execute args with execvp(args[0], args).
		 *             If execvp() returns, print an error and exit the child
		 *             with EXIT_FAILURE.
		 * child > 0: this is the parent. If background == 0, wait specifically
		 *            for this child using waitpid(). If background == 1, do not
		 *            wait before returning to the prompt.
		 */

		child = fork();
		
		if (child < 0){
			// fork failed
			perror("fork failed");
			exit(EXIT_FAILURE);
		} else if (child == 0){
			if (execvp(args[0], args) < 0){
				perror("execvp failed");
				exit(EXIT_FAILURE);
			}
		} else {
			if (background == 0) {
				// wait for if this child is running in foreground
				waitpid(child, NULL, 0);
			}
		}
	}

	return 0;
}
