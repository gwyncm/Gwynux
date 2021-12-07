#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "oid.hpp"
#include "block.hpp"

#define MAXINS 	 1000
static int trace=0;

#define MAXTYPES	10
const char *instype[MAXTYPES] = 
	{
	"parameter",
	"sendmsg",
	"iftrue",
	"iffalse",
	"repeat",
	"return",
	"whiletrue",
	"whilefalse",
	"startblk",
	"endblk"
	};
	
block::block(void)
{
init();
}

void block::init(void)
{
number=0;
curline=0;
}

/*<>*/
int block::size(void)
{
// do not include size of instruction list
return(2*sizeof(short));
}

void block::set(int line,int fld,char *value)
/* sets an attribute value */
{
int code;
if (line>number) return;
switch(fld)
	{
case 0:		// opcode
code=getinstype(value);
if (code<0) return;
if (line==number) gencode(code);
if (line<number) setcode(line,code);	
break;

case 1:		// object/value
blist[line].setobj(atoi(value));
break;

case 2:		// msgid
blist[line].setmsg(atoi(value));
break;

case 3:		// parameter
blist[line].setint(atoi(value));
break;

case 4:		// number
number=atoi(value);
break;

case 5:		// object
blist[line].setint(atoi(value));
break;

	}
}

char *block::get(int line,int fld)
/* returns an attribute value */
{
int in;
static char result[20];
if (line>=number) return("");
switch(fld)
	{
case 0:		// opcode
curline=line;
in=blist[line].getcode();
if (in<MAXTYPES && in>=0) return(getinstype(in));
return("invalid");

case 1:		// object
sprintf(result,"%d",blist[line].getobj());
return(result);

case 2:		// message
sprintf(result,"%d",blist[line].getmsg());
return(result);

case 3:		// parm
sprintf(result,"%d",blist[line].asint());
return(result);

case 4:		// number
sprintf(result,"%d",number);
return(result);
	}
return("");
}

int block::getnumber(void)
{
return(number);
}

oid block::getid(int in)
{
oid id;
id.setseg(blist[in].getobj());
id.settype(OT_SYMBOL);
return(id);
}

int block::getins(int in)
{
return(blist[in].getcode());
}

int block::getnparm(int in)
{
int i;
for(i=in; i<number; i++)
	if (getins(i+1)!=PARAMETER) return(i-in);
return(i);
}

oid *block::getparm(int in)
{
return((oid *)&blist[in+1]); 
}

char *block::getinstype(int in)
{
static char rinse[12];
if (in<0 || in>=MAXTYPES) return("INVALID");
strcpy(rinse,instype[in]);
return(rinse); 
}

int  block::getinstype(char *in)
{
int i;
for(i=0; i<MAXTYPES; i++)
	if (!strcmp(in,instype[i])) return(i);
return(-1);
}

int block::getmsg(int in)
{
return(blist[in].getmsg());
}

void block::genmsg(int tn,int objnum,int msgid)
{
if (number>=MAXINS) 
	{
	printf("Method buffer overflow\n");
	exit(0);
	}
blist[number].setcode(SENDMSG);
blist[number].setmsg(msgid);
blist[number].setobj(objnum);
number+=1;
}

void block::gencode(int code)
{
if (number>=MAXINS) 
	{
	printf("Method buffer overflow\n");
	exit(0);
	}
blist[number].setcode(code);
blist[number].setmsg(0);
blist[number].setobj(0);
number++;
}

void block::setcode(int ln,int code)
{
if (ln>=number)  return;
blist[ln].setcode(code);
blist[ln].setmsg(0);
blist[ln].setobj(0);
}

void block::genparm(oid sid)
{
if (number<1) 
	{
	printf("Parameter sequence error\n");
	exit(0);
	}
if (number>=MAXINS) 
	{
	printf("Method buffer overflow\n");
	exit(0);
	}
blist[number]=sid;
number++;
}

/*<>*/
void block::additem(void)
{
int i;
if (curline>=number) return;
for(i=number; i>curline; i--) blist[i] = blist[i-1];
blist[curline].setcode(SENDMSG);
number++;
}

/*<>*/
void block::delitem(void)
{
int i;
if (curline>=number) return;
for(i=curline; i<number; i++) blist[i] = blist[i+1];
number--;
}

long block::message(int msg,int num,oid parm[])
{
int rc=0,ip=0;
if (num) ip = parm[0].getseg();
switch(msg)
	{
case 0:
init();
break;

case 1:
additem();
break;

case 2:
delitem();
break;

case 3:
rc=getnumber();
break;

case 4:
rc=getins(ip);
break;

case 5:
rc=getid(ip).asint();
break;

case 6:
rc=getnparm(ip);
break;

case 7:
rc=getmsg(ip);
break;

case 8:
rc = (long) getparm(ip);
break;

	}
return(rc);
}

