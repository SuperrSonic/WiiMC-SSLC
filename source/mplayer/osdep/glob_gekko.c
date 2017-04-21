/*
 * rodries
 */

#include <sys/types.h>
#include <stdio.h>

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>

#include "glob.h"

int glob(const char *pattern, int flags,
          int (*errfunc)(const char *epath, int eerrno), glob_t *pglob)
{
 	//bad patch
 	char *filename;
 	filename=strdup(pattern);
 	filename[strlen(filename)-1]='\0';
    pglob->gl_pathv = malloc(sizeof(char*));
    pglob->gl_pathv[0] = filename;
    pglob->gl_pathc++;

    return 0;
	
}

void globfree(glob_t *pglob)
{
	int i;
	for(i=0; i <pglob->gl_pathc ;i++)
	{
		free(pglob->gl_pathv[i]);
	}
	free(pglob->gl_pathv);
}

