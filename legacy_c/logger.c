#include <stdio.h>
#include "logger.h"

void write_log(char *message)
{
    FILE *logs;

    logs = fopen("logs/system.log", "a");

    if(logs == NULL){
        return;
    }

    fprintf(logs, "%s\n", message);

    fclose(logs);
}