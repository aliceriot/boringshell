#include <stdio.h>
#include "shell.h"

void main()
{
				char *line = NULL;
				size_t len = 0;
				cmd_struct *command;

				while (promptLine("sheeeellll >>>> ", &line, &len) > 0) {
								command = parse_command(

				}
}
