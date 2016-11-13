#include "str.h"


void clear_console(void)
{
	pid_t status = fork();
	switch(status)
	{
		case -1:
			printf("Couldn\'t \'fork()\' the current process into a child: %s\n", strerror(errno));
			break;

		case 0:
			if(execvp("clear", (char *const[]){"clear", NULL}) == -1)
			{
				printf("Couldn\'t \'exec()\' the command: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}

			exit(EXIT_SUCCESS);

		default:
			wait(&status);
			break;
	}
}


void command_cursor(void)
{
	printf("|: ");
	fflush(stdout);
}


void prompt_keyboard(char *const buffer)
{
	strncpy(buffer, "", COMMANDBUFFER);
	fgets(buffer, COMMANDBUFFER, stdin);
}


void free_args(char **args, uint16_t *const nbArgs)
{
	for(uint16_t i = 0; i < *nbArgs; i++)
	{
		free(args[i]);
		args[i] = NULL;
	}

	*nbArgs = 0;
}


void parse_command(const char *const buffer, char **const args, uint16_t *const nbArgs)
{
	size_t size = 0;
	char *ptr = NULL;

	uint16_t i = 0;
	const size_t length = strlen(buffer);
	while(i < length)
	{
		while(buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\0')
		{
			i++;
		}

		if(buffer[i] == '\n')
		{
			break;
		}

		ptr = strstr(buffer + i, " ");
		size = strlen(buffer + i) - (ptr != NULL ? strlen(ptr) : 0);
		args[*nbArgs] = strndup(buffer + i, size);

		if(args[*nbArgs][size - 1] == '\n')
		{
			args[*nbArgs][size - 1] = '\0';
		}

		(*nbArgs)++;
		i += size + 1;
	}
}


bool command_validation(const char *const *const args, const uint16_t nbArgs, const char *const command, const uint8_t nbCommandArgs)
{
	if(strcmp(args[0], command) || nbArgs != nbCommandArgs)
	{
		return false;
	}

	else
	{
		return true;
	}
}


bool str_beginwith(const char *w, const char *s)
{
	while(*s)
	{
		if(*s != *w)
		{
			return false;
		}

		w++;
		s++;
	}

	return true;
}


bool prompt_yes_no(char *const buffer, char **const args, uint16_t *const nbArgs)
{
	while(1)
	{
		command_cursor();
		prompt_keyboard(buffer);
		free_args(args, nbArgs);
		parse_command(buffer, args, nbArgs);

		if(*nbArgs != 1)
		{
			continue;
		}

		else
		{
			if(!strcmp(args[0], "no"))
			{
				return false;
			}

			else
			{
				if(!strcmp(args[0], "yes") || !strcmp(args[0], "YES"))
				{
					return true;
				}
			}
		}
	}
}