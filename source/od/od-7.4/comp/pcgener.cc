/*
pc++ code generator
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "oid.hpp"
#include "pcgener.hpp"
#include "block.hpp"
#include "sessmgr.hpp"
#include "syslog.hpp"
#include "defclass.hpp"	// for message codes
#include "string.hpp"	// for message codes

static	block 	sb;	

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

pcgener::pcgener(void)
{
init();
}

void pcgener::init(void)
{
name[0]=0;
level=0;
notype=0;
objid.init();
classid.init();
msgid=0;
msgid=0;
osize=0;
priv=0;
}

/*<>*/
oid pcgener::genblock(int sn)
{
int i,number;
oid bid;
char value[20];
number=sb.getnumber();
//bid=SM->create_object(sn,"block",number*sizeof(int));
bid=SM->create_object(sn,"block",0);
if (bid.item<0) return(bid);
//printf("PCG: block %s created - size %d\n",bid.aschar(),number);
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
void pcgener::genparm(int sn,char *parm,char type)
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
void pcgener::setname(char *nm)
{
char *np;
strncpy(compname,nm,sizeof(compname));
np=strchr(compname,'.');
if (np) *np=0;
}

/*<>*/
void pcgener::generate(int sn,int code,char *parm,int line,int ptype)
{
char *np;
int rc,on;
oid cid,bid;
oid pid[2];
//printf("generate code %d\n",code);
switch(code)
	{
case 1:		/* add a method to a class */
on=SM->lookup_symbol(sn,cname);
classid=SM->getid(sn,on);
if (classid.item>=0)
	{
	if (notype)
		strncpy(metname,type,sizeof(metname));
	else
		strncpy(metname,name,sizeof(metname));
	if (!strcmp(cname,metname)) return;
	if (!strcmp("init",metname)) return;
	rc=SM->sendmsg(sn,classid,CM_ADDMET,0,0);
	if (rc<0) break;
	SM->sessset(sn,classid,rc,CA_METNAME,metname);
//printf("pcgener addmet %s %s %s\n",cname,classid.aschar(),metname);
	}
else
       	{
	sprintf(msg,"on line %d class %s not found",line,cname);
	SL->log_error(msg);
        }
break;

case 2:		/* send a message to an object */
strncpy(name,type,sizeof(name));
np=strchr(name,'.');
if (np)
	{
	*np=0;
	np++;
	on=SM->lookup_symbol(sn,name);
	if (on<0)
        	{
sprintf(msg,"on line %d object %s not found",line,name);
		SL->log_error(msg);
                msgid = -1;
                return;
                }
	cid=SM->getclass(sn,on);
	msgid=SM->lookupmet(sn,cid,np);
	if (msgid<0)
		{
sprintf(msg,"on line %d method %s not found for object %s",line,np,name);
		SL->log_error(msg);
               	return;
		}
//printf("pcgener: sending message %d to %s\n",msgid,name);
	sb.genmsg(sn,on,msgid);
	}
else
	{
	if (classid.item<0)
        	{
sprintf(msg,"on line %d class not valid",line);
		SL->log_error(msg);
                msgid = -1;
                return;
                }
	msgid=SM->lookupmet(sn,classid,name);
	if (msgid<0)
		{
sprintf(msg,"on line %d method %s not found for object %s",line,name,cname);
		SL->log_error(msg);
       		return;
		}
 	sb.genmsg(sn,-1,msgid);
	}
break;

case 3:		/* generate a message parameter */
if (msgid<0 || parm[0]==')') break;
genparm(sn,parm,ptype);
break;

case 4:		/* save the parameter in name */
strncpy(name,parm,sizeof(name));
notype=0;
break;

case 5:		/* create a new class */
priv=0;
strncpy(cname,parm,sizeof(cname));
classid=SM->create_class(sn,cname);
if (classid.item<0)
        {
	sprintf(msg,"on line %d cannot create class %s",line,cname);
	SL->log_error(msg);
        msgid = -1;
        return;
        }
break;

case 6:		/* create a new object */
classid=SM->lookup_object(sn,type);
rc=SM->create_symbol(sn,cname,classid,osize);
if (rc<0)
        {
sprintf(msg,"on line %d cannot create object of class %s",line,type);
	SL->log_error(msg);
	msgid = -1;
	return;
        }
break;

case 7:		/* save the parameter in type */
strncpy(type,parm,sizeof(type));
notype=1;
break;

case 8:		/* "c" method name (main) */
strcpy(metname,cname);
classid=SM->create_class(sn,"cstart");
if (classid.item<0) 
	{
sprintf(msg,"on line %d cannot create class cstart",line);
	SL->log_error(msg);
	return;
	}
strncpy(cname,"cstart",sizeof(cname));
strncpy(metname,"main",sizeof(metname));
//printf("pcgener: adding method %s to class %s %s\n",metname,cname,
//	classid.aschar());
rc=SM->sendmsg(sn,classid,CM_ADDMET,0,0);
if (rc<0) break;
SM->sessset(sn,classid,rc,CA_METNAME,"main");
break;

case 9:				/* end of block */
sb.gencode(ENDBLOCK);
if (--level<0)
	{
//	sprintf(msg,"on line %d unmatched }",line);
//	SM->log_error(msg);
	level=0;
	}
if (level!=0) break;		/* generate only at level 0 */
//printf("PCG(9) adding method %s to class %s\n",metname,classid.aschar());
if (classid.item>=0) 
	{
	pid[1]=genblock(sn);
	msgid=SM->lookupmet(sn,classid,metname);
	pid[0]=SM->create_object(sn,"shint",msgid);
//printf("PCG(9) method number %d  block %s set\n",msgid,pid[1].aschar());
	rc=SM->sendmsg(sn,classid,CM_SETBLOCK,2,pid);
	if (rc<0)
		{
                sprintf(msg,"on line %d %s is not a member of %s",
			line,metname,cname);
		SL->log_error(msg);
		}
	}
break;

case 10:	/* set method name and classid */
strcpy(metname,name);
classid=SM->lookup_object(sn,cname);
break;

case 11:	/* start of block */
sb.gencode(STARTBLK);
level++;
break;

case 12:	/* set attrib length */
on=SM->lookup_symbol(sn,cname);
classid=SM->getid(sn,on);
if (classid.item>=0)
	SM->sessset(sn,classid,attrnum,CA_LENGTH,parm);
else
       	{
	sprintf(msg,"on line %d class %s not found",line,cname);
	SL->log_error(msg);
        }
break;

case 13:	/* add an attrib to a class */
if (priv) break;
on=SM->lookup_symbol(sn,cname);
classid=SM->getid(sn,on);
if (classid.item>=0)
	{
	attrnum=SM->sendmsg(sn,classid,CM_ADDATTR,0,0);
	if (attrnum<0)
		{
		sprintf(msg,"on line %d number of attributes exceeded for %s on %s",
			line,cname,name);
		SL->log_error(msg);
		}
	SM->sessset(sn,classid,attrnum,CA_ATTRNAME,name);
	SM->sessset(sn,classid,attrnum,CA_DATATYPE,type);
        }
else
       	{
	sprintf(msg,"on line %d class %s not found",line,cname);
	SL->log_error(msg);
        }
break;

case 14:	/* copy type to cname */
strncpy(cname,type,sizeof(cname));
break;

case 15:	/* set a parent class */
if (classid.item<0)
       	{
sprintf(msg,"on line %d parent class not valid",line);
	SL->log_error(msg);
        msgid = -1;
        return;
        }
SM->setparent(sn,classid,parm);
break;

case 16:	/* set a method parameter type */
break;

case 17:	/* save the parameter in cname */
osize=0;
strncpy(cname,parm,sizeof(cname));
break;

case 18:	/* assignment/test */
opfunc[0]=0;
if (parm[0]=='=') strncpy(opfunc,"equals",sizeof(opfunc));
if (parm[0]=='-') strncpy(opfunc,"minusequals",sizeof(opfunc));
if (parm[0]=='+') strncpy(opfunc,"plusequals",sizeof(opfunc));
if (parm[0]=='>') strncpy(opfunc,"isgreater",sizeof(opfunc));
if (parm[0]=='<') strncpy(opfunc,"isless",sizeof(opfunc));
if (parm[0]=='>' && parm[1]=='=') strncpy(opfunc,"isgreatereq",sizeof(opfunc));
if (parm[0]=='<' && parm[1]=='=') strncpy(opfunc,"islesseq",sizeof(opfunc));
break;

case 20:	/* -- / ++ */
if (parm[0]=='+') strncpy(opfunc,"plusequals",sizeof(opfunc));
if (parm[0]=='-') strncpy(opfunc,"minusequals",sizeof(opfunc));

case 19:	/* int assignment */
on=SM->lookup_symbol(sn,type);
if (on<0)
       	{
	sprintf(msg,"on line %d object %s not found",line,type);
	SL->log_error(msg);
        msgid = -1;
        return;
        }
classid=SM->getclass(sn,on);
msgid=SM->lookupmet(sn,classid,opfunc);
if (msgid<0)
	{
sprintf(msg,"on line %d method %s not found for object %s",line,opfunc,type);
	SL->log_error(msg);
       	return;
	}
sb.genmsg(sn,on,msgid);
if (code==20) 
	genparm(sn,"1",'I');
else
	genparm(sn,parm,ptype);
break;

case 22:	// set object size
osize=atoi(parm);
break;

case 23:	// set private
priv=1;
break;

case 36:
case 37:
case 33:
case 38:
case 39:
case 40:
printf("pcgener %d <%s> type %s opfunc %s\n",code,parm,type,opfunc);
case 41:
case 42:
sb.gencode(code-31);
break;

       }
return;
}
