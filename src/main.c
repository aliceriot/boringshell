#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

int main()
{
				char *line = NULL;
				size_t len = 0;
				cmd_struct *command;

				while (promptLine("sheeeellll >>>> ", &line, &len) > 0) {
								pipeline_struct *pipeline = pipe_parse(line);
								int n_pipes = pipeline->n_cmds - 1;
								int (*pipes)[2] = calloc(sizeof(int[2]), n_pipes);

						    for (int i = 1; i < pipeline->n_cmds; ++i) {
									      pipe(pipes[i-1]);  
									      pipeline->cmds[i]->redirect[STDIN_FILENO] = pipes[i-1][0];
									      pipeline->cmds[i-1]->redirect[STDOUT_FILENO] = pipes[i-1][1];
								}
								
								for (int i = 0; i < pipeline->n_cmds; ++i) {
												run_with_redirect(pipeline->cmds[i], n_pipes, pipes);
								}

								close_all_the_pipes(n_pipes, pipes);

								for (int i = 0; i < pipeline->n_cmds; ++i) {
												wait(NULL);
								}
				}
				fputs("\n", stderr);
				return 0;
}
