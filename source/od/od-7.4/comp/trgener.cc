/*
ST translator generator
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "oid.hpp"
#include "syslog.hpp"
#include "trgener.hpp"

extern FILE *ofile;

trgener::trgener(void)
{
init();
}

void trgener::init(void)
{
name[0]=0;
level=0;
inswitch=0;
notype=0;
}

/*<>*/
void trgener::generate(int code,char *parm,int line)
{
int i,rc;
char *np;
//printf("pgen: case %d parm %s\n",code,parm);
switch(code)
	{
case 1:		/* add a method to a class */
if (notype)
	strncpy(metname,type,sizeof(metname));
else
	strncpy(metname,name,sizeof(metname));
fprintf(ofile,"%s addMethod: '%s'.\n",cname,metname);
break;

case 21:	/* method call after switch --- doesn't happen now */
strncpy(name,type,sizeof(name));

case 2:		/* send a message to an object */
np=strchr(type,'.');
if (np)
	{
	*np=0;
	np++;
	fprintf(ofile,"%s %s",type,np);
	}
else
	fprintf(ofile,"self %s",type);
break;

case 3:		/* generate a message parameter */
if (parm[0]==')')
	fprintf(ofile,".\n");
else
	{
	if (isdigit(parm[0]) || (parm[0]=='-' && isdigit(parm[1])))
		{
		if (parm[1]=='x')
			fprintf(ofile,": 16r%s",&parm[2]);
		else
			fprintf(ofile,": %s",parm);
		}
	else
		fprintf(ofile,": '%s'",parm);
	}
break;

case 4:		/* save the parameter in name */
strncpy(name,parm,sizeof(name));
notype=0;
break;

case 5:		/* create a new class */
fprintf(ofile,"\n%s <- class new.\n",parm);
strncpy(cname,parm,sizeof(cname));
break;

case 6:		/* create a new object */
fprintf(ofile,"%s <- %s new.\n",cname,type);
break;

case 7:		/* save the parameter in type */
strncpy(type,parm,sizeof(type));
notype=1;
break;

case 8:		/* "c" method name (main) */
strncpy(metname,cname,sizeof(metname));
fprintf(ofile,"cstart <- class new.\n");
strncpy(cname,"cstart",sizeof(cname));
strncpy(metname,"main",sizeof(metname));
fprintf(ofile,"%s addMethod: '%s'.\n",cname,metname);
//fprintf(ofile,"start <- cstart new.\n");
fprintf(ofile,"%s setMethod: %s as:\n[\n",cname,metname);
break;

case 9:				/* end of block */
if (inswitch) { inswitch=0; }
if (--level>0) break;  /* end of switch */
if (level<0)
	{
	sprintf(msg,"on line %d unmatched }",line);
	SL->log_error(msg);
	level=0;
	}
fprintf(ofile,"]\n");
break;

case 10:	/* set method name and classid */
strncpy(metname,name,sizeof(metname));
fprintf(ofile,"\n%s setMethod: %s as:\n[\n",type,metname);
break;

case 11:	/* start of block */
level++;
break;

case 12:	/* start of switch --- now attribute size */
//inswitch=1;
break;

case 13:	/* case value --- add attribute to class */
//fprintf(ofile,"%s=%s\nifTrue:\n[\n",swval,parm);
fprintf(ofile,"%s addAttribute: '%s'.\n",cname,name);
break;

case 14:	/* break --- copy type to cname */
//fprintf(ofile,"]\n");
strncpy(cname,type,sizeof(cname));
break;

case 15:	/* set a parent class */
fprintf(ofile,"%s setParent: '%s'.\n",cname,parm);
break;

case 16:	/* save the parameter in type and set type */
strncpy(type,parm,sizeof(type));
break;

case 17:	/* save the parameter in cname */
strncpy(cname,parm,sizeof(cname));
break;

case 22:	/* set switch value */
//strncpy(swval,parm,sizeof(swval));
//fprintf(ofile,":%s |\n",swval);
break;
       }
return;
}
