/*
sql compiler 
*/

#include <string.h>
#include <stdio.h>

#include "oid.hpp"
#include "sqlcomp.hpp"
#include "scanner.hpp"
#include "syslog.hpp"

static scanner scan;
extern FILE *ofile;

#define MAX_DEFS 60
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

	0,	0,	100,	"create",	// create statement 
	0,	0,	200,	"insert",	// insert statement 
	0,	0,	300,	"select",	// select statement 

/* create statement */

	100,	0,	101,	"table",	// create table
	100,	0,	109,	"view",		// create view
	101,	0,	102,	"<Symbol>",	// table name 
	102,	0,	103,	"(",	
	102,	0,	0,	";",	
	103,	0,	104,	"<Symbol>",	// column name
	104,	0,	105,	"<Symbol>",	// column type
	105,	0,	106,	"(",	
	105,	0,	103,	",",	
	105,	0,	108,	")",	
	106,	0,	107,	"<Int>",	// column size
	107,	0,	105,	")",	
	108,	0,	0,	";",
	109,	0,	110,	"<Symbol>",	// view name 
	110,	0,	111,	"as",	 
	110,	0,	112,	"(",	 
	111,	0,	300,	"select",	 
	112,	0,	113,	"<Symbol>",	// view colname 
	113,	0,	112,	",",	
	113,	0,	110,	")",	

/* insert statement */

		
	200,	0,	201,	"into",		
	201,	0,	202,	"<Symbol>",	// table name 
	202,	0,	203,	"values",		
	203,	0,	204,	"(",		
	204,	0,	205,	"<Char>",
	204,	0,	205,	"<Int>",
	204,	0,	205,	"<Float>",
	205,	0,	204,	",",
	205,	0,	206,	")",	
	206,	0,	0,	";",
	
/* select statement */

	300,	0,	301,	"*",
	300,	0,	302,	"<Symbol>",	// colname or table
	301,	0,	305,	"from",
	302,	0,	300,	",",	
	302,	0,	303,	".",	
	303,	0,	304,	"<Symbol>",	// colname
	304,	0,	300,	",",
	305,	0,	306,	"<Symbol>",	// table
	306,	0,	305,	",",	
	306,	0,	0,	";",	
	306,	0,	307,	"where",	
	307,	0,	308,	"<Symbol>",	// colname
	308,	0,	309,	"=",	
	309,	0,	310,	"<Int>",	
	309,	0,	310,	"<Char>",	
	309,	0,	310,	"<Float>",	
	310,	0,	307,	"and",	
	310,	0,	307,	"or",	
	310,	0,	0,	";",	
	

#define MAX_TOKENS	52	
        };	 

sqlcomp::sqlcomp(void)
{
init();
}

/*<>*/
void sqlcomp::init(void)
{
curline=0;
level=0;
comptok=0;
compsw=1;
numdefs=0;
echo=0;
}

/*<>*/
int sqlcomp::topcompile(char *fbase)
{
int err;
char stfile[20]; 
char fname[20]; 
printf("SQL Compiler v1.0\n");
sprintf(stfile,"%s.img",fbase);
sprintf(fname,"%s.sql",fbase);
ofile=fopen(stfile,"w");
err=compile(fname);
fclose(ofile);
return (err);
}

/*<>*/
void sqlcomp::setecho(int ec)
{
echo=ec;
}

/*<>*/
void sqlcomp::setcomptok(int compt)
{
comptok=compt;
}

/*<>*/
void sqlcomp::genprint(void)
{
//gen.genprint();
}

/*<>*/
void sqlcomp::help(void)
{
int link;
for(link=0; link<MAX_TOKENS; link++) 
	{
        if (toklist[link].parent==level)
	        SL->log_error(toklist[link].name);
        }
}

/*<>*/
int sqlcomp::token(char *tok,int type)
{
int link,i,rc;
//printf("token called <%s> %d\n",tok,type);
for(link=0; link<MAX_TOKENS; link++)
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
       	       	                	//gen.generate(
					//	toklist[link].code,
                       	               	//	tok,curline,type);
					}
                                level=toklist[link].move;
                                return(0);
                                }
		}                
        }
return(-1);
}

/*<>*/
long sqlcomp::compline(char *line)
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
	  	       	compile(cp);
			scan.set_code(46,8);
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
int sqlcomp::compile(char *name)
/* import command file */
{
int rc,ll;
FILE *infile;
char filetext[400],*r;
//gen.setname(name);
infile=fopen(name,"r");
//printf("compiling %s file=%p\n",name,infile);
if (!infile) 
	{
        sprintf(msg,"input file %s not found",name);
	SL->log_error(msg);
	return(1);
        }
scan.set_code(46,8); // set . to character class
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

