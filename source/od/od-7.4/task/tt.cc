#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "oid.hpp"
#include "taskmgr.hpp"

extern short VERSION;

static int bt=0;
static int st=0;
static int gst=0;
static int sql=0;
static int hpg=0;
static int echo=0;
static int map=0;
static int help=0;
static int err=0;
static int img=0;
static int pcp=0;
static int comptok=0;
static char fbase[20];
static char stfile[20];

FILE 	*ofile;

int main(int argc,char *argv[])
{
oid id,cid,nid;
int tmd,on;
long i,rc,tn;
char *cp;
char fname[20];
fname[0]=0;
for(i=1; i<argc; i++)
	{
//printf("argv[%d]=%s argc=%d\n",i,argv[i],argc);
	if (!strcmp(argv[i],"-?")) help=1;
	if (!strcmp(argv[i],"-l")) echo=1;
	if (!strcmp(argv[i],"-m")) map=1;
	if (!strcmp(argv[i],"-s")) gst=1;
	if (!strcmp(argv[i],"-t")) comptok=1;
	if (argv[i][0]!='-') strcpy(fname,argv[i]); 
	}
if (help)
	{
	printf("usage: pc <filename>.pcp|<filename>.img\n");
	printf("options: -?	     produce this message\n");
	printf("         -l          produce listing\n");
	printf("         -m          produce object map\n");
	printf("         -s          convert to smalltalk\n");
	printf("         -t          print compiler tokens\n");
	exit(0);
	}
if (fname[0]) 
	{
        cp=strchr(fname,'.');
        if (cp)
        	{
                if (!strcmp(&cp[1],"img")) img++;
                if (!strcmp(&cp[1],"pcp")) pcp++;
                if (!strcmp(&cp[1],"sql")) sql++;
                if (!strcmp(&cp[1],"st"))  st++;
                if (!strcmp(&cp[1],"hpp")) hpg++; 
                if (!strcmp(&cp[1],"bt")) bt++; 
		*cp=0;
		strcpy(fbase,fname);
		*cp='.';
                }
        }
printf("Toadware Technologies Object Director Version %d.%d\n",
	(VERSION>>8),VERSION&0xff);
printf("Copyright Toadware Technologies Inc. (1993-2005)\n");

/******************** image functions ********************/

tmd=TM->create_image("test_image");
TM->close_image();
tmd=TM->open_image("test_image");
//TM->setimgname(tmd,"test_image_rename");
for (i=0; i<100; i++)
	{
	id=TM->create_object(tmd,cid,100);
//	TM->setparent(tmd,id,nid);
	nid.item++;
//	TM->setclass(tmd,id,nid);
	}
//printf("Image name %s\n",TM->getimgname(tmd));

/******************** symbol functions ********************/

for (i=0; i<10; i++)
	{
	sprintf(fname,"symbol-%02d",i);
	on=TM->create_symbol(tmd,fname,cid,100);
	}

/******************** object functions ********************/

for (i=0; i<10; i++)
	{
	sprintf(fname,"object-%02d",i);
	id=TM->create_object(tmd,cid,0);
	}

TM->showmap();
TM->close_image();

if (err) printf("Program terminated with errors\n");
return(err);
}
