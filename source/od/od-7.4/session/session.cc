#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "objmgr.hpp"
#include "session.hpp"

void session::init(void)
{
curimg = 0;
status = 0;
}

/*<>*/
int session::close_image(void)
{
return(OM->close_image(curimg));
}

/*<>*/
int session::open_image(char *fname)
{
curimg=OM->open_image(fname);
return(curimg);
}

/*<>*/
int session::create_image(char *fname)
{
curimg=OM->create_image(fname);
return(curimg);
}

/*<>*/
int session::getimage(void)
{
return(curimg);
}

/*<>*/
char *session::getimgname(void)
{
if (!status) return(0);
return(OM->getimgname(curimg));
}

/*<>*/
void session::setimgname(char *name)
{
if (!status) return;
OM->setimgname(curimg,name);
}

/*<>*/
int session::getstatus(void)
{
return(status);
}

/*<>*/
void session::setstatus(int st)
{
status=st;
}

/*<>*/
void session::showmap(void)
{
OM->showmap(curimg);
}


/*<>*/
oid session::create_object(oid cid,int asize)
{
return(OM->create_object(curimg,cid,asize));
}

/*<>*/
oid session::create_object(char *cname,int asize)
{
return(OM->create_object(curimg,cname,asize));
}

/*<>*/
void session::delete_object(oid id)
{
OM->delete_object(curimg,id);
}

/*<>*/
void session::delete_symbol(int on)
{
OM->delete_symbol(curimg,on);
}

/*<>*/
int session::create_symbol(char *sname,oid cid,int size)
{
return(OM->create_symbol(curimg,sname,cid,size));
}

/*<>*/
oid session::getparent(oid id)
{
return(OM->getparent(curimg,id));
}

/*<>*/
oid session::getclass(oid id)
{
return(OM->getclass(curimg,id));
}

/*<>*/
oid session::getclass(int on)
{
return(OM->getclass(curimg,on));
}

/*<>*/
oid session::getid(int id)
{
return(OM->getid(curimg,id));
}

/*<>*/
int session::lookup_symbol(char *oname)
{
return(OM->lookup_symbol(curimg,oname));
}

/*<>*/
void session::setname(oid id,char *name)
{
OM->setname(curimg,id,name);
}

/*<>*/
void session::setparent(oid id,oid pid)
{
OM->setparent(curimg,id,pid);
}

/*<>*/
void session::setid(int on,oid id)
{
OM->setid(curimg,on,id);
}

/*<>*/
char *session::getname(oid id)
{
return(OM->getname(curimg,id));
}

/*<>*/
char *session::getname(int id)
{
return(OM->getname(curimg,id));
}

/*<>*/
void *session::getaddress(oid id)
{
return(OM->getaddress(curimg,id));
}

/*<>*/
char *session::sessget(oid id,int line,int fld)
{
return(OM->taskget(curimg,id,line,fld));
}

/*<>*/
void session::sessset(oid id,int line,int fld,char *value)
{
OM->taskset(curimg,id,line,fld,value);
}

/*<>*/
long session::sendmsg(oid id,int msg,int num,oid parm[])
{
return(OM->sendmsg(curimg,id,msg,num,parm));
}
	
