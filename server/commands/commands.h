#ifndef __COM__
#define __COM__


#include "../core/server.h"

#define OK        "ok"
#define FAIL      "fail"
#define ENDT      "ENDT"
#define PRESENT   "present"
#define FINISHED  "finished"

#define WHO        "who"
#define HELP       "help"
#define EXIT       "exit"
#define SEND       "send"
#define CLEAR      "clear"
#define WHISPER    "whisper"
#define DISCONNECT "disconnect"

#define ARGDISCONNECT 2


void process_command(const char*, const uint8_t);
void* on_connection(void*);
void* command_handler(void*);
void* connections_handler(void*);


#endif
