#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "symtab.hpp"

#define MAX_OBJS	250

symtab::symtab(void)
{
//init();
}

void symtab::init(void)
{
int i;
for(i=0; i<MAX_OBJS; i++) name[i].init();
} 

int symtab::size(void)
{
return(sizeof(symtab));
}

void symtab::set(int line,int fld,char *value)
/* sets an attribute value */
{
}

char *symtab::get(int line,int fld)
/* returns an attribute value */
{
return("");
}


oid symtab::getid(int on)
{
static oid empty;
if (on>=MAX_OBJS) return(empty);
if (!*name[on].getname()) return(empty);
return(name[on].getid());
}

void symtab::setid(int on,oid id)
{
if (on>=MAX_OBJS) return;
name[on].setid(id);
}


/*<>*/
void symtab::delete_symbol(int on)
{
if (on>=MAX_OBJS) return;
name[on].init();
}

/*<>*/
int symtab::create_symbol(char *oname,oid cid,int size)
{
int i;
for(i=0; i<MAX_OBJS; i++)
	if (!strcmp(name[i].getname(),oname)) return(-2);
for(i=0; i<MAX_OBJS; i++)
	{
	if (!*name[i].getname())
		{
//printf("symtab create_entry succeded for %s %d\n",oname,i);
		name[i].init();
		name[i].setname(oname);
		name[i].setcid(cid);
		name[i].setsize(size);
		return(i);
		}
	}
//printf("symtab create_entry failed for %s\n",oname);
return(-1);
}

/*<>*/
int symtab::lookup_symbol(char *oname)
{
int i;
int on;
for(i=0; i<MAX_OBJS; i++)
	{
	if (name[i].test(oname))
		return(i);
	}
return(-1);
}

/*<>*/
void symtab::setname(oid on,char *cname)
{
int i;
oid id;
for(i=0; i<MAX_OBJS; i++)
	{
	id=name[i].getid();
        if (id.getitem()==on.getitem() && id.getseg()==on.getseg()) 
        	{ name[i].setname(cname); return; }
	}
}

/*<>*/
char *symtab::getname(int on)
{
int i;
oid id;
if (on<MAX_OBJS) return(name[on].getname()); 
return(0);
}

/*<>*/
char *symtab::getname(oid on)
{
int i;
oid id;
for(i=0; i<MAX_OBJS; i++)
	{
	id=name[i].getid();
        if (id.getitem()==on.getitem() && id.getseg()==on.getseg() && id.getitem()>=0) 
        	return(name[i].getname()); 
	}
return("");
}

/*<>*/
void symtab::show(char *iname)
{
FILE *fp;
int i,size;
oid id,cid;
char *nm;
char fn[20];
sprintf(fn,"%s.sym",iname);
fp=fopen(fn,"w");
if (!fp) return;
for(i=0; i<MAX_OBJS; i++) 
	{
	nm=name[i].getname();
	id = name[i].getid();
	cid = name[i].getcid();
	size = name[i].getsize();
	if (nm[0]) 
		{
		fprintf(fp,"num %3d id %8s size %4d type %s",i,id.aschar(),size,id.astype());
		fprintf(fp,"cid %8s type %s name %s\n",cid.aschar(),cid.astype(),nm);
		}
	}
fclose(fp);
}

/*<>*/
oid symtab::getcid(int on)
{
static oid empty;
if (on>=MAX_OBJS) return(empty);
if (!*name[on].getname()) return(empty);
return(name[on].getcid());
}

/*<>*/
int symtab::getsize(int on)
{
if (on>=MAX_OBJS) return(0);
return(name[on].getsize());
}

/*<>*/
long symtab::message(int msg,int num,oid parm[])
/* object level message routine */
{
int rc;
rc=0;
switch(msg)
	{
case 0:
init();
break;
	}
return(rc);
}
