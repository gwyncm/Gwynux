/*
smaltalk code generator
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "oid.hpp"
#include "block.hpp"
#include "sessmgr.hpp"
#include "stgener.hpp"
#include "syslog.hpp"
#include "string.hpp"	// for message codes
#include "defclass.hpp"	// for message codes

static	block	sb;		

static int xtoi(char *cp)
{
int i,l;
l=0;
for(i=0; cp[i]; i++)
	{
	l<<=4;
	if (isdigit(cp[i]))
		l|=(cp[i]-'0');
	else
		l|=((toupper(cp[i])-'A')+10);
	}
return(l);
}

stgener::stgener(void)
{
init();
}

void stgener::init(void)
{
objid.init();
classid.init();
msgid=0;
level=0;
blkname[0]=0;
reset();
}

/*<>*/
void stgener::genparm(int sn,char *parm,char type)
{
oid sid,num;
switch(type)
	{
case 'I':
sid.seg=atoi(parm);
sid.type=OT_SHINT;
break;

case 'H':
sid.seg=xtoi(&parm[2]);
sid.type=OT_SHINT;
break;

case 'S':
// don't think this occurs
sid.type=2;
break;

default:
sid=SM->create_object(sn,"string",1+strlen(parm));
num=SM->create_object(sn,"shint",1+strlen(parm));
SM->sendmsg(sn,sid,ST_SETMAX,1,&num);
SM->sessset(sn,sid,0,0,parm);
break;
	}   
sb.genparm(sid);
}

/*<>*/
oid stgener::genblock(int sn)
{
int i,number;
oid bid;
char value[20];
number=sb.getnumber();
bid=SM->create_object(sn,"block",number*sizeof(int));
if (bid.item<0) return(bid);
sprintf(value,"%d",number);
SM->sessset(sn,bid,0,4,value);
for(i=0; i<number; i++) 
	{
	strncpy(value,sb.get(i,3),sizeof(value));
	SM->sessset(sn,bid,i,5,value);
	}
sb.init();
return(bid);
}
/*<>*/
void stgener::reset(void)
{
objname[0]=0;
metname[0]=0;
target[0]=0;
parm1[0]=0;
parm2[0]=0;
type1=0;
type2=0;
}

/*<>*/
void stgener::generate(int sn,int code,char *parm,int line,int ptype)
{
char *np;
oid cid,pid[2];
int i,rc,on,msgid;
switch(code)
	{
case 1:		/* <Symbol> <Char> <Int> <Float> */
if (!objname[0]) { strncpy(objname,parm,sizeof(objname)); return; }
if (!metname[0]) { strncpy(metname,parm,sizeof(metname)); return; }
if (!parm1[0]) { strncpy(parm1,parm,sizeof(parm1)); type1=ptype; return; }
if (!parm2[0]) { strncpy(parm2,parm,sizeof(parm2)); type2=ptype; return; }
break;

case 2:		/* [ */
//printf("t=<%s> o=<%s> m=<%s> p1=<%s> p2=<%s>\n",target,objname,metname,parm1,parm2);
strncpy(blkname,parm1,sizeof(parm1));
classid=SM->lookup_object(sn,objname);
reset();
level++;
break;

case 3:		/* ] */
if (--level>0) return;  /* not end of method */
if (level<0)
	{
	sprintf(msg,"on line %d unmatched ]",line);
	SL->log_error(msg);
	level=0;
	}
if (classid.item>=0) 
	{
	pid[1]=genblock(sn);
	msgid=SM->lookupmet(sn,classid,blkname);
	pid[0]=SM->create_object(sn,"shint",msgid);
	rc=SM->sendmsg(sn,classid,CM_SETBLOCK,2,pid);
	if (rc<0)
		{
                sprintf(msg,"on line %d %s is not a member of the class %s",
			line,blkname,SM->getname(sn,classid));
		SL->log_error(msg);
		}
	}
break;

case 4:		/* <- */
strcpy(target,objname);
objname[0]=0;
break;

case 5:		/* . */
//printf("stg: t=<%s> o=<%s> m=<%s> p1=<%s> p2=<%s>\n",target,objname,metname,parm1,parm2);
if (!objname[0]) return;
if (!strncmp(parm1,"16r",3)) 
	{ 
	parm1[0]='0'; 
	parm1[1]='x'; 
	parm1[2]=parm1[3];
	parm1[3]=parm1[4];
	parm1[4]=0;
	type1='H';
	}
if (!strcmp(metname,"setParent"))
	{
	classid=SM->lookup_object(sn,objname);
	SM->setparent(sn,objid,parm1);
	reset();
	return;
	}
if (!strcmp(metname,"addMethod"))
	{
	if (!strcmp(objname,parm1)) return;
	on=SM->lookup_symbol(sn,objname);
	classid=SM->getid(sn,on);
	if (classid.item>=0)
		{
		rc=SM->sendmsg(sn,classid,CM_ADDMET,0,0);
		if (rc!=0) 
			SM->sessset(sn,classid,rc,CA_METNAME,parm1);
		}
	reset();
	return;
	}
if (!strcmp(metname,"addAttribute"))
	{
	reset();
	return;
	}
if (!strcmp(metname,"new"))
	{
	if (!strcmp("class",objname))
		objid=SM->create_class(sn,target);
	else
		{
		classid=SM->lookup_object(sn,objname);
		rc=SM->create_symbol(sn,target,classid,atoi(parm1));
		if (rc<0)
    	    		{
			sprintf(msg,"on line %d cannot create object of class %s",line,objname);
			SL->log_error(msg);
			msgid = -1;
			reset();
			return;
			}
		}
	reset();
	return;
	}
if (!strcmp(objname,"self"))
	{
	cid=classid;				
	if (cid.item<0)
        	{
sprintf(msg,"on line %d class not valid",line);
		SL->log_error(msg);
		reset();
                return;
                }
	msgid=SM->lookupmet(sn,cid,metname);
	if (msgid<0)
		{
sprintf(msg,"on line %d method %s not found for object self",line,metname);
		SL->log_error(msg);
		reset();
		return;
		}
 	sb.genmsg(sn,-1,msgid);
	if (parm1[0]) genparm(sn,parm1,type1);
	if (parm2[0]) genparm(sn,parm2,type2);
	reset();
	return;
	}
on=SM->lookup_symbol(sn,objname);
if (on<0)
       	{
	sprintf(msg,"on line %d object %s not found",line,objname);
	SL->log_error(msg);
        msgid = -1;
	reset();
        return;
        }
cid=SM->getclass(sn,on);
msgid=SM->lookupmet(sn,cid,metname);
if (msgid<0)
	{
sprintf(msg,"on line %d method %s not found for object %s",line,metname,objname);
	SL->log_error(msg);
	reset();
       	return;
	}
sb.genmsg(sn,on,msgid);
if (parm1[0]) genparm(sn,parm1,type1);
if (parm2[0]) genparm(sn,parm2,type2);
reset();
break;

case 6:		/* : */
break;

       }        
return;
}
