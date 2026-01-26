#include "command.h"

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#include "pico/stdlib.h"
#include "pico/stdio.h"



uint32_t characters_recived = 0;
char buffer[MAX_COMMAND_LENGTH];
command_status_t command_status = NO_COMMAND;
    
void poll_command(void)
{
    int16_t ch = getchar_timeout_us(100);
    while (ch != PICO_ERROR_TIMEOUT)
    {
        printf("%c", ch);
        buffer[characters_recived++] = (char) ch;
        if ((char)ch == COMMAND_START)
        {
            command_status = COMMAND_FORMING;
        }
        else if ((char)ch == COMMAND_END && command_status == COMMAND_FORMING)
        {
            command_status = COMMAND_READY;
            // todo process command here
        }
        ch = getchar_timeout_us(100);
    }
}