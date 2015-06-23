#include <stdio.h>

/* data structures */
typedef struct {
				char* progname;
			  int redirect[2];
				char* args[];
} cmd_struct;

typedef struct {
				int n_cmds;
			  cmd_struct* cmds[];
} pipeline_struct;

int promptLine(char *prompt, char **line, size_t *len);
cmd_struct* parse_command(char* str);
char* next_non_empty(char **line);
pid_t run_with_redirect(cmd_struct *cmd, int npipes, int(*pipes)[2]);
int exec_with_redirect(cmd_struct *cmd, int npipes, int (*pipes)[2]);
pipeline_struct* pipe_parse(char *line);
void close_all_the_pipes(int n_pipes, int (*pipes)[2]);
