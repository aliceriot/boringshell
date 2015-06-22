#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_LEN 1000
#define TOKEN_SEP " \t\n\r"

int promptLine(char *prompt, char **line, size_t *len) {
				fputs(prompt, stderr);
				return getline(line, len, stdin);
}

cmd_struct* parse_command(char* str) {
				char* copy = strndup(str, MAX_LEN);
				char* token;
				int i = 0;

				cmd_struct* ret = calloc(sizeof(cmd_struct) + MAX_LEN * sizeof(char*), 1);
				
				while ((token = next_non_empty(&copy))) {
								ret->args[i++] = token;
				}
				ret->progname = ret->args[0];
				ret->redirect[0] = ret->redirect[1] = -1;
				return ret;
}

char* next_non_empty(char **line) {
				char *tok;
				while ((tok = strsep(line, TOKEN_SEP)) && !*tok);
				return tok;
}

// pid_t forkexec(cmd_struct* command, int n_pipes, int (*pipes)[2]) {
pid_t forkexec(cmd_struct* command) {
				pid_t child_pid = fork();

				if (child_pid) {  /* We are the parent. */
								switch(child_pid) {
												case -1:
																fprintf(stderr, "Oh dear.\n");
																return -1;
												default:
																return child_pid;
								}
				} else {  // We are the child. */
								// exec_with_redir(command, n_pipes, pipes);
								forkexec(command);
								perror("OH DEAR");
								return 0;
				}
}


