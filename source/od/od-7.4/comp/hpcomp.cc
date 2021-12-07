/*
hpp compiler
*/

#include <string.h>
#include <stdio.h>

#include "oid.hpp"
#include "hpcomp.hpp"
#include "hpgener.hpp"
#include "scanner.hpp"
#include "syslog.hpp"

static scanner scan;
extern FILE *ofile;
static hpgener hgen;

#define MAX_DEFS 50
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
/* class statements */

	0,	0,	10,	"class",	// class definition


	10,	5,	11,	"<Symbol>",	// class name 
	11,	0,	12,	":",
	11,	0,	12,	"extends",
	12,	0,	12,	"public",
	12,	0,	11,	"<Symbol>",	// parent name 
	11,	0,	13,	"{",
	13,	23,	13,	"public",
	13,	24,	13,	"private",
	13,	0,	13,	"protected",
	13,	0,	13,	"native",

	13,	7,	16,	"<Symbol>",	// type 
	13,	0,	13,	":",
	13,	0,	13,	"~",		// distructor 
	13,	0,	15,	"}",
	15,	2,	0,	";",
	16,	4,	16,	"<Symbol>",	// type  
	16,	0,	16,	"*",
	16,	13,	13,	";",		// end of item 
	16,	13,	19,	"[",		// attr 
	16,	1,	17,	"(",		// method 

	17,	0,	18,	"<Symbol>",	// parm type 
	17,	0,	20,	")",		
	18,	0,	18,	"*",
	18,	0,	18,	"<Symbol>",	// parm name 
	18,	0,	17,	",",
	18,	0,	19,	"[",
	18,	0,	13,	";",
	18,	0,	20,	")",		
	19,	0,	19,	"<Int>",	// number 
	19,	0,	18,	"]",
	20,	0,	13,	";",
	20,	0,	0,	"{",


#define MAX_TOKENS	120	
        };	 

hpcomp::hpcomp(void)
{
init();
}

/*<>*/
void hpcomp::init(void)
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
int hpcomp::topcompile(char *fbase)
{
int err;
char stfile[20]; 
char fname[20]; 
printf("HPP Compiler v1.0\n");
#ifdef MPP
sprintf(stfile,"%s.mpp",fbase);
#else
sprintf(stfile,"%s.cd",fbase);
#endif
sprintf(fname,"%s.hpp",fbase);
ofile=fopen(stfile,"w");
err=compile(fname);
fclose(ofile);
return (err);
}

/*<>*/
void hpcomp::setecho(int ec)
{
echo=ec;
}

/*<>*/
void hpcomp::setcomptok(int compt)
{
comptok=compt;
}

/*<>*/
void hpcomp::help(void)
{
int link;
for(link=0; link<number; link++) 
	{
        if (toklist[link].parent==level)
	        SL->log_error(toklist[link].name);
        }
}

/*<>*/
int hpcomp::token(char *tok,int type)
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
       	       	                	hgen.generate(
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
long hpcomp::compline(char *line)
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
//		printf("compiling on\n");                                
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
			hgen.setgen(0);
	  	       	compile(cp);
			hgen.setgen(1);
                        }
		return(0);
                }
        if (!strcmp(parm,"define"))
        	{
		parm=scan.get_next();
                strncpy(deftab[numdefs].name,parm,sizeof(deftab[0].name));
//              printf("defining <%s> %d\n",deftab[numdefs].name,numdefs);
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
//				printf("found %s compiling off\n",parm);
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
	rc=token(parm,type);
//	printf("rc=%d\n",rc);
       	if (rc<0) 
               	{
                sprintf(msg,"on line %d at %d %s invalid",curline,level,parm);
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
int hpcomp::compile(char *name)
/* import command file */
{
int rc,ll;
FILE *infile;
char filetext[400],*r;
hgen.setname(name);
infile=fopen(name,"r");
//printf("hpcomp compiling %s file=%p\n",name,infile);
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
	if (echo) printf("%s\n",filetext); 
	rc=compline(filetext);
	if (rc<0) { fclose(infile); return(rc); }
	r=fgets(filetext,sizeof(filetext),infile);
	}
fclose(infile);
//scan.set_code(46,1);
return(SL->get_errors());
}

