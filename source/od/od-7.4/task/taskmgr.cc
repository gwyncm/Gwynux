/*****************************
/* System Task Manager Class */
/*****************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include "oid.hpp"
#include "sharemgr.hpp"
#include "sessmgr.hpp"
#include "taskmgr.hpp"
#include "defclass.hpp"		// symbols
#include "block.hpp"		// symbols
#include "basemgr.hpp"

#define MAX_TASKS 30

FILE	*savefile;	// used by generate

basemgr 	bc;
static sharemgr smgr;

taskmgr* taskmgr::_instance = 0;

/*<>*/
taskmgr* taskmgr::getinstance()
{
if (_instance == 0) _instance = new taskmgr;
return (_instance);
}

/*<>*/
taskmgr::taskmgr(void)
{
int i;
active=0;
number=0;
waittime=5;
curtask=1;
for(i=0; i<MAX_TASKS; i++) tasklist[i].init();
}

/*<>*/
void taskmgr::init(void)
{
}

/*<>*/
int taskmgr::size(void)
{
return(sizeof(taskmgr));
}

/*<>*/
int taskmgr::taskmsg(void)
/* process a message for an task */
{
int rc,i;
oid id;
while(number>0) 
	{
	if (++active>=MAX_TASKS) 
		{ 
		active=0; 
// this could actually take as long as a minute if there is no i/o pending
		usleep(1);
		}
	rc=tasklist[active].taskmsg();
	if (rc)
		{
		id=tasklist[active].getobject();
		sendmsg(active,id,1,0,0);
//printf("TM: taskmsg task %d rc=%d number=%d\n",active,rc,number);
		}
	if (rc==2) { tasklist[active].setstatus(0); number--; }
	}
return(-1);
}

/*<>*/
oid taskmgr::getobject(int tn)
{
return(tasklist[tn].getobject());
}

/*<>*/
void taskmgr::stop(void)
{
number=0;
active=0;
}

/*<>*/
void taskmgr::setpriority(int tn,int py)
{
if (tn<0) tn=active;
tasklist[tn].setpriority(py);
}

/*<>*/
oid taskmgr::getparent(int tn,oid on)
{
if (tn<0) tn=active;
return(tasklist[tn].getparent(on));
}

/*<>*/
int taskmgr::close_image(void)
{
return(tasklist[active].close_image());
}


/*<>*/
void taskmgr::setstatus(int tn,int st)
{
if (tn<0) tn=active;
tasklist[tn].setstatus(st);
}

/*<>*/
void taskmgr::setobject(int tn,oid id)
{
if (tn<0) tn=active;
tasklist[tn].setobject(id);
}

// ************************ object functions *****************

/*<>*/
void taskmgr::showmap(void)
{
tasklist[active].showmap();
}

/*<>*/
int taskmgr::getactive(void)
{
return(active);
}

/*<>*/
void taskmgr::setactive(int tn)
{
active=tn;
}

/*<>*/
int taskmgr::getsession(int tn)
{
if (tn<0) tn=active;
if (tn>=MAX_TASKS) return(-2);
if (tasklist[tn].getstatus()<0) return(-1);
return(tasklist[tn].getsession());
}

/*<>*/
int taskmgr::lookupmet(int tn,oid cid,char *mname)
{
/* lookup a method for a class */
int i;
char *mp,*ip;
if (!strcmp(mname,"init")) return(0);
mp=taskget(tn,cid,0,CA_METNAME);
for(i=0; mp[0]; i++)
	{
	if (!strcmp(mp,mname)) return(i); 
	mp=taskget(tn,cid,i+1,CA_METNAME);
	}
return(-1);
}

/*<>*/
int taskmgr::lookup_symbol(int tn,char *oname)
{
if (tn<0) tn=active;
return(tasklist[tn].lookup_symbol(oname));
}

/*<>*/
oid taskmgr::lookup_object(int tn,char *oname)
{
int on;
if (tn<0) tn=active;
on=tasklist[tn].lookup_symbol(oname);
return(getid(tn,on));
}

/*<>*/
void taskmgr::delete_object(int tn,oid id)
{
if (tn<0) tn=active;
tasklist[tn].delete_object(id);
}

/*<>*/
void taskmgr::delete_symbol(int tn,int on)
{
if (tn<0) tn=active;
tasklist[tn].delete_symbol(on);
}

/*<>*/
int taskmgr::create_symbol(int tn,char *sname,oid cid,int size)
{
if (tn<0) tn=active;
return(tasklist[tn].create_symbol(sname,cid,size));
}

/*<>*/
void taskmgr::taskset(int tn,oid id,int line,int fld,char *value)
{
int btype,stn;
char *ip,*atype;
oid cid,aid,*alist;
if (id.type==OT_SYMBOL) id=getid(tn,id.seg);
if (id.type==OT_SHARE) 
	{
	stn=smgr.gettask(id.item);
	if (stn<0) return;
	id=smgr.getobj(id.item);
	}
if (id.type==OT_STRING || id.type==OT_CLASS || id.type==OT_BLOCK) 
	{ tasklist[tn].taskset(id,line,fld,value); return; }
if (id.item<0) return;
cid=getclass(tn,id);
btype=atoi(taskget(tn,cid,0,CA_BASETYPE));
while (btype<1)				// search for base class
        {
	id=getparent(tn,id);
	if (id.item<0) return; 
	cid=getclass(tn,id);
	btype=atoi(taskget(tn,cid,0,CA_BASETYPE));
	}
ip = (char *) getaddress(tn,id);
bc.setobj(btype,ip,line,fld,value);
}

/*<>*/
char *taskmgr::taskget(int tn,oid id,int line,int fld)
{
int btype,stn;
static char retint[8];
oid cid,aid,*alist;
char *ip,*rp,*atype;
//printf("TM: taskget id %s line %d fld %d\n",id.aschar(),line,fld);
if (id.type==OT_SHINT) { sprintf(retint,"%d",id.seg); return(retint); } 
if (id.type==OT_SYMBOL) id=getid(tn,id.seg);
if (id.type==OT_SHARE) 
	{
	stn=smgr.gettask(id.item);
	if (stn<0) return("");
	id=smgr.getobj(id.item);
	return(taskget(stn,id,line,fld));
	}
if (id.type==OT_STRING || id.type==OT_CLASS || id.type==OT_BLOCK) 
	{ rp=tasklist[tn].taskget(id,line,fld); return(rp); }
// added for parameters only
if (tn<0) tn=getactive();
//return(taskget(tn,id,line,fld));
if (id.item<0) return("");
cid=getclass(tn,id);
btype=atoi(taskget(tn,cid,0,CA_BASETYPE));
while (btype<1)				// search for base class
	{
	id=getparent(tn,id);
	if (id.item<0) return(""); 
	cid=getclass(tn,id);
	btype=atoi(taskget(tn,cid,0,CA_BASETYPE));
	}
ip = (char *) getaddress(tn,id);
rp=bc.getobj(btype,ip,line,fld);
return(rp);
}

/*<>*/
long taskmgr::sendmsg(int tn,oid id,int msg,int num,oid parm[])
{
long rc;
int btype,stn,sn;
char *ip,*pp;
oid cid,bid,pnum;
sn=tasklist[tn].getsession();
//printf("TM: sendmsg id=%s msg=%d tn=%d\n",id.aschar(),msg,tn);
if (tn<0) tn=getactive();
if (id.type==OT_SYMBOL) id=getid(tn,id.seg);
if (id.type==OT_SHARE) 
	{
	stn=smgr.gettask(id.item);
	if (stn<0) return(0);
	id=smgr.getobj(id.item);
	}
if (id.type==OT_STRING || id.type==OT_CLASS || id.type==OT_BLOCK) 
	{ rc=tasklist[tn].sendmsg(id,msg,num,parm); return(rc); }
if (id.item<0) 
	{
	printf("TM: sendmsg invalid object id=%s msg=%d\n",id.aschar(),msg);
	return(0);
	}
bid.item = -1;
while(bid.item<0)	// search for implementing object
	{
// get the class
	cid=getclass(tn,id);
	btype=atoi(taskget(tn,cid,0,CA_BASETYPE));
//printf("TM: sendmsg class cid=%s btype=%d\n",cid.aschar(),btype);
	if (btype>0)
		{
// base class - send message 
		ip = (char *)getaddress(tn,id);
		rc=bc.sendmsg(sn,btype,ip,msg,num,parm);
//printf("TM: sendmsg baseclass %s btype %d msg %d rc=%d %p\n",cid.aschar(),btype,msg,rc,ip);
		return(rc);
		}
	else	
		{	
		pnum.seg=msg;
		pnum.type=OT_SHINT; 
		ip = (char *)getaddress(tn,cid);
		rc=bc.sendmsg(sn,0,ip,CM_GETBLOCK,1,&pnum);
		bid.setint(rc);
//printf("TM: sendmsg block %s\n",bid.aschar());
		if (bid.item<0)
			{
// no block - get parent
			id=getparent(tn,id);
			if (id.item<0) bid.item=0;
			}
		else
			{
// exec block method
//printf("TM: execing block %s\n",bid.aschar());
			rc=exec_block(tn,bid,1,&id);
			return(rc);
			}
		}
	}
return(0);
}
	
/*<>*/	
char *taskmgr::get(long line,int fld)
/* returns a task attribute value */
{
oid id;
static char retch[50];
if (!tasklist[line].getstatus()) return("");	
switch(fld)
	{
case 1:		// task number
curtask=line;
sprintf(retch,"%d",line);
return(retch);

case 2:		// task image name
//return(tasklist[line].getimgname());
return("fix me");

case 3:		// task image edit status
return("TEST");

case 4:		// task object id
id=tasklist[line].getobject();
sprintf(retch,"%s",id.aschar());
return(retch);

case 5:		// task priority
sprintf(retch,"%d",tasklist[line].getpriority());
return(retch);

case 6:		// task object name
return(getname(line,tasklist[line].getobject()));
	}
return("");
}

/*<>*/	
void taskmgr::set(long line,int fld,char *value)
/* sets a task attribute value */
{
if (tasklist[line].getstatus()==0) return;
switch(fld)
	{
case 2:		// image name
tasklist[line].setimgname(value);
break;

case 5:		// set priority
setpriority(line,atoi(value));
break;
	}
}

/*<>*/
oid taskmgr::getclass(int tn,oid id)
{
if (tn<0) tn=active;
return(tasklist[tn].getclass(id));
}

/*<>*/
oid taskmgr::getclass(int tn,int on)
{
if (tn<0) tn=active;
return(tasklist[tn].getclass(on));
}

/*<>*/
void taskmgr::setid(int tn,int on,oid id)
{
if (tn<0) tn=active;
tasklist[tn].setid(on,id);
}

// ********************* object references **************************

/*<>*/
int taskmgr::start(int tn,char *oname)
{
oid id;
char noname[20];
int newnum,oldnum,on,i;
newnum = -1;
if (tn<0) oldnum=active; else oldnum=tn;
if (oname[0]) id=lookup_object(tn,oname);
for(i=0; i<MAX_TASKS; i++) 
	{
	if (!tasklist[i].getstatus())
		{
		newnum=i;
		break;
		}
	}			
//printf("TM: starting %s id %s tn %d\n",oname,id.aschar(),newnum);
if (newnum<0) return(-1);
tasklist[newnum]=tasklist[oldnum];
tasklist[newnum].setstatus(1);
tasklist[newnum].setobject(id);
number++;
return(newnum);
}

/*<>*/
int taskmgr::create_image(char *fname)
{
oid obj,cid;
int on,rc,newtask;
char *il,lname[100];
newtask=start(-1,"");
if (newtask<0) return(newtask);
sprintf(lname,"%s",fname);
rc=tasklist[newtask].create_image(lname);
if (rc<0) 
	{
	tasklist[newtask].setstatus(0);
	number--;
	return(-1);
	}
return(newtask);
}

/*<>*/
int taskmgr::open_image(char *fname)
{
char btype[10];
oid id,obj,cid;
char *cname,*oname;
char *il,lname[100];
int on,rc,newtask,i,at;

newtask=start(-1,"");
if (newtask<0) return(newtask);
sprintf(lname,"%s",fname);
rc=tasklist[newtask].open_image(lname);
if (rc<0)
	{
	il=getenv("OD_IMGLIB");
	if (il)
		{
		sprintf(lname,"%s/%s",il,fname);
		rc=tasklist[newtask].open_image(lname);
		}
	}
if (rc<0) 
	{
	tasklist[newtask].setstatus(0);
	number--;
	return(-1);
	}
else
	tasklist[newtask].setstatus(2);		// run only once

// tag all the base classes 
// with their base class type and size

cname=bc.getname(0);
for(i=0; cname[0]; i++)
	{
	on=lookup_symbol(newtask,cname);
	if (on >= 0)
		{
		cid=getid(newtask,on);
		sprintf(btype,"%d",i);
		taskset(newtask,cid,0,CA_BASETYPE,btype);
		id.seg=bc.getsize(i);
		id.type=OT_SHINT;
		sendmsg(newtask,cid,CM_SETSIZE,1,&id);

//printf("TM: open_image: tagging type %2d cid %s size %4d %s\n",
//	i,cid.aschar(),id.seg,cname);

		}
	cname=bc.getname(i+1);
	}

//printf("TM: open_image: tagging completed\n");

// generate all static objects

at=getactive();
setactive(newtask);
oname=getname(newtask,i=0);
while(oname[0])
	{
	id=getid(newtask,i);
//printf("TM: generate %s %s\n",oname,id.aschar());
	if (id.item<0) 
		{
		cid=getclass(newtask,i);
		cname=getname(newtask,cid);
		if (cname && cname[0])
			{
			id=create_object(newtask,cname,0);
			setid(newtask,i,id);
//printf("TM: generated static %s class %s %s\n",oname,cname,id.aschar());
			}
		}
	oname=getname(newtask,++i);
	}
setactive(at);

// start main

if (newtask>=0) exec(newtask,"cstart");

//printf("TM: image open task %d\n",newtask);

return(newtask);
}

/*<>*/
void taskmgr::exec(int tn,char *oname)
{
int on;
oid cid,id;
char noname[20];

on=lookup_symbol(tn,oname);
cid=getclass(tn,on);

//printf("TM: exec executing %s %s on=%d\n",oname,cid.aschar(),on);

// this is only used to construct ocstart and odispx

if (cid.type==OT_BASEOBJ && cid.item==0) // class class	
	{
	noname[0]='o'; noname[1]=0;
        strncpy(&noname[1],oname,sizeof(noname)-1);
	id=getid(tn,on);
//printf("TM: exec getid object %s\n",id.aschar());
	on=create_symbol(tn,noname,id,0); 
	id=create_object(tn,oname,0);	// create here to run constructor
//printf("TM: exec created object %d %s tn %d\n",on,id.aschar(),tn);
	setid(tn,on,id);
	setobject(tn,id);
	}
else
	{
	id=getid(tn,on);
	setobject(tn,id);
	}
}

/*<>*/
void taskmgr::exit(void)
{
int i,j,in;
oid id,cid,pid;
smgr.clearimage(active);
for(i=0; i<MAX_TASKS; i++)
	{
	if (tasklist[i].getstatus())
		{
		if (tasklist[i].getsession()==tasklist[active].getsession())
			{ 
//			tasklist[i].close_image();	
			tasklist[i].setstatus(0); 
			if (curtask==i) for(j=0; j<MAX_TASKS; j++) 
				if (tasklist[j].getstatus()) curtask==j;
			number--; 
			}
		}
	}
}

/*<>*/
oid taskmgr::create_class(int tn,char *cname)
/* create a class and return its id */
{
int on;
oid id,cid;	
on=lookup_symbol(tn,cname);
if (on>=0) { printf("taskmgr: create_class duplicate class %s created\n",cname); }
cid.item=0;		// class class
cid.type=OT_BASEOBJ;
id=create_object(tn,"defclass",0);
taskset(tn,id,0,CA_CLASSNAME,cname);
on=create_symbol(tn,cname,cid,0); 
setid(tn,on,id);
return(id);
}

/*<>*/
oid taskmgr::create_object(int tn,oid cid,int asize)
{
oid id;
if (tn<0) tn=active;
id=tasklist[tn].create_object(cid,asize);
return(id);
}

/*<>*/
oid taskmgr::create_object(int tn,char *cname,int asize)
{
int bsize,on,an,len;
char *pname,*atype,*ip;
oid id,cid,pid,sid,num,*alist;
if (!strcmp(cname,"shint")) 
	{ id.type=OT_SHINT; id.seg=asize; id.item=0; return(id); }
//printf("TM: create_object class %s \n",cname);
if (!strcmp(cname,"string")) 
	{ id=tasklist[tn].create_object(cname,asize); return(id); }
if (!strcmp(cname,"defclass")) 
	{ id=tasklist[tn].create_object(cname,asize); return(id); }
if (!strcmp(cname,"block")) 
	{ id=tasklist[tn].create_object(cname,asize); return(id); }
cid=lookup_object(tn,cname);
ip = (char *) getaddress(tn,cid);
bsize=atoi(bc.getobj(0,ip,0,CA_OBJSIZE));
// create the object
id=create_object(tn,cid,asize+bsize);
//printf("TM: create_object class %s id %s\n",cname,id.aschar());
// create any parent objects
pname=taskget(tn,cid,0,CA_PARENT);
if (pname[0])
	{
	pid=create_object(tn,pname,0);
	tasklist[tn].setparent(id,pid);
	}
//printf("TM: create_object class %s cid %s size %d parent %s\n",cname,cid.aschar(),bsize,pname);
// run constructor
if (cid.type==OT_BINARY || cid.type==OT_CLASS) 
	{
	sendmsg(tn,id,0,0,0);  // construct defined or base objects 
	}
return(id);
}

/*<>*/
oid taskmgr::getid(int tn,int on)
{
oid id;
if (tn<0) tn=active;
id=tasklist[tn].getid(on);
//printf("TM: get id for symbol %d = %s\n",on,id.aschar());
return(id);
}

/*<>*/
void taskmgr::setname(int tn,oid id,char *name)
{
if (tn<0) tn=active;
tasklist[tn].setname(id,name);
}

/*<>*/
char *taskmgr::getname(int tn,oid id)
{
if (tn<0) tn=active;
if (id.type==OT_SHARE)
	return(smgr.getname(id.item));
if (id.type==OT_SYMBOL) 
	return(tasklist[tn].getname(id.item));
if (id.type==OT_CLASS) id.type=0;
if (id.type==OT_BASEOBJ) id.type=0;
return(tasklist[tn].getname(id));
}

/*<>*/
char *taskmgr::getname(int tn,int id)
{
if (tn<0) tn=active;
return(tasklist[tn].getname(id));
}

/*<>*/
void *taskmgr::getaddress(int tn,oid id)
{
if (tn<0) tn=active;
return(tasklist[tn].getaddress(id));
}

/*<>*/
void taskmgr::setparent(int tn,oid cid,char *pname)
/* set the parent of any class */
/* copy attributes and methods */
{
oid pid;
char *np;
int i,rc;
taskset(tn,cid,0,CA_PARENT,pname);
pid=lookup_object(tn,pname);
if (pid.item>=0)
	{
	np=taskget(tn,pid,1,CA_METNAME);
	for(i=1; np[0]; i++)
		{
		rc=sendmsg(tn,cid,CM_ADDMET,0,0);
		if (rc<0) return;
		taskset(tn,cid,rc,CA_METNAME,np);
		np=taskget(tn,pid,i+1,CA_METNAME);
		}
	np=taskget(tn,pid,0,CA_ATTRNAME);
	for(i=0; np[0]; i++)
		{
		rc=sendmsg(tn,cid,CM_ADDATTR,0,0);
		if (rc<0) return;
		taskset(tn,cid,rc,CA_ATTRNAME,np);
		taskset(tn,cid,rc,CA_DATATYPE,"parent");
		np=taskget(tn,pid,i+1,CA_ATTRNAME);
		}
	}
}

/*<>*/
void taskmgr::share(char *oname)
{
oid id;
int img,tn;
id=lookup_object(-1,oname);
if (id.item<0) return; 
img=getsession(-1);
tn=getactive();
smgr.addobj(oname,id,img,tn);
}


/*<>*/
long taskmgr::exec_block(int tn,oid bid,int num,oid parm[])
// execute a code block
// there is no use made of parameters yet
// parm[0] will be self (the containing class)
{
long rc=0;
oid id,pnum,*pid;
int i,number,ins,nparm,msg;
number=sendmsg(tn,bid,BLOCK_GETNUMBER,0,0);
//printf("TM: exec_block size %d\n",number);
for(i=0; i<number; i++) 
	{
	pnum=create_object(tn,"shint",i);
	ins=sendmsg(tn,bid,BLOCK_GETINST,1,&pnum);
//printf("TM: exec_block ins  %d\n",ins);
	switch(ins)
       		{	
case SENDMSG:				
nparm=sendmsg(tn,bid,BLOCK_GETNPARM,1,&pnum);
msg=sendmsg(tn,bid,BLOCK_GETMSG,1,&pnum);
rc=sendmsg(tn,bid,BLOCK_GETID,1,&pnum);
id.setint(rc);
if (id.seg<0) id=parm[0];
pid = (oid *) sendmsg(tn,bid,BLOCK_GETPARM,1,&pnum);
//printf("TM: exec_block SMSG  nparm %d msg %d id %s\n",nparm,msg,id.aschar());
rc=sendmsg(tn,id,msg,nparm,pid);
i+=nparm;
break;

case RETURN:				
return(rc);

case IFTRUE:				
case IFFALSE:				
case REPEAT:				
case WHILETRUE:				
case WHILEFALSE:				
break;

case STARTBLK:				
break;

case ENDBLOCK:				
case PARAMETER:				
break;

default:
i=number;
break;
		}
	}
return(rc);
}

/*<>*/
long taskmgr::message(int sn,int msg,int num,oid parm[])
/* class method entry point */
{
long rc;
int i,len,tn;
int ip=0;
char *cp = "";
tn=getactive();
if (num)
	{
        ip = parm[0].getseg();
	if (parm[0].gettype()==OT_STRING) cp = SM->sessget(sn,parm[0],0,0);
        }
rc=0;
switch(msg)
	{
case 0: 	
//init();
break;

case 2:
exit();
break;

case 3:
start(tn,cp);
break;

case 4:
exec(tn,cp);
break;

case 5:
open_image(cp);
break;

default:
rc = -1;
break;
	}
return(rc);
}

#ifdef DISPLAY

/*<>*/
void execmgr::exit(void)
{
int tn,im,ai;
oid id,cid,pid;
tn=getactive();
ai=getimage(tn);
tn=0;
im=getimage(tn);
while(im != -2)
	{
	if (im==ai)
		{
		id=getobject(tn);
		cid=getclass(tn,id);
		pid.type=OT_SHINT; pid.seg=0; pid.item=0;
		if (!strcmp("display",taskget(tn,cid,0,CA_PARENT)))
			sendmsg(tn,id,D_SETVISIBLE,1,&pid);
		}
	im=getimage(++tn);
	}
exit();
}


#endif
