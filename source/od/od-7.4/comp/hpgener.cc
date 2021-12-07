/*
hpp code generator
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "oid.hpp"
#include "hpgener.hpp"

#define MAX_SYMS 50
static struct			/* method table */
	{
	char	name[20];	
	} mettab[MAX_SYMS];

static struct			/* attrib table */
	{
	char	name[20];	
	} attrtab[MAX_SYMS];

extern FILE *ofile;

hpgener::hpgener(void)
{
init();
}

void hpgener::init(void)
{
cname[0]=0;
name[0]=0;
type[0]=0;
notype=0;
pub=0;
genlevl=0;
nummet=0;
numattr=0;
metgen=1;
}

/*<>*/
void hpgener::setgen(int gn)
{
// only generate on level 0 
if (gn) genlevl--; else genlevl++;
if (genlevl<0) genlevl=0;
}

/*<>*/
void hpgener::setname(char *nm)
{
char *np;
if (genlevl) return;
strncpy(compname,nm,sizeof(compname));
np=strchr(compname,'.');
if (np) *np=0;
}

/*<>*/
void hpgener::generate(int code,char *parm,int line,int ptype)
{
int i;
if (!genlevl) switch(code)
	{
case 1:		/* add a method <name> to a class */
if (!metgen || !pub) return;
if (!strcmp(name,"getmet")) { metgen=0; return; }
if (nummet>=MAX_SYMS) { printf("hpgener: max symbols exceeded\n"); return; }
if (notype)
	strncpy(mettab[nummet++].name,type,sizeof(mettab[nummet].name));
else
	strncpy(mettab[nummet++].name,name,sizeof(mettab[nummet].name));
break;

case 2:		 // end of class */
#ifdef MPP
fprintf(ofile,"#define NUMMETS %d\n",nummet);
fprintf(ofile,"static char *mettab[NUMMETS] =\n\t{\n");
fprintf(ofile,"\t\"init\",\n");
for(i=1; i<nummet; i++)
	{
	fprintf(ofile,"\t\"%s\"",mettab[i].name);
	if (i<(nummet-1)) fprintf(ofile,",");
	if (strlen(mettab[i].name)<5) fprintf(ofile,"\t");
	fprintf(ofile,"\t// %d\n",i);
	}
fprintf(ofile,"\t};\n\n");
fprintf(ofile,"char *%s::getmet(int mn)\n{\n",compname);
fprintf(ofile,"if (mn>=NUMMETS) return(\"\");\n");
fprintf(ofile,"return(mettab[mn]);\n}\n\n");
fprintf(ofile,"#define NUMATTR %d\n",numattr);
fprintf(ofile,"static char *attrtab[NUMATTR] =\n\t{\n");
for(i=0; i<numattr; i++)
	{
	fprintf(ofile,"\t\"%s\"",attrtab[i].name);
	if (i<(numattr-1)) fprintf(ofile,",\n"); else fprintf(ofile,"\n");
	}
fprintf(ofile,"\t};\n\n");
fprintf(ofile,"char *%s::getattr(int mn)\n{\n",compname);
fprintf(ofile,"if (mn>=NUMATTR) return(\"\");\n");
fprintf(ofile,"return(attrtab[mn]);\n}\n\n");
#else
fprintf(ofile,"class %s\n\t{\n",cname);
fprintf(ofile,"void\tinit(void);\n");
for(i=1; i<nummet; i++)
	fprintf(ofile,"void\t%s(void);\n",mettab[i].name);
fprintf(ofile,"\t};\n");
#endif
break;

case 4:		/* save the parameter in name */
strncpy(name,parm,sizeof(name));
notype=0;
break;

case 5:		/* create a new class */
strncpy(cname,parm,sizeof(cname));
break;

case 7:		/* save the parameter in type */
strncpy(type,parm,sizeof(type));
notype=1;
break;

case 13:	/* add an attrib to a class */
if (pub) return;
if (numattr>=MAX_SYMS) { printf("hpgener: max symbols exceeded\n"); return; }
strncpy(attrtab[numattr++].name,name,sizeof(attrtab[numattr].name));
break;

case 23:
pub=1;
break;

case 24:
pub=0;
break;

       }
return;
}
