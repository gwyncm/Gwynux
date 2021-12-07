/*
smalltalk compiler
*/

#include <string.h>
#include <stdio.h>

#include "oid.hpp"
#include "scanner.hpp"
#include "stcomp.hpp"
#include "stgener.hpp"
#include "syslog.hpp"

static scanner scan;
static stgener sgen;

typedef struct
	{
	int	parent;		/* my parent */
	int	code;		/* generate code */
        int	move;		/* move to location */
        char 	*name;		/* name or type of command */
        } token;

static token toklist[] = 
	{

	0,	1,	0,	"<Symbol>",	
	0,	1,	0,	"<Char>",
	0,	1,	0,	"<Int>",
	0,	2,	0,	"[",
	0,	3,	0,	"]",
	0,	0,	1,	"<",
	1,	4,	0,	"-",
	0,	5,	0,	".",
	0,	6,	0,	":"
	
#define MAX_TOKENS	9
        };	 

stcomp::stcomp(void)
{
init();
}

/*<>*/
void stcomp::init(void)
{
curline=0;
number=MAX_TOKENS;
level=0;
echo=0;
}

/*<>*/
int stcomp::topcompile(int tn,char *fbase)
{
int err;
char fname[20]; 
sprintf(fname,"%s.lst",fbase);
SL->set_file(fname);
sprintf(fname,"%s.st",fbase);
printf("ST Compilier v1.0 compiling %s\n",fname);
err=compile(tn,fname);
return (err);
}

/*<>*/
void stcomp::setecho(int ec)
{
echo=ec;
}

/*<>*/
void stcomp::help(void)
{
int link;
for(link=0; link<number; link++) 
	{
        if (toklist[link].parent==level)
	        SL->log_error(toklist[link].name);
        }
}

/*<>*/
int stcomp::token(int tn,char *tok,int type)
{
int link,i,rc;
for(link=0; link<number; link++)
	{

	/* if the token is on my level */

        if (toklist[link].parent==level)	
		{
                
                /* if the token matches or the type matches */
                
                if (!strcmp(tok,toklist[link].name) ||
			(toklist[link].name[0]=='<' &&
   				(toklist[link].name[1]==type ||
	                	toklist[link].name[1]=='P' )))		
				{
				// printf("level=%2d tok=<%s>\n",level,tok);
				if (toklist[link].code)
        	       	               	sgen.generate(tn,
						toklist[link].code,
                        	       		tok,curline,type);
                                level=toklist[link].move;
                                return(0);
                                }
		}                
        }
return(-1);
}

/*<>*/
long stcomp::compline(int tn,char *line)
{
char *cp;
long rc=0;
char *parm;
int type;
curline++;
cp=strchr(line,'"');	// look for comments
if (cp) *cp=0;
scan.set_line(line);
parm=scan.get_next();
while(parm)
	{
	type=scan.get_type();
	rc=token(tn,parm,type);
       	if (rc<0) 
               	{
                sprintf(msg,"on line %d %s invalid",curline,parm);
		SL->log_error(msg);
                SL->log_error("Expecting");
                help();
                }
	if (rc>0) rc=0; 
	parm=scan.get_next();
        }
return(rc);
}

/*<>*/
int stcomp::compile(int tn,char *name)
/* import command file */
{
int rc,ll;
FILE *infile;
char filetext[200],*r;
infile=fopen(name,"r");
//printf("compiling %s\n",name);
if (!infile) 
	{
        sprintf(msg,"Input file %s not found",name);
	SL->log_error(msg);
	return(1);
        }
r=fgets(filetext,sizeof(filetext),infile);
while(r)
	{
	ll=strlen(filetext);
	filetext[ll-1]=0;
	if (echo) SL->log_message(filetext); 
	rc=compline(tn,filetext);
	if (rc<0) { fclose(infile); return(rc); }
	r=fgets(filetext,sizeof(filetext),infile);
	}
fclose(infile);
return(SL->get_errors());
}
