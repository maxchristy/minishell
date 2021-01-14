/* Maxwell Christy (mchris25)
CPE 357 Assignment 6 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parseline.h"

#define LINEMAX 512
#define CMDMAX 10
#define ARGMAX 10
#define PIPEMAX 9

void parser()
{
	/* error vars */
	int cmdindex = 0;
	int arginstage = -1;
	int irno = 0;
	int orno = 0;

	int stages = 1;
	int indexes = 0;
	int j = 0;
	int i = 0;
	int fin;
	int fout;
	int len;
	int pipes[9][2];
	int insav = dup(STDIN_FILENO);
	int outsav = dup(STDOUT_FILENO);

	char line[LINEMAX] = {'\0'};
	char *tokens[PIPEMAX + (ARGMAX * CMDMAX) + CMDMAX];
	char *token;

	pid_t child;

	if(isatty(STDOUT_FILENO) == 1)
	{
		write(STDOUT_FILENO, "8-P ", 4);
	}

	if((len = read(STDIN_FILENO, line, LINEMAX + 1)) == 1)
	{
		fprintf(stderr, "invalid null command\n");
		return;
	}
	else if(len == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}

	else if(len > LINEMAX)
	{
		fprintf(stderr, "command line length exceeds max length\n");
		return;
	}
	line[strlen(line) - 1] = '\0';

	token = strtok(line, " ");
	while(token != NULL)
	{
		tokens[indexes++] = token;
		token = strtok(NULL, " ");
	}
	if(strcmp(tokens[j], "\n") == 0)
	{
		fprintf(stderr, "invalid null command\n");
		return;
	}
	/* check for glaring errors */
	for(j = 0; j < indexes; j++)
	{
		if((j + 1) < indexes && strcmp(tokens[j + 1], "|") == 0 &&
			strcmp(tokens[j], "|") == 0)
		{
			fprintf(stderr, "invalid null command\n");
			return;
		}
		if(strcmp(tokens[j], "|") == 0)
		{
			if(orno == 1)
			{
				fprintf(stderr, "%s: ambiguous output\n",
					tokens[cmdindex]);
				return;
			}
			stages++;
			cmdindex = j + 1;
			arginstage = -1;
			irno = 0;
			orno = 0;
		}
		else
		{
			arginstage++;
			if(arginstage > ARGMAX)
			{
				fprintf(stderr, "%s: too many arguments\n",
					tokens[cmdindex]);
				return;
			}
		}
		if(strcmp(tokens[j], "<") == 0)
		{
			irno++;
			if(stages > 1)
			{
				fprintf(stderr, "%s: ambiguous input\n",
					tokens[cmdindex]);
				return;
			}
			if(irno > 1)
			{
				fprintf(stderr, "%s: bad input redirection\n",
					tokens[cmdindex]);
				return;
			}
		}
		if(strcmp(tokens[j], ">") == 0)
		{
			orno++;
			if(orno > 1)
			{
				fprintf(stderr, "%s: bad output redirection\n",
					tokens[cmdindex]);
				return;
			}
		}
		if((j == (indexes - 1) || strcmp(tokens[j + 1], "|") == 0) &&
			strcmp(tokens[j], "<") == 0)
		{
			fprintf(stderr, "%s: bad input redirection\n",
				tokens[cmdindex]);
			return;
		}
		if((j == (indexes - 1) || strcmp(tokens[j + 1], "|") == 0) &&
			strcmp(tokens[j], ">") == 0)
		{
			fprintf(stderr, "%s: bad output redirection\n",
				tokens[cmdindex]);
			return;
		}
	}

	if(stages > CMDMAX)
	{
		fprintf(stderr, "pipeline too deep\n");
		return;
	}

	if(strcmp(tokens[0], "cd") == 0)
	{
		if(chdir(tokens[1]) == -1)
		{
			perror("chdir");
		}
		return;
	}

	char *stage = (char*)calloc(1024, sizeof(char));

	struct curstage curstage;
	curstage.input = "original stdin\0";
	curstage.output = "original stdout\0";
	curstage.argcount = 0;
	curstage.stageno = 0;
	curstage.inpipe = 0;
	curstage.outpipe = 0;
	curstage.inred = 0;
	curstage.outred = 0;

	for(j = 0; j < 12; j++)
	{
		curstage.argvector[j] = NULL;
	}

	for(j = 0; j < 9; j++)
	{
		pipe(pipes[j]);
	}

	/* input should be proper by this point */
	for(j = 0; j < indexes; j++)
	{
		if(j == 0
		|| !(strcmp(tokens[j - 1], ">") == 0
		|| strcmp(tokens[j - 1], "<") == 0
		|| strcmp(tokens[j], "|") == 0
		|| strcmp(tokens[j], "<") == 0
		|| strcmp(tokens[j], ">") == 0))
		{
			curstage.argvector[curstage.argcount] = tokens[j];
			curstage.argcount++;
		}
		if(strcmp(tokens[j], "|") != 0)
		{
			strcat(stage, tokens[j]);
		}
		if((j + 1) < indexes &&
			strcmp(tokens[j + 1], "|") != 0)
		{
			strcat(stage, " ");
		}
		if(curstage.inpipe == 1)
		{
			curstage.input = "pipe from stage\0";
			curstage.outpipe = 0;
			curstage.output = "original stdout\0";
		}
		if(strcmp(tokens[j], "<") == 0)
		{
			curstage.input = tokens[j + 1];
			curstage.inred = 1;

			fin = open(tokens[j + 1],  O_CREAT | O_RDWR , 0666);
			dup2(fin, STDIN_FILENO);
			close(fin);
			continue;

		}
		if(strcmp(tokens[j], ">") == 0)
		{
			curstage.output = tokens[j + 1];
			curstage.outred = 1;

			fout = open(tokens[j + 1],
				O_CREAT | O_RDWR |O_TRUNC, 0666);
			dup2(fout, STDOUT_FILENO);
			close(fout);
			continue;
		}
		

		if(strcmp(tokens[j], "|") == 0)
		{
			curstage.output = "pipe to stage\0";
			curstage.outpipe = 1;
			curstage.stage = stage;
			/* print_output(curstage); */
			if((child = fork()) == 0)
			{
				if(strcmp(curstage.input,
					"pipe from stage") == 0)
				{
					dup2(pipes[curstage.stageno - 1][0],
						STDIN_FILENO);
					close(pipes[curstage.stageno - 1][1]);
				}
				if(strcmp(curstage.output,
					"pipe to stage") == 0)
				{
					dup2(pipes[curstage.stageno][1],
						STDOUT_FILENO);
					close(pipes[curstage.stageno][0]);
				}
				execvp(curstage.argvector[0],
					curstage.argvector);
				return;
			}
			else
			{
				if(strcmp(curstage.input,
					"pipe from stage") == 0)
				{
					close(pipes[curstage.stageno - 1][0]);
				}
				if(strcmp(curstage.output,
					"pipe to stage") == 0)
				{
					close(pipes[curstage.stageno][1]);
				}
				wait(NULL);
			}


			curstage.inpipe = 1;
			curstage.outpipe = 0;
			curstage.inred = 0;
			curstage.outred = 0;
			curstage.stageno++;
			curstage.argcount = 0;
			memset(stage,0,1024);
			for(i = 0; i < 12; i++)
			{
				curstage.argvector[i] = NULL;
			}
			continue;
		}
		

		if(j == indexes - 1)
		{
			curstage.stage = stage;
			/* print_output(curstage); */
			if((child = fork()) == 0)
			{
				if(strcmp(curstage.input,
					"pipe from stage") == 0)
				{
					dup2(pipes[curstage.stageno - 1][0],
						STDIN_FILENO);
				}
				for(i = 0; i < 9; i++)
				{
					close(pipes[i][0]);
				}
				execvp(curstage.argvector[0],
					curstage.argvector);
				return;
			}
			else
			{				
				for(i = 0; i < 9; i++)
				{
					close(pipes[i][1]);
				}
				wait(NULL);
				dup2(insav, STDIN_FILENO);
				dup2(outsav, STDOUT_FILENO);
				
			}
		}
	}
	free(stage);
}

/* unused, relic of ASGN5 */
void print_output(struct curstage curstage)
{
	int v;
	printf("\n--------\n");

	printf("Stage %d: %c%s%c\n",
		curstage.stageno, 34, curstage.stage, 34);

	printf("--------\n");

	printf("     input: %s ", curstage.input);
	if(curstage.inpipe == 1)
	{
		printf("%d", curstage.stageno - 1);
	}
	printf("\n");

	printf("    output: %s ", curstage.output);
	if(curstage.outpipe == 1)
	{
		printf("%d", curstage.stageno + 1);
	}
	printf("\n");

	printf("      argc: %d\n", curstage.argcount);

	printf("      argv: ");
	for(v = 0; v < curstage.argcount; v++)
	{
		printf("%c%s%c", 34, curstage.argvector[v], 34);
		if(v != curstage.argcount - 1)
		{
			printf(",");
		}
	}
	printf("\n");
}