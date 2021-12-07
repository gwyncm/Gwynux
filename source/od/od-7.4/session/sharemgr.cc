#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "sharemgr.hpp"

#define MAX_OBJ	30

void sharemgr::init(void)
{
// do not reinitialize
}

sharemgr::sharemgr(void)
{
int i;
for(i=0; i<MAX_OBJ; i++) 
	{
	tasknum[i] = imgnum[i] = -1;
	objname[i][0]=0;
	}
number=0;
}

char *sharemgr::get(int line,int fld)
{
return("");
}

void sharemgr::set(int line,int fld,char *value)
{
}

int sharemgr::gettask(int on)
{
if (on<0 || on>=MAX_OBJ || imgnum[on]<0) return(-1);
return(tasknum[on]);
}

oid sharemgr::getobj(int on)
{
return(objlist[on]);
}

/*<>*/
int sharemgr::addobj(char *oname,oid id,int img,int tn)
{
int i;
number = -1;
for(i=0; i<MAX_OBJ; i++)
	if (imgnum[i]<0) { number=i; break; }	
if (number<0) return(number);
if (oname[0])
	{
	objlist[number]=id;
	imgnum[number]=img;
	tasknum[number]=tn;
	strncpy(objname[number],oname,sizeof(objname[number]));
	return(number);
	}
return(0);
}

/*<>*/
int sharemgr::lookup(char *oname)
{
int i;
for(i=0; i<MAX_OBJ; i++)
	if (imgnum[i]>=0 && !strcmp(oname,objname[i]))
		return(i);
return(-1);
}

/*<>*/
char *sharemgr::getname(int in)
{
if (in<0 || in>=MAX_OBJ || imgnum[in]<0) return("");
return(objname[in]);
}

/*<>*/
void sharemgr::clearimage(int in)
{
int i;
for(i=0; i<MAX_OBJ; i++)
	{
        if (imgnum[i]==in) 
		imgnum[i] = -1;
	}
}

/*<>*/
long sharemgr::message(int msg,int num,oid parm[])
/* class method entry point */
{
long rc;
int i,len;
int ip=0;
char *cp = "";
if (num) ip = parm[0].getseg();
rc=0;
switch(msg)
	{
case 0: 	
init();
break;

case 2:
clearimage(ip);
break;

	}
return(rc);
}
