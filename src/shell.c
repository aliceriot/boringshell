#include <stdio.h>
#include "shell.h"

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
				

