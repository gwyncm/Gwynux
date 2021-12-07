#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oid.hpp"
#include "block.hpp"
#include "objmgr.hpp"
#include "imgmgr.hpp"
#include "symtab.hpp"

#include "string.hpp"
#include "defclass.hpp"

objmgr* objmgr::_instance=0;

objmgr* objmgr::getinstance()
{
if (_instance == 0) _instance = new objmgr;
return (_instance);
}

void objmgr::init(void)
{
}

objmgr::objmgr(void)
{
symlist.item=0;		// first object is the symtab
} 

/*<>*/
oid objmgr::create_object(int curimg,char *cname,int asize)
{
oid id;
block *bp;
string *sp;
defclass *dp;
// where does the class come from 
// how is the type tagged
oid empty,cid;
int type,size;
if (!strcmp(cname,"shint"))
	{ id.type=OT_SHINT; id.seg=asize; id.item=0; return(id); }
if (!strcmp(cname,"string"))
	{
	id=IM->create_object(curimg,cid,asize+sizeof(string));
	sp = (string *)getaddress(curimg,id);
	sp->message(0,0,0); // construct
	id.type=OT_STRING;
	return(id);
	}
if (!strcmp(cname,"defclass"))
	{
	id=IM->create_object(curimg,cid,asize+sizeof(defclass));
	dp = (defclass *)getaddress(curimg,id);
	dp->message(0,0,0,0); // construct
	id.type=OT_CLASS;
	return(id);
	}
if (!strcmp(cname,"block"))
	{
	id=IM->create_object(curimg,cid,asize+sizeof(defclass));
	bp = (block *)getaddress(curimg,id);
	bp->message(0,0,0); // construct
	id.type=OT_BLOCK;
	return(id);
	}
return(empty);
}

/*<>*/
oid objmgr::create_object(int curimg,oid cid,int asize)
{
return(IM->create_object(curimg,cid,asize));
}

/*<>*/
void objmgr::delete_object(int curimg,oid id)
{
IM->delete_object(curimg,id);
}

/*<>*/
oid objmgr::getparent(int curimg,oid id)
{
return(IM->getparent(curimg,id));
}

/*<>*/
oid objmgr::getclass(int curimg,oid id)
{
return(IM->getclass(curimg,id));
}


/******************** image functions ****************/

/*<>*/
int objmgr::create_image(char *fname)
{
int rc;
oid id;
symtab *op;
rc=IM->create_image(fname);
//printf("OM: image created rc=%d\n",rc);
if (rc<0) return(rc);
id=IM->create_object(rc,id,sizeof(symtab));	// create symtab
//printf("OM: object created rc=%s\n",id.aschar());
op = (symtab *) IM->getaddress(rc,id);		// get address
//printf("OM: symtab address=%p\n",op);
op->init();					// construct symtab
//printf("OM: symtab constructed\n");
return(rc);
}

int objmgr::open_image(char *fname)
{
return(IM->open_image(fname));
}

int objmgr::close_image(int curimg)
{
return(IM->close_image(curimg));
}

void objmgr::showmap(int curimg)
{
symtab *op;
IM->showmap(curimg);
op = (symtab *) getaddress(curimg,symlist);
op->show(IM->getimgname(curimg));
}

char *objmgr::getimgname(int curimg)
{
return(IM->getimgname(curimg));
}

void objmgr::setimgname(int curimg,char *name)
{
IM->setimgname(curimg,name);
}

void *objmgr::getaddress(int curimg,oid id)
{
IM->getaddress(curimg,id);
}

void objmgr::setparent(int curimg,oid id,oid pid)
{
return(IM->setparent(curimg,id,pid));
}

/*********************  symbol functions *****************/

/*<>*/
int objmgr::create_symbol(int curimg,char *sname,oid cid,int size)
{
int rc;
symtab *op;
op = (symtab *) IM->getaddress(curimg,symlist);
rc=op->create_symbol(sname,cid,size);
return(rc);
}

/*<>*/
void objmgr::delete_symbol(int curimg,int on)
{
symtab *op;
op = (symtab *) IM->getaddress(curimg,symlist);
op->delete_symbol(on);
}

/*<>*/
int objmgr::lookup_symbol(int curimg,char *oname)
{
int sid;
symtab *op;
op = (symtab *) IM->getaddress(curimg,symlist);
sid=op->lookup_symbol(oname);
return(sid);
}

/*<>*/
oid objmgr::getid(int curimg,int on)
/* get an object id for a symbol */
{
oid id;
symtab *op;
op = (symtab *) getaddress(curimg,symlist);
id=op->getid(on);
return(id);
}

/*<>*/
oid objmgr::getclass(int curimg,int on)
/* return the class id of a symbol */
{
oid cid;
symtab *op;
op = (symtab *) getaddress(curimg,symlist);
cid=op->getcid(on);
return(cid);
}

/*<>*/
void objmgr::setid(int curimg,int on,oid id)
/* set an object id in the symtab */
{
symtab *olp;
olp = (symtab *) getaddress(curimg,symlist);
olp->setid(on,id);
}

/*<>*/
char *objmgr::getname(int curimg,int on)
/* get the name of an object */
{
char *cp;
symtab *olp;
olp = (symtab *) getaddress(curimg,symlist);
cp=olp->getname(on);
return(cp);
}

/*<>*/
char *objmgr::getname(int curimg,oid on)
/* get the name of an object */
{
char *cp;
symtab *olp;
olp = (symtab *) getaddress(curimg,symlist);
cp=olp->getname(on);
return(cp);
}

/*<>*/
void objmgr::setname(int curimg,oid on,char *name)
/* set an object name in the symtab */
{
symtab *olp;
olp = (symtab *) getaddress(curimg,symlist);
olp->setname(on,name);
}


/********************  object get/set  **********************/

/*<>*/
char *objmgr::taskget(int curimg,oid id,int line,int fld)
{
int stn;
oid sid;
char *rc;
block *bp;
string *sp;
defclass *dp;
static char retint[8];
//printf("OM: taskget id %s line %d fld %d\n",id.aschar(),line,fld);
if (id.type==OT_SHINT) { sprintf(retint,"%d",id.seg); return(retint); } 
if (id.type==OT_STRING)	
	{
	sid=id; sid.type=0;
	sp = (string *)getaddress(curimg,sid);
	rc=sp->get(line,fld);
	return(rc);
	}
if (id.type==OT_CLASS)	
	{
	sid=id; sid.type=0;
	dp = (defclass *)getaddress(curimg,sid);
	rc=dp->get(line,fld);
	return(rc);
	}
if (id.type==OT_BLOCK)	
	{
	sid=id; sid.type=0;
	bp = (block *)getaddress(curimg,sid);
	rc=bp->get(line,fld);
	return(rc);
	}
return("");
}

/*<>*/
void objmgr::taskset(int curimg,oid id,int line,int fld,char *value)
{
int stn;
oid sid;
block *bp;
string *sp;
defclass *dp;
if (id.type==OT_STRING)	
	{
	sid=id; sid.type=0;
	sp = (string *)getaddress(curimg,sid);
	sp->set(line,fld,value);
	}
if (id.type==OT_CLASS)	
	{
	sid=id; sid.type=0;
	dp = (defclass *)getaddress(curimg,sid);
	dp->set(line,fld,value);
	}
if (id.type==OT_BLOCK)	
	{
	sid=id; sid.type=0;
	bp = (block *)getaddress(curimg,sid);
	bp->set(line,fld,value);
	}
}

/*<>*/
long objmgr::sendmsg(int curimg,oid id,int msg,int num,oid parm[])
{
int stn;
oid sid;
long rc;
block *bp;
string *sp;
defclass *dp;
if (id.type==OT_STRING)	
	{
	sid=id; sid.type=0;
	sp = (string *)getaddress(curimg,sid);
	rc=sp->message(msg,num,parm);
	return(rc);
	}
if (id.type==OT_CLASS)	
	{
	sid=id; sid.type=0;
	dp = (defclass *)getaddress(curimg,sid);
	rc=dp->message(0,msg,num,parm);
	return(rc);
	}
if (id.type==OT_BLOCK)	
	{
	sid=id; sid.type=0;
	bp = (block *)getaddress(curimg,sid);
	rc=bp->message(msg,num,parm);
	return(rc);
	}
}
	
