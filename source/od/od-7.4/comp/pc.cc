//	add support for multi files

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "../image/oid.hpp"
#include "pccomp.hpp"
#include "sqlcomp.hpp"
#include "stcomp.hpp"
#include "trcomp.hpp"
#include "hpcomp.hpp"
#include "taskmgr.hpp"

extern short VERSION;

static int img=0;
static int st=0;
static int gst=0;
static int sql=0;
static int hpg=0;
static int echo=0;
static int map=0;
static int help=0;
static int err=0;
static int pcp=0;
static int comptok=0;
static char fbase[20];
static char stfile[20];

static stcomp	scomp;
static trcomp	tcomp;
static sqlcomp	qcomp;
static hpcomp	hcomp;
static pccomp	pcomp;

FILE 	*ofile;

int main(int argc,char *argv[])
{
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
		*cp=0;
		strcpy(fbase,fname);
		*cp='.';
                }
        }
printf("Toadware Technologies Object Director Version %d.%d\n",
	(VERSION>>8),VERSION&0xff);
printf("Copyright Toadware Technologies Inc. (1993-2004)\n");
if (pcp)
	{
	if (gst)
		{
		tcomp.setecho(echo);
		tcomp.topcompile(fbase);
		}
	else
		{
		pcomp.setecho(echo);
		pcomp.setcomptok(comptok);
		tn=TM->getactive();
		TM->create_image(fbase); 
		err=pcomp.topcompile(tn,fbase);
		if (map) TM->showmap();
		if (!err) { TM->close_image(); }
		}
	}
if (st)
	{
	scomp.setecho(echo);
	tn=TM->getactive();
	TM->create_image(fbase); 
	err=scomp.topcompile(tn,fbase);
	if (map) TM->showmap();
	if (!err) { TM->close_image(); }
	}
if (sql)
	{
	qcomp.setcomptok(comptok);
	qcomp.setecho(echo);
	err=qcomp.topcompile(fbase);
	}
if (hpg)
	{
	hcomp.setcomptok(comptok);
	hcomp.setecho(echo);
	err=hcomp.topcompile(fbase);
	}
if (img)
	{
	TM->open_image(fbase);
	if (map) TM->showmap();
	TM->taskmsg();
        }
if (err) printf("Program terminated with errors\n");
return(err);
}
