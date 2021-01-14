/* Maxwell Christy (mchris25)
CPE 357 Assignment 6 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "parseline.h"
#include "mush.h"

#define LINEMAX 512
#define CMDMAX 10
#define ARGMAX 10
#define PIPEMAX 9


/* decided to edit parseline.c
   as opposed to putting logic
   in mush.c                */

int main(int argc, char *argv[])
{
	
	struct sigaction sa;
	sa.sa_handler = siginthandler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	signal(SIGINT, siginthandler);

	while(1)
	{
		parser();
	}
	return 0;
}

void siginthandler(int signum)
{
	write(STDOUT_FILENO, "\n8-P ", 5);
}