#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "object.hpp"

object::object(void)
{
init();
}

void object::init(void)
{
ref 	= 0;
size 	= 0;
start	= 0;
numvar	= 0;
myclass.init();
parent.init();
}

int object::get_start(void)
{
return(start);
}

int object::get_size(void)
{
return(size);
}

int object::get_ref(void)
{
return(ref);
}

void object::set_start(int st)
{
start=st;
}

void object::set_ref(int re)
{
ref=re;
}

void object::set_size(int sz)
{
size=sz;
}

oid object::get_class(void)
{
return(myclass);
}

void object::set_class(oid mc)
{
myclass = mc;
}

void object::set_parent(oid pp)
{
parent=pp;
}

oid object::get_parent(void)
{
return(parent);
}

void object::show(FILE *fp,int seg,int item)
{
// used to produce object listing
if (ref)
	{
	fprintf(fp,"id %2d.%3d ref %1d numvar %2d size %4d start %4d class %s ", 
		seg,item,ref,numvar,size,start,myclass.aschar());
	fprintf(fp,"parent %s\n",
		parent.aschar());
	}
}
