#ifndef	parseline
#define parseline

void parser();

struct curstage
{
	int stageno;
	int argcount;
	int inpipe;
	int outpipe;
	int inred;
	int outred;

	char *input;
	char *output;
	char *stage;
	char *argvector[12];
	/* usually 11 */
};

void print_output(struct curstage);

#endif