#include <string.h>

void trim(char* line)
{
    int l;

    l = strlen(line );
    while (line[l-1] == '\n' || line[l-1] == '\r') line[--l] = '\0';
}

