#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    DIR*            FD;
    struct dirent*  in_file;
    FILE            *entry_file;
    char            buffer[BUFSIZ];
	int             a=0;
    char            *str;
    int             i=0;

    
    str = calloc(sizeof(char), 12);
    if (NULL == (FD = opendir (argv[1]))) 
    {
        fprintf(stderr, "Error : %s\n", strerror(errno));
        return 1;
    }
    while ((in_file = readdir(FD)) && (strstr(in_file->d_name, ".log")))
    {
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue; 
        entry_file = fopen(in_file->d_name, "r");
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : %s\n", strerror(errno));
            return 1;
        }
        while (fgets(buffer, BUFSIZ, entry_file) != NULL)
        {
			if (!(strncmp("\"20", buffer, 3)) && !i)
            {
               str = strncpy(str, buffer, 12);
               i++;
            }
            if (strncmp(str, buffer, 12) && i)
            {
                str++;
                if (a)
                    printf ("%s= %d\n", str, a);         
                if (!(strncmp("\"20", buffer, 3)) && (buffer[27] == 'E' 
                    || buffer[27] == 'C' || buffer[27] == 'F'))
                {
                    a = 1;
                    str = strncpy(str, buffer, 12);
                }
                else
                {
                    i = 0;
                    a = 0;
                }
            }
            else if (!strncmp(str, buffer, 12) && (buffer[27] == 'E' 
                || buffer[27] == 'C' || buffer[27] == 'F'))
                a++;
        }
            str++;
            if (a)
                printf ("%s= %d\n", str, a);
        fclose(entry_file);
    }
    return 0;
}