#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "oid.hpp"
#include "file.hpp"
#include "basetab.hpp"
#include "scanner.hpp"

static scanner scan;

//#define MAIN

#define MAX_ENTS	80
static struct				/* base table */
	{
        char 	status[1];		/* class included */
	char	classname[20];		/* class name */
	char	objname[20];		/* object name */
        char 	desc[30];		/* description */
	} base[MAX_ENTS];		/* base table ents */


/*<>*/
basetab::basetab(void)
{
}

/*<>*/
void basetab::init(void)
/* initialize base table */
{
int i;
char *cp;
cp = (char *) &base;
for(i=0; i<sizeof(base); i++) cp[i]=0;
offset=0;
}

/*<>*/
int basetab::size(void)
{
return(sizeof(basetab));
}

/*<>*/
void basetab::first(void)
{
offset=0;
}

/*<>*/
void basetab::next(void)
{
offset+=20;
if (offset>=(MAX_ENTS-20)) offset=MAX_ENTS-20;
}

/*<>*/
void basetab::prev(void)
{
offset-=20;
if (offset<0) offset=0;
}

/*<>*/
char *basetab::get(long line,int fld)
{
char *cp;
cp="";
if ((offset+line)>=MAX_ENTS) return(cp);
switch(fld)
	{
case 0:
cp=base[line+offset].status;
break;

case 1:
cp=base[line+offset].classname;
break;

case 2:
cp=base[line+offset].objname;
break;

case 3:
cp=base[line+offset].desc;
break;

	}
return(cp);
}

/*<>*/
void basetab::set(long line,int fld,char *value)
{
if ((line+offset)>MAX_ENTS) return;
switch(fld)
	{
case 0:
base[line+offset].status[0]=value[0];
break;

case 1:
strncpy(base[line+offset].classname,value,sizeof(base[line+offset].classname));
break;

case 2:
strncpy(base[line+offset].objname,value,sizeof(base[line+offset].objname));
break;

case 3:
strncpy(base[line+offset].desc,value,sizeof(base[line+offset].desc));
break;

	}
}

/*<>*/
int basetab::load(char *fname)
{
file ft;
ft.setname(fname);
ft.open();
ft.read((char *)&base,sizeof(base));
ft.close();
return(0);
}

/*<>*/
int basetab::save(char *fname)
{
file ft;
ft.setname(fname);
ft.open();
ft.write((char *)&base,sizeof(base));
ft.close();
return(0);
}

/*<>*/
void basetab::write(char *fname)
{
int i;
FILE *fp;
fp=fopen(fname,"w");
if (!fp) return;
for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' || 
		base[i].status[0]=='&'))
                fprintf(fp,"%s\t\t'%s'\t'%s'\n",
			base[i].classname,
			base[i].objname,
			base[i].desc);
fclose(fp);
}

/*<>*/
void basetab::read(char *fname)
{
int i;
char *cp,*rc,*tp,line[100];
FILE *fp;
fp=fopen(fname,"r");
if (!fp) return;
cp = (char *) &base;
for(i=0; i<sizeof(base); i++) cp[i]=0;
rc=fgets(line,100,fp);
i=0;
while(rc)
	{
	if (line[0]!='#')
		{
		scan.set_line(line);	
		tp=scan.get_next();
		if (!tp) break;
		strncpy(base[i].classname,tp,sizeof(base[i].classname));
		tp=scan.get_next();
		if (!tp) break;
		strncpy(base[i].objname,tp,sizeof(base[i].objname));
		tp=scan.get_next();
		if (!tp) break;
		strncpy(base[i].desc,tp,sizeof(base[i].desc));
		base[i++].status[0]='*';
		}
	rc=fgets(line,100,fp);
	}
fclose(fp);
}

/*<>*/
void basetab::build(char *bn)
{
FILE *fp;
char bname[40];
int i,count,lcount;
sprintf(bname,"%s.cc",bn);
fp=fopen(bname,"w");
if (!fp) return;

// standard include files

fprintf(fp,"#include <stdio.h>\n");
fprintf(fp,"#include <string.h>\n");
fprintf(fp,"\n");
fprintf(fp,"union attr { float fattr; int iattr; };\n");
fprintf(fp,"\n");
fprintf(fp,"#include \"oid.hpp\"\n");
fprintf(fp,"#include \"%s.hpp\"\n",bn);

// class include files

for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' || 
		base[i].status[0]=='&'))
                fprintf(fp,"#include \"%s.hpp\"\n",base[i].classname);
fprintf(fp,"\n");

// build type list

count=0;
for(i=0; i<MAX_ENTS; i++) 
	if (base[i].status[0]=='*' || base[i].status[0]=='&') count++;
lcount=count;
fprintf(fp,"#define MAX_TYPES %d\n",count);
fprintf(fp,"static char *ctypes[MAX_TYPES] =\n\t{\n");
for(i=0; i<MAX_ENTS; i++)
        if (!strcmp(base[i].classname,"aclass"))
		{
		fprintf(fp,"\t\"class\",\n");
		count--;
		}
	else
        	{
        	if (base[i].classname[0] && (base[i].status[0]=='*' ||
			base[i].status[0]=='&'))
        		{
                	if (--count>0)
	        	        fprintf(fp,"\t\"%s\",\n",base[i].classname);
			else                        
         			fprintf(fp,"\t\"%s\"\n",base[i].classname);
                	}
		}
fprintf(fp,"\t};\n\n");
		
// build constructor, get, set

fprintf(fp,"/*<>*/\n");
fprintf(fp,"%s::%s(void)\n",bn,bn);
fprintf(fp,"{\n");
fprintf(fp,"init();\n");
fprintf(fp,"}\n\n");

fprintf(fp,"/*<>*/\n");
fprintf(fp,"char *%s::get(int type,long line,int fld)\n",bn);
fprintf(fp,"{\n");
fprintf(fp,"return(\"\");\n");
fprintf(fp,"}\n\n");

fprintf(fp,"/*<>*/\n");
fprintf(fp,"void %s::set(int type,long line,int fld,char *value)\n",bn);
fprintf(fp,"{\n");
fprintf(fp,"}\n\n");

// build lookup routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"int %s::gettype(char *atype)\n",bn);
fprintf(fp,"{\n");
fprintf(fp,"/* gets the type of a base class */\n");
fprintf(fp,"int i;\n");
fprintf(fp,"for(i=0; i<MAX_TYPES; i++)\n");
fprintf(fp,"        if (!strcmp(atype,ctypes[i])) return(i);\n");
fprintf(fp,"return(-1);\n");
fprintf(fp,"}\n\n");

// build getname routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"char *%s::getname(int cn)\n",bn);
fprintf(fp,"{\n");
fprintf(fp,"/* gets the name of a base class */\n");
fprintf(fp,"if (cn>=MAX_TYPES) return(\"\");\n");
fprintf(fp,"return(ctypes[cn]);\n");
fprintf(fp,"}\n\n");

// build size routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"int %s::getsize(int type)\n",bn);
fprintf(fp,"/* gets the size of a base class */\n");
fprintf(fp,"{\n");
fprintf(fp,"int size;\n");
fprintf(fp,"size = 0;\n");
fprintf(fp,"switch(type)\n");
fprintf(fp,"        {\n");
count=0;
for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' ||
		base[i].status[0]=='&'))
        	{
		fprintf(fp,"case %d:\n",count++);
		if (base[i].objname[0])
                	fprintf(fp,"size = 0;\t/*  %s */\n",
                        	base[i].classname);
		else
                	fprintf(fp,"size = ((%s *) 0)->size();\n",
                		base[i].classname);
		fprintf(fp,"break;\n\n");
                }
fprintf(fp,"        }\n");
fprintf(fp,"return(size);\n");
fprintf(fp,"}\n\n");

// build init routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"void %s::init(void)\n",bn);
fprintf(fp,"/* initialize static classes */\n");
fprintf(fp,"{\n");
fprintf(fp,"}\n\n");

// build message routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"long %s::sendmsg(int sn,int type,char *obj,int msg,int num,oid parm[])\n",bn);
fprintf(fp,"/* send a message to an object of a class */\n");
fprintf(fp,"{\n");
fprintf(fp,"long rc;\n");
fprintf(fp,"rc=0;\n");
fprintf(fp,"switch(type)\n");
fprintf(fp,"        {\n");
count=0;
for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' || 
		base[i].status[0]=='&'))
        	{
		fprintf(fp,"case %d:\n",count++);
		if (base[i].objname[0])
                	fprintf(fp,"rc = %s->message(sn,msg,num,parm);\n",
                		base[i].objname);
		else
               		fprintf(fp,"rc = ((%s *) obj)->message(sn,msg,num,parm);\n",
                		base[i].classname);
                fprintf(fp,"break;\n\n");
		}
fprintf(fp,"        }\n");
fprintf(fp,"return(rc);\n");
fprintf(fp,"}\n\n");

// build get routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"char *%s::getobj(int type,char *obj,long lne,int fld)\n",bn);
fprintf(fp,"/* send a get to an object of a class */\n");
fprintf(fp,"{\n");
fprintf(fp,"char *rc;\n");
fprintf(fp,"rc=\"\";\n");
fprintf(fp,"switch(type)\n");
fprintf(fp,"        {\n");
count=0;
for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' ||
		base[i].status[0]=='&'))
        	{
                fprintf(fp,"case %d:\n",count++);
		if (base[i].objname[0])
               		fprintf(fp,"rc = %s->get(lne,fld);\n",
               			base[i].objname);
		else
               		fprintf(fp,"rc = ((%s *) obj)->get(lne,fld);\n",
               			base[i].classname);
               	fprintf(fp,"break;\n\n");
		}
fprintf(fp,"        }\n");
fprintf(fp,"return(rc);\n");
fprintf(fp,"}\n\n");

// build set routine

fprintf(fp,"/*<>*/\n");
fprintf(fp,"void %s::setobj(int type,char *obj,long lne,int fld,char *value)\n",bn);
fprintf(fp,"/* send a set to an object of a class */\n");
fprintf(fp,"{\n");
fprintf(fp,"switch(type)\n");
fprintf(fp,"        {\n");
count=0;
for(i=0; i<MAX_ENTS; i++)
        if (base[i].classname[0] && (base[i].status[0]=='*' ||
		base[i].status[0]=='&'))
        	{
		fprintf(fp,"case %d:\n",count++);
		if (base[i].objname[0])
               		fprintf(fp,"%s->set(lne,fld,value);\n",
               			base[i].objname);
		else
               		fprintf(fp,"((%s *) obj)->set(lne,fld,value);\n",
               			base[i].classname);
               	fprintf(fp,"break;\n\n");
		}
fprintf(fp,"        }\n");
fprintf(fp,"}\n\n");
fclose(fp);
}

#ifdef MAIN

basetab bt;

int main(int argc,char *argv[])
{
int i;
char bname[40],*cp;
for(i=1; i<argc; i++)
	{
//printf("argv[%d]=%s\n",i,argv[i]);
	}
if (argc<2) 
	printf("usage: bt <basename>\n"); 
else
	{
	strncpy(bname,argv[1],sizeof(bname));
	bt.read(bname);
	cp=strchr(bname,'.');
	if (!cp) exit(0);
	*cp=0;
	bt.build(bname);
	}
}
#else
/*<>*/
long basetab::message(int sn,int msg,int num,oid parm[])
{
long rc;
int ip=0;
char *cp = "";
if (num)
	{
//        ip = parm[0].getseg();
//	if (parm[0].gettype()==OT_DEFINED) cp = parm[0].get(0,0);
        }
rc=0;
switch(msg)
	{
case 0:
init();
break;

case 1:
rc=load(cp);
break;

case 2:
rc=save(cp);
break;

case 3:
build(cp);
break;

case 4:
first();
break;

case 5:
next();
break;

case 6:
prev();
break;

case 7:
write(cp);
break;

case 8:
read(cp);
break;

default:
rc = -1;
break;
	}
return(rc);
}
#endif
