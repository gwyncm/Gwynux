#include <stdio.h>
#include <string.h>
#include "oid.hpp"
#include "syslog.hpp"

#define MAX_ERRORS 40

syslog* syslog::_instance = 0;

/*<>*/
syslog* syslog::getinstance()
{
if (_instance == 0) _instance = new syslog;
return (_instance);
}

syslog::syslog(void)
{
filename[0]=0;
echo=0;
errors=0;
fp=0;
}

void syslog::init(void)
{
// do not reinit
}

char *syslog::get(int line,int fld)
{
return("");
}

void syslog::set(int line,int fld,char *value)
{
log_error(value);
}

void syslog::set_file(char *name)
{
strncpy(filename,name,sizeof(filename));
//printf("syslog: setting filename to <%s>\n",name);
}

void syslog::log_message(char *msg)
{
if (filename[0])
	{
	if (!fp) fp=fopen(filename,"w");
	if (fp) fprintf(fp,"%s\n",msg);
	}
if (!filename[0] || echo)
	printf("%s\n",msg);
}

void syslog::log_error(char *err)
{
if (errors>MAX_ERRORS) return;
if (filename[0])
	{
	if (!fp) fp=fopen(filename,"w");
	if (fp) fprintf(fp,"*** ERROR *** %s\n",err);
	}
if (!filename[0] || echo)
	printf("*** ERROR *** %s\n",err);
errors++;
if (errors==MAX_ERRORS) log_error("Max errors exceeded");
}

int syslog::get_errors(void)
{
return(errors);
}

void syslog::set_echo(int ec)
{
echo=ec;
}

/*<>*/
long syslog::message(int msg,int num,oid parm[])
/* class method entry point */
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

case 3:
rc=get_errors();
break;

	}
return(rc);
}
