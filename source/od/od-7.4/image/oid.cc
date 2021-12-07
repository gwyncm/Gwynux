#include <stdio.h>
#include "oid.hpp"

typedef struct { short seg; char item; unsigned char type; } map;
typedef union { map ioid; int iint; } imp;

char *types[] = 
	{ 
	"binary ",
	"shint  ",
	"baseobj",
	"symbol ",
	"string ",
	"class  ",
	"share  ",
	"block  ",
	"inst   "
	};
	
oid::oid(void)
{
init();
}

void oid::init(void)
{
seg  = 0;
item = -1;
type = OT_BINARY;
}

int oid::gettype(void)
{
return(type&15);
}

int oid::getcode(void)
{
return(type>>4);
}

int oid::getobj(void)
{
return(seg);
}

int oid::getseg(void)
{
return(seg);
}

int oid::getitem(void)
{
return(item);
}

int oid::getmsg(void)
{
return(item);
}

void oid::setcode(int cd)
{
type=cd<<4;
}

void oid::settype(int ty)
{
type=ty;
}

void oid::setobj(int ob)
{
seg=ob;
}

void oid::setseg(int sn)
{
seg=sn;
}

void oid::setmsg(int me)
{
item=me;
}

char *oid::astype(void)
{
return(types[type&0xf]);
}

char *oid::aschar(void)
{
static char result[12];
sprintf(result,"%01d%01d.%2d.%3d",type>>4,type&15,seg,item);
return(result);
}

int oid::asint(void)
{
imp im;
im.ioid.seg  = seg;
im.ioid.item = item;
im.ioid.type = type;
return(im.iint);
}

void oid::setint(int in)
{
imp im;
im.iint=in;
seg  = im.ioid.seg;
item = im.ioid.item;
type = im.ioid.type;
}
