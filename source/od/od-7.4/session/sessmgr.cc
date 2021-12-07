/********************************/
/* System Session Manager Class */
/********************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include "oid.hpp"
#include "sharemgr.hpp"
#include "sessmgr.hpp"
#include "defclass.hpp"		// symbols

#define MAX_SESSIONS 30

static sharemgr smgr;

sessmgr* sessmgr::_instance = 0;

/*<>*/
sessmgr* sessmgr::getinstance()
{
if (_instance == 0) _instance = new sessmgr;
return (_instance);
}

/*<>*/
sessmgr::sessmgr(void)
{
int i;
number=0;
for(i=0; i<MAX_SESSIONS; i++) sesslist[i].init();
}

/*<>*/
void sessmgr::init(void)
{
}

/*<>*/
int sessmgr::size(void)
{
return(sizeof(sessmgr));
}

/*<>*/
oid sessmgr::getparent(int sn,oid on)
{
oid empty;
if (sn<0 || sn >=number) return(empty);
return(sesslist[sn].getparent(on));
}

/*<>*/
int sessmgr::close_image(int sn)
{
if (sn<0 || sn >= MAX_SESSIONS) return(0);
//printf("SM: closing session %d\n",sn);
sesslist[sn].setstatus(0);
number--;
return(sesslist[sn].close_image());
}


/*<>*/
void sessmgr::setstatus(int sn,int st)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].setstatus(st);
}

// ************************ object functions *****************

/*<>*/
void sessmgr::showmap(int sn)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].showmap();
}

/*<>*/
int sessmgr::lookupmet(int sn,oid cid,char *mname)
{
/* lookup a method for a class */
int i;
char *mp,*ip;
if (!strcmp(mname,"init")) return(0);
mp=sessget(sn,cid,0,CA_METNAME);
for(i=0; mp[0]; i++)
	{
	if (!strcmp(mp,mname)) return(i); 
	mp=sessget(sn,cid,i+1,CA_METNAME);
	}
return(-1);
}

/*<>*/
int sessmgr::lookup_symbol(int sn,char *oname)
{
if (sn<0 || sn >= MAX_SESSIONS) return(-1);
return(sesslist[sn].lookup_symbol(oname));
}

/*<>*/
oid sessmgr::lookup_object(int sn,char *oname)
{
int on;
oid empty;
if (sn<0 || sn >= MAX_SESSIONS) return(empty);
on=sesslist[sn].lookup_symbol(oname);
return(getid(sn,on));
}

/*<>*/
void sessmgr::delete_object(int sn,oid id)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].delete_object(id);
}

/*<>*/
void sessmgr::delete_symbol(int sn,int on)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].delete_symbol(on);
}

/*<>*/
int sessmgr::create_symbol(int sn,char *sname,oid cid,int size)
{
if (sn<0 || sn >= MAX_SESSIONS) return(-1);
return(sesslist[sn].create_symbol(sname,cid,size));
}

/*<>*/
char *sessmgr::sessget(int sn,oid id,int line,int fld)
{
int ssn;
if (id.type==OT_SHARE)
	{
	ssn=smgr.gettask(id.item);
	if (ssn<0) return("");
	id=smgr.getobj(id.item);
	return(sessget(ssn,id,line,fld));
	}
if (sn<0 || sn >= MAX_SESSIONS) return("");
//printf("SM: sessget id %s line %d fld %d\n",id.aschar(),line,fld);
return(sesslist[sn].sessget(id,line,fld));
}

/*<>*/
void sessmgr::sessset(int sn,oid id,int line,int fld,char *value)
{
int ssn;
if (id.type==OT_SHARE)
	{
	ssn=smgr.gettask(id.item);
	if (ssn<0) return;
	id=smgr.getobj(id.item);
	}
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].sessset(id,line,fld,value);
}

/*<>*/
long sessmgr::sendmsg(int sn,oid id,int msg,int num,oid parm[])
{
int ssn;
if (id.type==OT_SHARE)
	{
	ssn=smgr.gettask(id.item);
	if (ssn<0) return(0);
	id=smgr.getobj(id.item);
	}
if (sn<0 || sn >= MAX_SESSIONS) return(-1);
return(sesslist[sn].sendmsg(id,msg,num,parm));
}
	
/*<>*/	
char *sessmgr::get(long line,int fld)
/* returns a task attribute value */
{
return("");
}

/*<>*/	
void sessmgr::set(long line,int fld,char *value)
/* sets a task attribute value */
{
}

/*<>*/
oid sessmgr::getclass(int sn,oid id)
{
oid empty;
if (sn<0 || sn >= MAX_SESSIONS) return(empty);
return(sesslist[sn].getclass(id));
}

/*<>*/
oid sessmgr::getclass(int sn,int on)
{
oid empty;
if (sn<0 || sn >= MAX_SESSIONS) return(empty);
return(sesslist[sn].getclass(on));
}

/*<>*/
void sessmgr::setid(int sn,int on,oid id)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].setid(on,id);
}

// ********************* object references **************************

/*<>*/
int sessmgr::start(int sn)
{
oid id;
int newnum,oldnum,i;
oldnum = sn;
newnum = -1;
for(i=0; i<MAX_SESSIONS; i++) 
	{
	if (!sesslist[i].getstatus())
		{
		newnum=i;
		break;
		}
	}			
//printf("SM: starting session %d\n",newnum);
if (newnum<0) return(-1);
// copy context if subtask
if (oldnum>=0) sesslist[newnum]=sesslist[oldnum];
sesslist[newnum].setstatus(1);
number++;
return(newnum);
}

/*<>*/
int sessmgr::create_image(char *fname)
{
oid obj,cid;
int on,rc,newsess;
char *il,lname[100];
newsess=start(-1);
if (newsess<0) return(newsess);
sprintf(lname,"%s",fname);
rc=sesslist[newsess].create_image(lname);
if (rc<0) 
	{
	sesslist[newsess].setstatus(0);
	number--;
	return(-1);
	}
return(newsess);
}

/*<>*/
int sessmgr::open_image(char *fname)
{
oid obj,cid;
int on,rc,newsess;
char *il,lname[100];
//printf("SM: opening image %s\n",fname);
newsess=start(-1);
if (newsess<0) return(newsess);
sprintf(lname,"%s",fname);
rc=sesslist[newsess].open_image(lname);
if (rc<0)
	{
	il=getenv("OD_IMGLIB");
	if (il)
		{
		sprintf(lname,"%s/%s",il,fname);
		rc=sesslist[newsess].open_image(lname);
		}
	}
if (rc<0) 
	{
	sesslist[newsess].setstatus(0);
	number--;
	return(-1);
	}
else
	sesslist[newsess].setstatus(2);		// run only once
return(newsess);
}

/*<>*/
oid sessmgr::create_class(int sn,char *cname)
/* create a class and return its id */
{
int on;
oid id,cid;	
//printf("SM: create_class class %s\n",cname);
on=lookup_symbol(sn,cname);
if (on>=0) { printf("SM: create_class duplicate class %s created\n",cname); }
cid.item=0;		// class class
cid.type=OT_BASEOBJ;
id=create_object(sn,"defclass",0);
sessset(sn,id,0,CA_CLASSNAME,cname);
on=create_symbol(sn,cname,cid,0); 
setid(sn,on,id); 
return(id);
}

/*<>*/
oid sessmgr::create_object(int sn,char *cname,int asize)
{
oid id;
id=sesslist[sn].create_object(cname,asize);
return(id);
}

/*<>*/
oid sessmgr::create_object(int sn,oid cid,int asize)
{
oid id;
if (sn<0 || sn >= MAX_SESSIONS) return(id);
id=sesslist[sn].create_object(cid,asize);
return(id);
}

/*<>*/
oid sessmgr::getid(int sn,int on)
{
oid id;
if (sn<0 || sn >= MAX_SESSIONS) return(id);
id=sesslist[sn].getid(on);
return(id);
}

/*<>*/
void sessmgr::setname(int sn,oid id,char *name)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].setname(id,name);
}

/*<>*/
char *sessmgr::getname(int sn,oid id)
{
if (sn<0 || sn >= MAX_SESSIONS) return(0);
if (id.type==OT_SHARE)
	return(smgr.getname(id.item));
if (id.type==OT_SYMBOL) 
	return(sesslist[sn].getname(id.item));
if (id.type==OT_CLASS) id.type=0;
if (id.type==OT_BASEOBJ) id.type=0;
return(sesslist[sn].getname(id));
}

/*<>*/
char *sessmgr::getname(int sn,int id)
{
if (sn<0 || sn >= MAX_SESSIONS) return(0);
return(sesslist[sn].getname(id));
}

/*<>*/
void *sessmgr::getaddress(int sn,oid id)
{
//printf("SM: getaddress %s session %d\n",id.aschar(),sn);
if (sn<0 || sn >= MAX_SESSIONS) return(0);
return(sesslist[sn].getaddress(id));
}

/*<>*/		//  used by OM - retain for now
void sessmgr::setparent(int sn,oid cid,oid pid)
{
if (sn<0 || sn >= MAX_SESSIONS) return;
sesslist[sn].setparent(cid,pid);
}

/*<>*/
void sessmgr::setparent(int sn,oid cid,char *pname)
/* set the parent of any class */
/* copy attributes and methods */
{
oid pid;
char *np;
int i,rc;
sessset(sn,cid,0,CA_PARENT,pname);
pid=lookup_object(sn,pname);
if (pid.item>=0)
	{
	np=sessget(sn,pid,1,CA_METNAME);
	for(i=1; np[0]; i++)
		{
		rc=sendmsg(sn,cid,CM_ADDMET,0,0);
		if (rc<0) return;
		sessset(sn,cid,rc,CA_METNAME,np);
		np=sessget(sn,pid,i+1,CA_METNAME);
		}
	np=sessget(sn,pid,0,CA_ATTRNAME);
	for(i=0; np[0]; i++)
		{
		rc=sendmsg(sn,cid,CM_ADDATTR,0,0);
		if (rc<0) return;
		sessset(sn,cid,rc,CA_ATTRNAME,np);
		sessset(sn,cid,rc,CA_DATATYPE,"parent");
		np=sessget(sn,pid,i+1,CA_ATTRNAME);
		}
	}
}

/*<>*/
void sessmgr::share(int sn,char *oname)
{
oid id;
int img;
id=lookup_object(-1,oname);
if (id.item<0) return; 
//img=getimage(-1);
//smgr.addobj(oname,id,img,sn);
// fix me
}

/*<>*/
long sessmgr::message(int sn,int msg,int num,oid parm[])
/* class method entry point */
{
long rc;
int i,len;
int ip=0;
char *cp = "";
if (num)
	{
        ip = parm[0].getseg();
	if (parm[0].gettype()==OT_STRING) cp = sessget(sn,parm[0],0,0);
        }
rc=0;
switch(msg)
	{
case 0: 	
//init();
break;

default:
rc = -1;
break;
	}
return(rc);
}
