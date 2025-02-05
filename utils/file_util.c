#include "file_util.h"
#include <stdio.h>
#include <stdlib.h>


void writeToFile(const char *filePath, char *value) 
{
    FILE *file = fopen(filePath, "w");
    if (file == NULL) 
    {
        perror("Unable to open the file...!!");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", value);
    fclose(file);
}
