/*
ST Translator
*/
#include <string.h>
#include <stdio.h>

#include "oid.hpp"
#include "scanner.hpp"
#include "trcomp.hpp"
#include "trgener.hpp"
#include "syslog.hpp"

#define MAX_STACK	10	// size of operator stack

extern FILE *ofile;
static scanner scan;
static trgener tgen;

typedef struct
	{
	int	parent;		/* my parent */
	int	code;		/* generate code */
        int	move;		/* move to location */
        char 	*name;		/* name or type of command */
        } token;

static token toklist[] = 
	{
/* all statements */

	0,	0,	10,	"class",
	0,	12,	40,	"switch",
	0,	0,	47,	"case",
	0,	0,	48,	"default",
	0,	9,	0,	"}",
	0,	14,	49,	"break",
	0,	0,	2,	"return",
	0,	0,	1,	"while",
	0,	0,	1,	"if",
	0,	0,	1,	"for",

	0,	0,	0,	"else",
	0,	11,	0,	"{",
	0,	0,	0,	";",
	0,	7,	20,	"<Symbol>",	/* not a keyword */
	1,	0,	50,	"(",
	2,	0,	0,	";",
	2,	0,	50,	"(",

/* class statement */

	10,	5,	11,	"<Symbol>",	/* class name */
	11,	0,	12,	":",
	12,	15,	11,	"<Symbol>",	/* parent name */

	11,	0,	13,	"{",
	13,	0,	14,	"public",
	13,	0,	14,	"private",
	13,	0,	14,	"protected",
	13,	7,	16,	"<Symbol>",	/* type */
	14,	0,	13,	":",
	13,	0,	13,	"~",		/* distructor */
	13,	0,	15,	"}",
	15,	0,	0,	";",
	16,	4,	16,	"<Symbol>",	/* type  */

	16,	0,	16,	"*",
	16,	19,	13,	";",
	16,	19,	19,	"[",		/* attr */
	16,	1,	17,	"(",		/* method */
	17,	16,	18,	"<Symbol>",	/* name */
	18,	0,	18,	"*",
	18,	4,	18,	"<Symbol>",	/* parm name */
	18,	0,	35,	")",
	18,	0,	17,	",",
	18,	0,	19,	"[",

	18,	0,	13,	";",
	19,	20,	19,	"<Int>",	/* number */
	19,	0,	18,	"]",

	35,	0,	13,	";",

/* symbol expresions */

	20,	17,	21,	"<Symbol>",	
	20,	0,	20,	"*",	
	20,	0,	50,	"+",	
	20,	0,	50,	"-",	
	20,	18,	22,	":",		/* define method */
	20,	21,	30,	"(",		/* exec method */
	20,	0,	50,	"=",		/* expression */

	20,	0,	50,	"[",		/* array */
	21,	8,	25,	"(",		/* define "c" method */
	21,	6,	0,	";",		/* define object */
	21,	6,	20,	",",		/* define object */
	21,	0,	22,	":",		/* define method */
	22,	0,	23,	":",
	23,	4,	24,	"<Symbol>",	/* method name */
	24,	10,	25,	"(",		/* cpp method */
	25,	0,	26,	"void",
	25,	7,	27,	"<Symbol>",	/* parm type */

	27,	0,	27,	"*",		
	27,	4,	28,	"<Symbol>",	/* parm name */
	26,	0,	29,	")",
	28,	0,	29,	")",
	28,	0,	25,	",",
	29,	11,	0,	"{",		/* start of method */

/* exec method - currently coding parameters */

	30,	3,	30,	"<Symbol>",
	30,	3,	30,	"<Int>",
	30,	3,	30,	"<Float>",
	30,	3,	30,	"<Char>",

	30,	3,	30,	"<Hex>",
	30,	0,	30,	"[",
	30,	0,	30,	"]",
	30,	0,	30,	",",
	30,	3,	0,	")",

/* switch statement */

	40,	0,	41,	"(",		/* switch */
	41,	22,	42,	"<Symbol>",
	42,	0,	43,	")",
	43,	11,	0,	"{",
	47,	13,	48,	"<Int>",	/* case */

	48,	0,	0,	":",
	49,	0,	0,	";",		/* break */

/* expression - 50+ range code is operator number */

	50,	0,	50,	"<Symbol>",
	50,	0,	50,	"<Int>",
	50,	0,	50,	"<Float>",
	50,	0,	50,	"<Char>",
	50,	0,	50,	"<Hex>",
	50,	20,	50,	"+",
	50,	21,	50,	"-",
	50,	30,	50,	"*",

	50,	31,	50,	"/",
	50,	50,	50,	"(",
	50,	51,	50,	")",
	50,	1,	50,	"<",
	50,	2,	50,	">",
	50,	3,	50,	"!=",
	50,	4,	50,	"<=",
	50,	5,	50,	">=",
	50,	6,	50,	"==",
	50,	0,	50,	"!",

	50,	0,	50,	";",	
	50,	0,	50,	",",
	50,	0,	50,	"&",
	50,	0,	50,	"[",
	50,	0,	50,	"=",
	50,	0,	50,	"]",
	50,	0,	50,	"]=",
	50,	0,	0,	"}",
	50,	0,	50,	"|",
	50,	0,	50,	">>",

	50,	0,	50,	"<<",

#define MAX_TOKENS	112
        };	 

trcomp::trcomp(void)
{
init();
}

/*<>*/
void trcomp::init(void)
{
curline=0;
number=MAX_TOKENS;
level=0;
echo=0;
}

/*<>*/
int trcomp::topcompile(char *fbase)
{
int err;
char stfile[20]; 
char fname[20]; 
printf("ST Translator v1.0\n");
sprintf(stfile,"%s.st",fbase);
sprintf(fname,"%s.pcp",fbase);
ofile=fopen(stfile,"w");
err=compile(fname);
fclose(ofile);
return (err);
}

/*<>*/
void trcomp::setecho(int ec)
{
echo=ec;
}

/*<>*/
void trcomp::help(void)
{
int link;
for(link=0; link<number; link++) 
	{
        if (toklist[link].parent==level)
	        SL->log_error(toklist[link].name);
        }
}

/*<>*/
int trcomp::token(char *tok,int type)
{
int link,i,rc;
if (tok[0]=='(') nest++;
if (tok[0]==')') 
	{
	if (--nest<=0 && level>=50)
		{
//		reduce(0);
		level=0;
		nest=0;
		return(0);
		}
	}
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
//				printf("level=%2d tok=<%s>\n",level,tok);
//				if (toklist[link].move>=50)
//					exprtok(tok,type,toklist[link].code);
//				else
					{
					if (toklist[link].code)
        	       	                	tgen.generate(
							toklist[link].code,
                        	               		tok,curline);
					}
                                level=toklist[link].move;
                                return(0);
                                }
		}                
        }
return(-1);
}

/*<>*/
long trcomp::compline(char *line)
{
char *cp;
int size,ll;
long rc=0;
char *parm;
int type;
curline++;
cp=strchr(line,'/');
if (cp && *(cp+1) == '/')
	{
	fprintf(ofile,"\"%s\"\n",cp+2);
	*cp=0;
	}
scan.set_line(line);
parm=scan.get_next();
if (line[0]=='#') 
        {
        if (strcmp(&line[1],"include"))
                {
                cp=strchr(line,'"');
                if (cp)
                       	{
                        cp++;
                        ll=strlen(cp);
                        cp[ll-1]=0;
	  	       	compile(cp);
                        }
                }
	return(0);
	}                                
while(parm)
	{
	type=scan.get_type();
	rc=token(parm,type);
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
int trcomp::compile(char *name)
{
int rc,ll;
FILE *infile;
char filetext[200],*r;
scan.set_code(46,8);	// set . to character class
infile=fopen(name,"r");
if (!infile) return(0);
r=fgets(filetext,sizeof(filetext),infile);
while(r)
	{
	ll=strlen(filetext);
	filetext[ll-1]=0;
	if (echo) printf("%s\n",filetext); 
	rc=compline(filetext);
	if (rc<0) { fclose(infile); return(rc); }
	r=fgets(filetext,sizeof(filetext),infile);
	}
fclose(infile);
//scan.set_code(46,1);
return(SL->get_errors());
}

