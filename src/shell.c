#include <stdio.h>
#include "shell.h"

int promptLine(char *prompt, char **line, size_t *len) {
				fputs(prompt, stderr);
				return getline(line, len, stdin);
}

