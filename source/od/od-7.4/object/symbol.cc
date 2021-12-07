#include <stdio.h>
#include <string.h>

#include "oid.hpp"
#include "symbol.hpp"

symbol::symbol(void)
{
init();
}

void symbol::init(void)
{
name[0]  = 0;
id.init();
cid.init();
size = 0;
}

void symbol::setname(char *aname)
{
strncpy(name,aname,sizeof(name));
}

int symbol::test(char *aname)
{
if (!strcmp(aname,name)) return(1);
return(0);
}

char *symbol::getname(void)
{
return(name);
}

void symbol::setid(oid nid)
{
id=nid;
}

oid symbol::getid(void)
{
return(id);
}

void symbol::setcid(oid nid)
{
cid=nid;
}

void symbol::setsize(int sz)
{
size=sz;
}

int symbol::getsize(void)
{
return(size);
}

oid symbol::getcid(void)
{
return(cid);
}
