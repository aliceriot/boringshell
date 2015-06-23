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

pid_t run_with_redirect(cmd_struct *command, int n_pipes, int (*pipes)[2]) 
{
				pid_t child_pid = fork();
				
				if (strcmp(command->progname, "cd") == 0) {
								chdir(command->args[1]);
								return child_pid;
				} else if  (child_pid) {
								switch(child_pid) {
												case - 1:
																fprintf(stderr, "Oh dear.\n");
																return -1;
												default:
																return child_pid;
								}
				} else {
								exec_with_redirect(command, n_pipes, pipes);
								perror("OH NOES");
								return 0;
				}
}

void close_all_the_pipes(int n_pipes, int (*pipes)[2])
{
				for (int i = 0; i < n_pipes; ++i) {
								close(pipes[i][0]);
								close(pipes[i][1]);
				}
}

int exec_with_redirect(cmd_struct *cmd, int n_pipes, int (*pipes)[2])
{
				int fd = -1;
				if ((fd = cmd->redirect[0]) != -1)
								dup2(fd, STDIN_FILENO);
				if ((fd = cmd->redirect[1]) != -1) 
								dup2(fd, STDOUT_FILENO);
				close_all_the_pipes(n_pipes, pipes);
				return execvp(cmd->progname, cmd->args);
}

pipeline_struct *pipe_parse(char *cmd)
{
				int i, numcmds = 0;
				char *copy = strndup(cmd, MAX_LEN);
				char *cmdstr;

				for (i = 0; copy[i] != '\0'; i++)
								if (copy[i] == '|')
												numcmds++;
				numcmds++;
				
				pipeline_struct *ret;
				ret = calloc(sizeof(pipeline_struct) + numcmds*sizeof(cmd_struct), 1);
				ret->n_cmds = numcmds;

				while ((cmdstr = strsep(&copy, "|")))
								ret->cmds[i++] = parse_command(cmdstr);

				return ret;
}
