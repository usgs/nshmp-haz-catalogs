#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#define LINESIZE 512


void Parse_Text(FILE *fp, const char *querystring, const char type, void* result);

/**
 * parsetext: Parse a text file and return the value of a match string.
 *
 *
 *
 */
void Parse_Text(FILE *fp, const char *querystring, const char type,
              void* result){
    int32_t res = 0, found = 0;

    /* Start from the beginning */
    rewind(fp);

    /* Look for the string until found */
    while (!found) {
        char line[LINESIZE];
        char delimiters[] = " =\n\r";
        char *name, *value;

        /* Read in one line */
        if (fgets(line, LINESIZE, fp) == NULL)
            break;

        name = strtok(line, delimiters);
        if ((name != NULL) && (strcmp(name, querystring) == 0)) {
            found = 1;
            value = strtok(NULL, delimiters);

            switch (type) {
            case 'i':
                res = sscanf(value, "%d", (int *)result);
                break;
            case 'I':
                res = sscanf(value, "%d", (int32_t *)result);
                break;
            case 'f':
                res = sscanf(value, "%f", (float *)result);
                break;
            case 'd':
                res = sscanf(value, "%lf", (double *)result);
                break;
            case 's':
                res = 1;
                strcpy((char *)result, value);
                break;
            default:
                fprintf(stderr, "parsetext: unknown type %c\n", type);
                exit(1);
            }
        }

    }

    if (found == 0){
        fprintf(stderr,"\n !!! CANNOT FIND %s IN INPUT FILE\n", querystring);
        fprintf(stderr,"Using default value for calculation.\n");
    }
    if (res == -1){
        fprintf(stderr, "parsetext: unknown type %c\n", type);
        exit(1);
    }

    return ;
}
