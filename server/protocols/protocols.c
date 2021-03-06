#include "protocols.h"


void who(const int8_t sender)
{
	char buffer[BUFFER] = "ID(s) of other client(s) currently connected: ";
	char microBuff[8];

	for(uint8_t i = 0; i < MAX_CLIENTS; i++)
	{
		if(core_variables.clients[i].status == TAKEN && i != sender)
		{
			strncpy(microBuff, "", 8);
			snprintf(microBuff, 8, "\t%d", i);
			strncat(buffer, microBuff, 8);
		}
	}

	if(sender >= 0)
	{
		printf("\n\n[WIFSS] [Client %d] has just listed others connected clients.\n\n", sender);
		SSL_write(core_variables.clients[sender].ssl, buffer, BUFFER);
	}

	else
	{
		printf("\n[WIFSS] %s\n\n", buffer);
	}
}


void message(const char *command, const uint8_t sender_id)
{
	char copy[BUFFER]   = "";
	char buffer[BUFFER] = "";

	strncpy(buffer, get_second_args_group(command), BUFFER);
	sprintf(copy, "[Client %d] : \"%s\".", sender_id, buffer);
	broadcast(sender_id, copy);
	printf("\n\n[Client %d] says: \"%s\".\n\n", sender_id, buffer);
}


void whisper(const char *command, const uint8_t sender_id)
{
	char copy[BUFFER]   = "";
	char buffer[BUFFER] = "";

	int16_t idTemp = get_second_args_group_as_integer(command);
	strncpy(copy, get_third_arg_group(command), BUFFER);

	if(core_variables.clients[idTemp].status == TAKEN && sender_id != idTemp && idTemp >= 0 && idTemp < MAX_CLIENTS)
	{
		sprintf(buffer, "[Client %d] whispers: \"%s\".", sender_id, copy);
		SSL_write(core_variables.clients[idTemp].ssl, buffer, BUFFER);
		printf("\n\n[Client %d] whispers to [Client %d]: \"%s\".\n\n", sender_id, idTemp, copy);
	}
	else
	{
		sprintf(buffer, "%s", "Error: This client is not connected or its identifier is invalid. Take a look to \"help\".");
		SSL_write(core_variables.clients[sender_id].ssl, buffer, BUFFER);
		printf("\n\n[Client %d] tried to whisper to [Client %d]: \"%s\", but he is not connected.\n\n", sender_id, idTemp, copy);
	}
}


void broadcast(const uint8_t sender, const char *msg)
{
	for(uint8_t i = 0; i < MAX_CLIENTS; i++)
	{
		if(i != sender && core_variables.clients[i].status == TAKEN)
		{
			SSL_write(core_variables.clients[i].ssl, msg, BUFFER);
		}
	}
}


void disconnect(const char *buffer)
{
	int16_t idTemp = get_second_args_group_as_integer(buffer);

	if(idTemp == -1)
	{
		close_all_connections(); 
		printf("\n");
	}

	else if(idTemp >= 0 && idTemp < MAX_CLIENTS)
	{
		if(core_variables.clients[idTemp].status == TAKEN)
		{
			SSL_write(core_variables.clients[idTemp].ssl, DISCONNECT, BUFFER);
			close(core_variables.clients[idTemp].sock);
		}
		else
		{
			printf("\n\033[35m[WIFSS] This client is already offline. You can\'t disconnect him.\033[0m\n\n");
		}
	}

	else
	{
		printf("\n\033[35m[WIFSS] This client identifier is invalid.\033[0m\n\n");
	}
}


void close_all_connections(void)
{
	printf("\n\033[35m[WIFSS] Closing all connections...\033[0m");

	for(uint8_t i = 0; i < MAX_CLIENTS; i++)
	{
		if(core_variables.clients[i].status == TAKEN)
		{
			SSL_write(core_variables.clients[i].ssl, DISCONNECT, BUFFER);
		}

		printf(".");
	}

	printf(" done.\n");
}
