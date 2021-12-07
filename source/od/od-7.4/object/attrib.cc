#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "oid.hpp"
#include "attrib.hpp"

#define NUMDTYPE	5
const char *dtypes[NUMDTYPE] =
	{
	"char",
	"short",
	"int",
	"string",
	"parent"
	};

/*<>*/
attrib::attrib(void)
/* constructor to call init */
{
init();
}

/*<>*/
void attrib::init(void)
/* initialize the object */
{
name[0]=0;
reference.init();
datatype=0;
length=1;
offset=0;
precision=0;
fieldtype='u';
access='w';
null='y';
key=0;
}

/*<>*/
int attrib::size(void)
/* return object size */
{
return(sizeof(attrib));
}

void attrib::set(int line,int fld,char *value)
/* sets an attribute value */
{
}

char *attrib::get(int line,int fld)
/* returns an attribute value */
{
return("");
}

/*<>*/
long attrib::message(int msg,int num,oid parm[])
/* process a attrib message */
{
switch(msg)
	{
case 0:
init();
break;

default:
return(-1);

	}
return(0);
}

/*<>*/
void attrib::setdatatype(char *value)
/* set attribute data type */
{
int i;
datatype = 0; //default to char
for (i=0; i<NUMDTYPE; i++)
	if (!strncmp(dtypes[i],value,sizeof(value))) datatype=i;
//printf("attrib settype set to <%s> %d from <%s>\n",dtypes[datatype],datatype,value);
}

/*<>*/
char *attrib::getdatatype(void)
/* get attribute data type */
{
static char rdtype[10];
strcpy(rdtype,dtypes[datatype]);
//printf("attrib gettype <%s> %d for <%s>\n",dtypes[datatype],datatype,name);
return(rdtype);
}

/*<>*/
void attrib::setlength(int num)
{
length=num;
}

/*<>*/
int attrib::getlength(void)
{
return(length);
}

/*<>*/
void attrib::setoffset(int off)
{
offset=off;
}

/*<>*/
int attrib::getoffset(void)
{
return(offset);
}

/*<>*/
char *attrib::getname(void)
{
return(name);
}

/*<>*/
void attrib::setname(char *nm)
{
strncpy(name,nm,sizeof(name));
}

/*<>*/
oid attrib::getref(void)
{
return(reference);
}

/*<>*/
void attrib::setref(oid aid)
{
reference=aid;
}

/*<>*/
void attrib::setprecision(int num)
{
precision=num;
}

/*<>*/
int attrib::getprecision(void)
{
return(precision);
}

/*<>*/
void attrib::setfieldtype(char *atype)
{
if (0==strpbrk(atype,"ULMDPNIulmdpni")) return;
fieldtype=tolower(atype[0]);
}

/*<>*/
char attrib::getfieldtype(void)
{
return(fieldtype);
}

/*<>*/
void attrib::setnull(char *atype)
{
if (0==strpbrk(atype,"YNyn")) return;
null=tolower(atype[0]);
}

/*<>*/
char attrib::getnull(void)
{
return(null);
}

/*<>*/
void attrib::setaccess(char *atype)
{
if (0==strpbrk(atype,"RWVAMQUrwvamqu")) return;
access=tolower(atype[0]);
}

/*<>*/
char attrib::getaccess(void)
{
return(access);
}

/*<>*/
void attrib::setkey(int num)
{
key=num;
}

/*<>*/
int attrib::getkey(void)
{
return(key);
}
