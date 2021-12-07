/*
pc++ compiler
*/

#include <string.h>
#include <stdio.h>

#include "oid.hpp"
#include "pccomp.hpp"
#include "pcgener.hpp"
#include "scanner.hpp"
#include "syslog.hpp"

static scanner scan;
static pcgener pgen;

#define MAX_DEFS 81
static struct			/* define table */
	{
	char	name[20];	
	} deftab[MAX_DEFS];

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

	0,	0,	10,	"class",	// class definition
	0,	36,	1,	"while",	// ( <expr> ) <stmt>
	0,	37,	1,	"for",		// ( expr; expr; expr ) <stmt>
	0,	33,	1,	"if",		// ( <expr> ) <stmt>
	0,	38,	0,	"else",		// <stmt>
	0,	39,	40,	"return",	// <expr> ;
	0,	40,	1,	"switch",	// ( <expr> ) {
	0,	41,	2,	"case",		// <int> :
	0,	0,	3,	"default",	// :
	0,	42,	0,	"break",	
	0,	7,	20,	"<Symbol>",	// not a keyword 
	0,	11,	0,	"{",		// start of block 
	0,	9,	0,	"}",		// end of block 

	0,	0,	0,	";",		// end of statement 
	1,	0,	40,	"(",		// ( <expr>  
	2,	0,	3,	"<Int>",
	3,	0,	0,	":",

/* class statement */

	10,	5,	11,	"<Symbol>",	// class name 
	11,	0,	12,	":",
	11,	0,	12,	"extends",
	12,	0,	12,	"public",
	12,	15,	11,	"<Symbol>",	// parent name 
	11,	0,	13,	"{",
	13,	0,	13,	"public",
	13,	23,	13,	"private",
	13,	0,	13,	"protected",
	13,	0,	13,	"native",

	13,	7,	16,	"<Symbol>",	// type 
	13,	0,	13,	":",
	13,	0,	13,	"~",		// distructor 
	13,	0,	15,	"}",
	15,	0,	0,	";",
	16,	4,	16,	"<Symbol>",	// type  
	16,	0,	16,	"*",
	16,	13,	13,	";",		// end of item 
	16,	13,	19,	"[",		// attr 
	16,	1,	17,	"(",		// method 

	17,	16,	18,	"<Symbol>",	// parm type 
	17,	0,	35,	")",		
	18,	0,	18,	"*",
	18,	4,	18,	"<Symbol>",	// parm name 
	18,	0,	17,	",",
	18,	0,	19,	"[",
	18,	0,	13,	";",
	18,	0,	35,	")",		
	19,	12,	19,	"<Int>",	// number 
	19,	0,	18,	"]",

	35,	0,	13,	";",
	35,	0,	0,	"{",

/* symbol expresions */

	20,	17,	21,	"<Symbol>",	// define object/method 
	20,	0,	20,	"*",		// ignore 
	20,	14,	22,	":",		// define method 
	20,	2,	30,	"(",		// exec method 
	20,	18,	40,	"=",		// assignment 
	20,	18,	41,	"-",		// - assignment 
	20,	18,	41,	"+",		// + assignment 
	20,	18,	41,	"*",		// * assignment 
	20,	18,	41,	"/",		// / assignment 
	20,	0,	40,	"<",		// < test 

	20,	0,	40,	">",		// > test 
	20,	18,	40,	"<=",		// <= test 
	20,	18,	40,	">=",		// >= test 
	21,	8,	25,	"(",		// define "c" method 
	21,	6,	0,	";",		// define object 
	21,	6,	20,	",",		// define object 

/* function definition */

	21,	0,	22,	":",		// define method 
	21,	0,	36,	"[",		// define size 
	22,	0,	23,	":",
	23,	4,	24,	"<Symbol>",	// function name 
	24,	10,	25,	"(",		// cpp method 
	25,	0,	26,	"void",
	25,	0,	29,	")",
	25,	7,	27,	"<Symbol>",	// parm type 
	27,	0,	27,	"*",		
	27,	4,	28,	"<Symbol>",	// parm name

	26,	0,	29,	")",
	28,	0,	29,	")",
	28,	0,	25,	",",
	29,	11,	0,	"{",		// start of block 
	36,	 0,	37,	"<Int>",	// set object size	 
	37,	 0,	21,	"]",		 

/* exec method - parameters */

	30,	3,	30,	"<Symbol>",
	30,	3,	30,	"<Int>",
	30,	3,	30,	"<Float>",
	30,	3,	30,	"<Char>",
	30,	3,	30,	"<Hex>",
	30,	0,	30,	"[",
	30,	0,	30,	"]",
	30,	0,	30,	",",
	30,	3,	0,	")",

/* expression evaluation */

	40,	40,	40,	"<Symbol>",
	40,	19,	40,	"<Int>",
	40,	0,	40,	"<Float>",
	40,	0,	40,	"<Char>",
	40,	0,	40,	"<Hex>",
	40,	18,	40,	"=",		
	40,	0,	40,	"+",
	40,	0,	40,	"-",
	40,	0,	40,	"*",
	40,	0,	40,	"/",

	40,	0,	40,	"(",
	40,	0,	40,	")",
	40,	0,	40,	"<",
	40,	0,	40,	">",
	40,	0,	40,	"!=",
	40,	0,	40,	"<=",
	40,	0,	40,	">=",
	40,	0,	40,	"==",
	40,	0,	40,	"!",
	40,	0,	40,	",",

	40,	0,	40,	"&",
	40,	0,	40,	"[",
	40,	0,	40,	"]",
	40,	0,	40,	"]=",
	40,	0,	40,	"|",
	40,	0,	40,	">>",
	40,	0,	40,	"<<",
	40,	0,	0,	";",
	41,	0,	40,	"=",
	41,	20,	40,	"-",

	41,	20,	40,	"+"

#define MAX_TOKENS	120	
        };	 

pccomp::pccomp(void)
{
init();
}

/*<>*/
void pccomp::init(void)
{
curline=0;
number=MAX_TOKENS;
level=0;
comptok=0;
compsw=1;
numdefs=0;
paren=0;
echo=0;
}

/*<>*/
int pccomp::topcompile(int tn,char *fbase)
{
int err;
char fname[20]; 
sprintf(fname,"%s.pcp",fbase);
printf("PC: Compiling %s\n",fname);
sprintf(fname,"%s.lst",fbase);
SL->set_file(fname);
sprintf(fname,"%s.pcp",fbase);
err=compile(tn,fname);
return (err);
}

/*<>*/
void pccomp::setecho(int ec)
{
echo=ec;
}

/*<>*/
void pccomp::setcomptok(int compt)
{
comptok=compt;
}

/*<>*/
void pccomp::help(void)
{
int link;
for(link=0; link<number; link++) 
	{
        if (toklist[link].parent==level)
	        SL->log_error(toklist[link].name);
        }
}

/*<>*/
int pccomp::token(int tn,char *tok,int type)
{
int link,i,rc;
//printf("token called <%s> %d\n",tok,type);
for(link=0; link<number; link++)
	{

	/* if the token is on my level */

        if (toklist[link].parent==level)	
		{
                
                /* if the token matches or the type matches */
                
                if (!strcmp(tok,toklist[link].name) ||
			(toklist[link].name[0]=='<' &&
   				toklist[link].name[1]==type ))
				{
				if (comptok)
				    printf("level=%2d tok=<%s>\n",level,tok);
				if (toklist[link].code)
					{
       	       	                	pgen.generate(tn,
						toklist[link].code,
                       	               		tok,curline,type);
					}
                                if (toklist[link].move==40)
					{
					if (level==40)	// the expr wart
						{
						if (tok[0]=='(') paren++;
						if (tok[0]==')') paren--;
						}
					else
						paren=0;
					}
                                level=toklist[link].move;
				if (paren<0) { level=0; paren=0;  }
                                return(0);
                                }
		}                
        }
return(-1);
}

/*<>*/
long pccomp::compline(int tn,char *line)
{
char *cp,*ep;
int size,i;
long rc=0;
char *parm;
int type;
curline++;
cp=strchr(line,'/');
if (cp && *(cp+1) == '/') *cp=0;
scan.set_line(line);
parm=scan.get_next();
if (line[0]=='#') 
        {
	parm=scan.get_next();
        if (!strcmp(parm,"endif"))
        	{
               	compsw=1;
//		printf("pccomp: compiling on\n");                                
		return(0);
                }
	if (!compsw) return(0);                
        if (!strcmp(parm,"include"))
                {
                cp=strchr(line,'"');
                if (cp)
                       	{
                        cp++;
                	ep=strchr(cp,'"');
			if (ep) *ep=0;
	  	       	compile(tn,cp);
                        }
		return(0);
                }
        if (!strcmp(parm,"define"))
        	{
		parm=scan.get_next();
                strncpy(deftab[numdefs].name,parm,sizeof(deftab[0].name));
//              printf("pccomp: defining <%s> %d\n",deftab[numdefs].name,numdefs);
                if (numdefs<MAX_DEFS) 
                	{
                	numdefs++;
                        return(0);
                        }
		else
                	{	
	                sprintf(msg,"on line %d MAXDEFS exceeded",curline);
			SL->log_error(msg);
                        return(1);
                        }                        	
                }
        if (!strcmp(parm,"ifndef"))
        	{
		parm=scan.get_next();
                for(i=0; i<numdefs; i++)
                	{
                	if (!strcmp(parm,deftab[i].name))
                        	{
                        	compsw=0;
//				printf("pccomp: found %s compiling off\n",parm);
                                }
                        }
		return(0);
                }
	return(0);
	}                                
if (!compsw) return(0);
while(parm)
	{
	type=scan.get_type();
	rc=token(tn,parm,type);
//	printf("pccomp: parm %s rc=%d\n",parm,rc);
       	if (rc<0) 
               	{
                sprintf(msg,"on line %d at level %d %s invalid",curline,level,parm);
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
int pccomp::compile(int tn,char *name)
/* import command file */
{
int rc,ll;
FILE *infile;
char filetext[400],*r;
pgen.setname(name);
infile=fopen(name,"r");
//printf("compiling %s file=%p\n",name,infile);
if (!infile) 
	{
        sprintf(msg,"input file %s not found",name);
	SL->log_error(msg);
	return(1);
        }
scan.set_code(46,8);	// set . to character class
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
//scan.set_code(46,1);
return(SL->get_errors());
}

