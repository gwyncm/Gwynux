#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "sessmgr.hpp"
#include "task.hpp"

void task::init(void)
{
obj.item = -1;
cursess = 0;
priority = 0;
counter = 0;
status = 0;
}

/*<>*/
int task::taskmsg(void)
// returns status if task is runable
{
clock_t ticks;
if (!status) return(0);
//printf("TASK: taskmsg obj=%s\n",obj.aschar());
if (obj.item<0) return(0);
if (priority<0) return(0);
if (priority)
	{
	ticks=time(0);
	if (ticks<counter) return(0);
	counter=ticks+priority;
	}
return(status);
}

/*<>*/
void task::setpriority(int py)
{
priority=py;
}

/*<>*/
int task::getpriority(void)
{
return(priority);
}

/*<>*/
oid task::getobject(void)
{
return(obj);
}

/*<>*/
void task::setobject(oid on)
{
obj=on;
}

/*<>*/
int task::close_image(void)
{
return(SM->close_image(cursess));
}

/*<>*/
int task::open_image(char *fname)
{
cursess=SM->open_image(fname);
return(cursess);
}

/*<>*/
int task::create_image(char *fname)
{
cursess=SM->create_image(fname);
return(cursess);
}

/*<>*/
int task::getsession(void)
{
return(cursess);
}

/*<>*/
char *task::getimgname(void)
{
if (!status) return(0);
//return(SM->getimgname(cursess));
return("fix me");
}

/*<>*/
void task::setimgname(char *name)
{
if (!status) return;
//SM->setimgname(cursess,name);
}

/*<>*/
int task::getstatus(void)
{
return(status);
}

/*<>*/
void task::setstatus(int st)
{
status=st;
}

/*<>*/
void task::showmap(void)
{
SM->showmap(cursess);
}


/*<>*/
oid task::create_object(oid cid,int asize)
{
return(SM->create_object(cursess,cid,asize));
}

/*<>*/
oid task::create_object(char *cname,int asize)
{
return(SM->create_object(cursess,cname,asize));
}

/*<>*/
void task::delete_object(oid id)
{
SM->delete_object(cursess,id);
}

/*<>*/
void task::delete_symbol(int on)
{
SM->delete_symbol(cursess,on);
}

/*<>*/
int task::create_symbol(char *sname,oid cid,int size)
{
return(SM->create_symbol(cursess,sname,cid,size));
}

/*<>*/
oid task::getparent(oid id)
{
return(SM->getparent(cursess,id));
}

/*<>*/
oid task::getclass(oid id)
{
return(SM->getclass(cursess,id));
}

/*<>*/
oid task::getclass(int on)
{
return(SM->getclass(cursess,on));
}

/*<>*/
oid task::getid(int id)
{
return(SM->getid(cursess,id));
}

/*<>*/
int task::lookup_symbol(char *oname)
{
return(SM->lookup_symbol(cursess,oname));
}

/*<>*/
void task::setname(oid id,char *name)
{
SM->setname(cursess,id,name);
}

/*<>*/
void task::setparent(oid id,oid pid)
{
SM->setparent(cursess,id,pid);
}

/*<>*/
void task::setid(int on,oid id)
{
SM->setid(cursess,on,id);
}

/*<>*/
char *task::getname(oid id)
{
return(SM->getname(cursess,id));
}

/*<>*/
char *task::getname(int id)
{
return(SM->getname(cursess,id));
}

/*<>*/
void *task::getaddress(oid id)
{
return(SM->getaddress(cursess,id));
}

/*<>*/
char *task::taskget(oid id,int line,int fld)
{
return(SM->sessget(cursess,id,line,fld));
}

/*<>*/
void task::taskset(oid id,int line,int fld,char *value)
{
SM->sessset(cursess,id,line,fld,value);
}

/*<>*/
long task::sendmsg(oid id,int msg,int num,oid parm[])
{
return(SM->sendmsg(cursess,id,msg,num,parm));
}
	
