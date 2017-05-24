#include "commands.h"


void* server_communication(void *param)
{
	int16_t result;
	char buffer[BUFFER];

	THREADS *threads = (THREADS*)param;

	while(1)
	{
		strncpy(buffer, "", BUFFER);

		result = recv(g_core_variables.server_sock, buffer, BUFFER, false);

		if(result <= 0 || !strcmp(buffer, DISCONNECT))
		{
			printf("\n\n[sthread] Server is demanding this client disconnection. Stopping now.");
			pthread_cancel(*(threads->cthread));
			pthread_exit(NULL);
		}

		else
		{
			/* We received from server a non-null string, let's print it */
			if(strcmp(buffer, ""))
			{
				printf("\n\n[sthread] %s\n\n", buffer);
				command_cursor();
			}
		}
	}
}


void* client_communication(void *param)
{
	THREADS *threads = (THREADS*)param;

	char buffer[BUFFER];
	char *args[BUFFER];
	uint16_t nbArgs;

	while(1)
	{
		command_cursor();
		prompt_keyboard(buffer);
		free_args(args, &nbArgs);
		parse_command(buffer, args, &nbArgs);

		// In order to avoid a segmentation fault if 'args' is empty...
		if(nbArgs == 0)
		{
			continue;
		}

		if(command_validation((const char* const*)args, nbArgs, EXIT, 1) || command_validation((const char* const*)args, nbArgs, LOGOUT, 1))
		{
			pthread_cancel(*(threads->sthread));
			pthread_exit(NULL);
		}

		else if(str_beginwith(buffer, SEND))
		{
			send(g_core_variables.server_sock, buffer, BUFFER, false);
		}

		else if(str_beginwith(buffer, WHISPER))
		{
			send(g_core_variables.server_sock, buffer, BUFFER, false);
		}

		else if(command_validation((const char* const*)args, nbArgs, WHO, 1))
		{
			char temp[BUFFER] = WHO;
			send(g_core_variables.server_sock, temp, BUFFER, false);
		}

		else if(command_validation((const char* const*)args, nbArgs, CLEAR, 1))
		{
			clear_console();
		}

		else if(command_validation((const char* const*)args, nbArgs, HELP, 1))
		{
			static const char *const helpMenu[32] =
			{
				"help",
				"who",
				"send <message>",
				"whisper <idClient> <message>",
				"exit",
				"logout",
				"clear",
				"\n"
			};

			for(uint8_t i = 0; helpMenu[i] != NULL; i++)
			{
				printf("\t%s\n", helpMenu[i]);
			}
		}

		else
		{
			printf("\nUnknown command. Try \"help\" for further information.\n\n");
		}
	}
}
