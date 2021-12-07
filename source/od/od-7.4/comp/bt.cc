#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "oid.hpp"
#include "basetab.hpp"

static short VERSION = 0x0402;

static int help=0;
static int bt=0;
static char fbase[20];
static basetab 	bcomp;

FILE 	*savefile;

int main(int argc,char *argv[])
{
long i;
char *cp;
char fname[20];
fname[0]=0;
for(i=1; i<argc; i++)
	{
//printf("argv[%d]=%s argc=%d\n",i,argv[i],argc);
	if (!strcmp(argv[i],"-?")) help=1;
	if (argv[i][0]!='-') strcpy(fname,argv[i]); 
	}
if (help)
	{
	printf("usage: bt <filename>.bt\n");
	printf("options: -?	     produce this message\n");
	exit(0);
	}
if (fname[0]) 
	{
        cp=strchr(fname,'.');
        if (cp)
        	{
                if (!strcmp(&cp[1],"bt")) bt++; 
		*cp=0;
		strcpy(fbase,fname);
		*cp='.';
                }
        }
printf("Toadware Technologies Object Director Version %d.%d\n",
	(VERSION>>8),VERSION&0xff);
printf("Copyright Toadware Technologies Inc. (1993-2005)\n");
if (bt)
	{
	bcomp.read(fname);
	bcomp.build(fbase);
	}
return(0);
}
