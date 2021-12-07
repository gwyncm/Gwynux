#include <stdio.h>
#include <string.h>

union attr { float fattr; int iattr; };

#include "oid.hpp"
#include "basemgr.hpp"
#include "defclass.hpp"
#include "taskmgr.hpp"
#include "htmldoc.hpp"

#define MAX_TYPES 3
static char *ctypes[MAX_TYPES] =
	{
	"defclass",
	"taskmgr",
	"htmldoc"
	};

/*<>*/
basemgr::basemgr(void)
{
init();
}

/*<>*/
char *basemgr::get(int type,long line,int fld)
{
return("");
}

/*<>*/
void basemgr::set(int type,long line,int fld,char *value)
{
}

/*<>*/
int basemgr::gettype(char *atype)
{
/* gets the type of a base class */
int i;
for(i=0; i<MAX_TYPES; i++)
        if (!strcmp(atype,ctypes[i])) return(i);
return(-1);
}

/*<>*/
char *basemgr::getname(int cn)
{
/* gets the name of a base class */
if (cn>=MAX_TYPES) return("");
return(ctypes[cn]);
}

/*<>*/
int basemgr::getsize(int type)
/* gets the size of a base class */
{
int size;
size = 0;
switch(type)
        {
case 0:
size = ((defclass *) 0)->size();
break;

case 1:
size = 0;	/*  taskmgr */
break;

case 2:
size = ((htmldoc *) 0)->size();
break;

        }
return(size);
}

/*<>*/
void basemgr::init(void)
/* initialize static classes */
{
}

/*<>*/
long basemgr::sendmsg(int sn,int type,char *obj,int msg,int num,oid parm[])
/* send a message to an object of a class */
{
long rc;
rc=0;
switch(type)
        {
case 0:
rc = ((defclass *) obj)->message(sn,msg,num,parm);
break;

case 1:
rc = TM->message(sn,msg,num,parm);
break;

case 2:
rc = ((htmldoc *) obj)->message(sn,msg,num,parm);
break;

        }
return(rc);
}

/*<>*/
char *basemgr::getobj(int type,char *obj,long lne,int fld)
/* send a get to an object of a class */
{
char *rc;
rc="";
switch(type)
        {
case 0:
rc = ((defclass *) obj)->get(lne,fld);
break;

case 1:
rc = TM->get(lne,fld);
break;

case 2:
rc = ((htmldoc *) obj)->get(lne,fld);
break;

        }
return(rc);
}

/*<>*/
void basemgr::setobj(int type,char *obj,long lne,int fld,char *value)
/* send a set to an object of a class */
{
switch(type)
        {
case 0:
((defclass *) obj)->set(lne,fld,value);
break;

case 1:
TM->set(lne,fld,value);
break;

case 2:
((htmldoc *) obj)->set(lne,fld,value);
break;

        }
}

