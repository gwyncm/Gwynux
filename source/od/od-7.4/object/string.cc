#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "oid.hpp"
#include "string.hpp"

string::string(void)
{
init();
}

void string::init(void)
{
//printf("string init address %p\n",&len);
max=0;
len=0;
data[0]=0;
}

int string::size(void)
{
return(sizeof(string));
}

void string::setmax(int mx)
{
max=mx;
}

void string::setlen(int ln)
{
len=ln;
}

int string::getlen(void)
{
return(len);
}

void string::system(void)
{
::system(data);
}

void string::set(int line,int fld,char *value)
/* sets an attribute value */
{
strncpy(data,value,max);
len=strlen(value);
}

char *string::get(int line,int off)
{
return(data);
}

long string::message(int msg,int num,oid parm[])
{
long rc;
int ip=0;
if (num) ip = parm[0].getseg();
rc=0;
switch(msg)
	{
case 0:
init();
break;

case 1:
setmax(ip);
break;

case 2:
setlen(ip);
break;

case 3:
system();
break;

case 4:
rc = getlen();
break;
	}
return(rc);
}


