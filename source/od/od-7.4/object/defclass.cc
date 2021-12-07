#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "oid.hpp"
#include "defclass.hpp"

#define MAX_ATTRS 60

extern FILE *savefile;

/*<>*/
void defclass::init(void)
/* initialize a class */
{
int i;
nummet=1;
numattr=0;
basetype=0;
objsize=0;
parent[0]=0;
classname[0]=0;
for(i=0; i<MAX_ATTRS; i++) attrlist[i].init();
}

/*<>*/
int defclass::size(void)
/* return object size */
{
return(sizeof(defclass));
}

/*<>*/
int defclass::addmet(void)
/* add a method to a class */
{
int i;
//printf("defclass: addmet %d attrs %d\n",nummet,numattr);
if ((nummet+numattr)>=MAX_ATTRS) return(-1);
for (i=numattr+nummet; i>nummet; i--)
	attrlist[i]=attrlist[i-1];
attrlist[nummet].init();
return(nummet++);
}

/*<>*/
int defclass::delattr(int an)
/* delete an attribute from a class */
{
int i;
oid aid;
// currently disabled
return(0);
if (an>=MAX_ATTRS || an<0) return(-1);
for (i=an; i<numattr; i++)
	attrlist[i]=attrlist[i+1];
numattr--;
getsize();	// recalculates offsets
}

/*<>*/
int defclass::addattr(void)
/* add an attribute to a class */
{
//printf("defclass: addattr %d mets %d\n",numattr,nummet);
if ((nummet+numattr)>=MAX_ATTRS) return(-1);
attrlist[nummet+numattr].init();
numattr++;
getsize();	// recalculates offsets
return(numattr++);
}

/*<>*/
char *defclass::getname(void)
{
return(classname);
}

/*<>*/
void defclass::setname(char *cname)
{
strncpy(classname,cname,sizeof(classname));
setmetname(0,cname);
}

#ifdef OID
/*<>*/
oid defclass::getattrname(int na)
{
oid empty;
if (na<0 || na>=numattr) return(empty);
return(attrlist[na].getname());
}
#endif

#ifdef OID
/*<>*/
oid defclass::getmetname(int na)
{
oid empty;
if (na<0 || na>=nummet) return(empty); 
return(attrlist[nummet+na].getname());
}
#endif

/*<>*/
oid defclass::getattrref(int na)
{
oid empty;
if (na<0 || (nummet+na)>=MAX_ATTRS) return(empty);
return(attrlist[nummet+na].getref());
}

/*<>*/
void defclass::setattrref(int an,oid rid)
{
int rc;
if (an<0 || (nummet+an)>=MAX_ATTRS) return;
attrlist[nummet+an].setref(rid);
}

/*<>*/
char *defclass::getattrtype(int na)
{
if (na<0 || (nummet+na)>=MAX_ATTRS) return("");
return(attrlist[nummet+na].getdatatype());
}

/*<>*/
void defclass::setbasetype(int ty)
/* set the type of a class */
{
basetype=ty;
//printf("defclass: basetype set to %d\n",ty);
}

/*<>*/
int defclass::getbasetype(void)
/* return the type of a class */
{
//printf("defclass: get basetype %d\n");
return(basetype);
}

/*<>*/
void defclass::setsize(int size)
/* sets the size of a class */
{
objsize=size;
}

/*<>*/
int defclass::getsize(void)
/* return the size of a class */
{
int i,attrsize;
if (basetype>0) return(objsize);
attrsize=0;
for (i=0; i<numattr; i++)
	{
	attrlist[i].setoffset(attrsize);
	attrsize+=attrlist[i].getlength();
	}
return(attrsize);
}

/*<>*/
int defclass::getattrlen(int an)
/* get length of an attribute in a class */
{
if (an<0 || an>=numattr) return(0);
return(attrlist[nummet+an].getlength());
}

/*<>*/
void defclass::setattrlen(int an,int len)
/* set the length of an attribute */
{
int i;
if (an<0 || an>=numattr) return;
attrlist[nummet+an].setlength(len);
getsize();	// recalculate offsets
}

/*<>*/
int defclass::getoffset(int an)
/* get the offset of an attribute */
{
if (an<0 || an>=numattr) return(-1);
return(attrlist[nummet+an].getoffset());
}

/*<>*/
char *defclass::getdatatype(int id)
/* get the type of an attribute */
{
if (id<0 || id>=numattr) return(0);
return(attrlist[nummet+id].getdatatype());
}

/*<>*/
int defclass::setblockid(int mn,oid mid)
/* set a bockid for a method */
{
if (mn<0 || mn>=nummet) return(-1);
attrlist[mn].setref(mid);
return(0);
}

/*<>*/
int defclass::getblockid(int msg)
/* get a bockid for a method */
{
oid empty;
if (msg<0 || msg>=nummet) return(empty.asint());
return(attrlist[msg].getref().asint());
}

/*<>*/
void defclass::setparent(char *pname)
{
strncpy(parent,pname,sizeof(parent));
// In case this is a reset
nummet=1;
numattr=0;
}

/*<>*/
char *defclass::getparent(void)
{
return(parent);
}

/*<>*/
int defclass::getnummet(void)
{
return(nummet);
}

/*<>*/
void defclass::setmetname(int mn,char *mname)
{
if (mn<0 || mn>=nummet) return;
//printf("defclass setmetname %d %s\n",mn,mname);
attrlist[mn].setname(mname);
}

/*<>*/
void defclass::setattrname(int an,char *aname)
{
if (an<0 || an>=numattr) return;
//printf("defclass setattrname %d %s\n",an,aname);
attrlist[nummet+an].setname(aname);
}

void defclass::set(int line,int fld,char *value)
/* sets an attribute field */
{
switch(fld)
	{
	case CA_ATTRNAME:	
	setattrname(line,value);
	break;

	case CA_DATATYPE:
	if (line<0 || line>=numattr) return;
	attrlist[line].setdatatype(value);
	break;

	case CA_LENGTH:		
	setattrlen(line,atoi(value));
	break;

	case CA_OFFSET:
	if (line<0 || line>=numattr) return;
	attrlist[line].setoffset(atoi(value));
	break;

	case CA_METNAME:	
	setmetname(line,value);
	break;

	case CA_PRECISION:
	if (line<0 || line>=numattr) return;
	attrlist[line].setprecision(atoi(value));
	break;

	case CA_KEY:
	if (line<0 || line>=numattr) return;
	attrlist[line].setkey(atoi(value));
	break;

	case CA_FIELDTYPE:
	if (line<0 || line>=numattr) return;
	attrlist[line].setfieldtype(value);
	break;

	case CA_NULL:
	if (line<0 || line>=numattr) return;
	attrlist[line].setnull(value);
	break;

	case CA_ACCESS:
	if (line<0 || line>=numattr) return;
	attrlist[line].setaccess(value);
	break;

	case CA_ATTRREF:
	break;

	case CA_PARENT:
	setparent(value);
	break;

	case CA_CLASSNAME:
	setname(value);
	break;
	
	case CA_BASETYPE:	
	setbasetype(atoi(value));
	break;

	}
}

char *defclass::get(int line,int fld)
/* returns an attribute field */
{
oid ap,mp,bid;
static char result[10];
switch(fld)
	{
	case CA_ATTRNAME:
	if (line<0 || line>=numattr) return("");
	return(attrlist[line].getname());

	case CA_DATATYPE:
	if (line<0 || line>=numattr) return("");
	return(getdatatype(line));

	case CA_LENGTH:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%d",getattrlen(line));
	return(result);

	case CA_OFFSET:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%d",getoffset(line));
	return(result);

	case CA_METNAME:
	if (line<0 || line>=nummet) return("");
	return(attrlist[line].getname());

	case CA_PRECISION:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%d",attrlist[line].getprecision());
	return(result);

	case CA_KEY:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%d",attrlist[line].getkey());
	return(result);

	case CA_FIELDTYPE:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%c",attrlist[line].getfieldtype());
	return(result);

	case CA_NULL:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%c",attrlist[line].getnull());
	return(result);

	case CA_ACCESS:
	if (line<0 || line>=numattr) return("");
	sprintf(result,"%c",attrlist[line].getaccess());
	return(result);

	case CA_NUMATTR:
	sprintf(result,"%d",numattr);
	return(result);

	case CA_ATTRREF:
	return("");		

	case CA_PARENT:
	return(parent);
	break;

	case CA_OBJSIZE:
	sprintf(result,"%d",getsize());
	return(result);

	case CA_CLASSNAME:
	return(classname);
	break;

	case CA_BLOCKID:
	if (line<0 || line>=nummet) return("");
	bid.setint(getblockid(line));
	sprintf(result,"%s",bid.aschar());
	return(result);

	case CA_BASETYPE:
	sprintf(result,"%d",basetype);
	return(result);
	}
return("");
}

// code generation

/*<>*/
void defclass::genclass(void)
/* generate class definition */
{
int i,length;
defclass *cp;
oid aid,mid,bid;
char *mp,*ap,*tp;
//printf("defclass genclass %s\n",name);
if (parent[0])
	fprintf(savefile,"class %s : %s\n\t{\n",classname,parent);
else	
	fprintf(savefile,"class %s\n\t{\n",classname);
for (i=0; i<numattr; i++)
	{
	tp=getdatatype(i); 
	if (strcmp(tp,"parent"))
		{
		ap=attrlist[i].getname();
		fprintf(savefile,"\t%s %s",getdatatype(i),ap); 
		length=getattrlen(i);
		if (length) fprintf(savefile,"[%d]",length);
		fprintf(savefile,";\n");
		}
	}
fprintf(savefile,"public:\n");
for (i=1; i<nummet; i++)
	{
	bid.setint(getblockid(i));
	if (bid.item>=0) 
		{
		mp=attrlist[i].getname();
		if (strcmp(mp,classname))
			fprintf(savefile,"void\t%s(void);\n",mp);
		else
			fprintf(savefile,"\t%s(void);\n",mp);
		}
	}
fprintf(savefile,"\t%s(void);\n",classname);
fprintf(savefile,"\t};\n\n");
}

/*<>*/
long defclass::message(int sn,int msg,int num,oid parm[])
/* send a message to a class */
/* session number ignored    */
/* used only for class class */
{
long rc;
int ip=0;
oid bid;
if (num) ip = parm[0].getseg();
rc=0;
switch(msg)
	{
case 0:
init();
break;

case 1:
rc=addmet();
break;

case 2:
rc=addattr();
break;

case 4:
//reset();
break;

case 5:
//rc=getattrname(ip);
break;

case 6:
//rc=getmetname(ip);
break;

case 11:
return(getsize());

case 16:
if (num<2) return(-1);
setblockid(ip,parm[1]);
break;

case 17:
rc=getblockid(ip);
break;

case 22:
delattr(ip);
break;

case 23:
genclass();
break;

case 24:
getnummet();
break;

case 25:
setbasetype(ip);
break;

case 26:
rc=getbasetype();
break;

case 27:
setsize(ip);
break;

default:
return(-1);

	}
return(rc);
}
