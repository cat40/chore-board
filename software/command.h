#ifndef COMMAND_H
#define COMMAND_H

#define MAX_COMMAND_LENGTH 2048

#define COMMAND_START   '#'
#define COMMAND_END     '!'

typedef enum {NO_COMMAND, COMMAND_FORMING, COMMAND_READY} command_status_t;

void poll_command(void);

#endif