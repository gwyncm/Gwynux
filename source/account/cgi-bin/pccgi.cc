#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include "oid.hpp"
#include "taskmgr.hpp"
#include "scanner.hpp"

#define MAXMESG 1600

FILE	*savefile;

static char mesg[MAXMESG];
static char line[2000];
static char buf[2000];
static scanner	scan;	

static int xtoi(char *cp)
{
int i,l;
l=0;
for(i=0; cp[i]; i++)
	{
	l<<=4;
	if (isdigit(cp[i]))
		l|=(cp[i]-'0');
	else
		l|=((toupper(cp[i])-'A')+10);
	}
return(l);
}

FILE 		*ofile;
short VERSION = 0x1204;
 
#define TBUFSIZE 8000
#define TLMAX 200
static char tbuf[TBUFSIZE+1];
static int  tlist[TLMAX+1];

static int parse(char *hmsg)
{
char hn[4];
int num,i,n;
printf("<%s>\n",hmsg);
n=num=0;
tlist[0]=0;
for(i=0; hmsg[i] && n<TBUFSIZE; i++)
	{
	switch(hmsg[i])
		{
	case '+': 
	tbuf[n++]=' '; 
	break;

	case '&': 
	case '\n': 
	tbuf[n++]=0;
	tlist[++num]=n;	
	if (num>=TLMAX) num--;
	break;
	
	case '%':
	hn[0]=hmsg[i+1];	
	hn[1]=hmsg[i+2];	
	hn[2]=0;
	i+=2;
	tbuf[n++]=xtoi(hn);
	break;

	default:
	tbuf[n++]=hmsg[i];
	break;
		}
	}
return(num);
}

int main(void)
{
short suid;
FILE *fp;
char *cp;
int size,num,rc;
printf("Content-type: text/html\n\n");
//getcwd(buf,2000);
//suid=geteuid();
//printf("running from %s with uid %d\n",buf,suid);
fp=fopen("/tmp/request.sh","w");
cp=getenv("CONTENT_LENGTH");
if (!cp) { printf("Content Length Undefined\n"); fclose(fp);  return(0); }
size=atol(cp);
fprintf(fp,"CONTENT_LENGTH=%d\n",size);
fprintf(fp,"export CONTENT_LENGTH\n");
fread(line,size,1,stdin);
cp=getenv("QUERY_STRING");
fprintf(fp,"QUERY_STRING=%s\n",cp);
fprintf(fp,"export QUERY_STRING\n");
fclose(fp);
fp=fopen("/tmp/request.html","w");
fprintf(fp,"%s",line);
fclose(fp);
num=parse(line);
rc=chdir(cp);
if (rc) { printf("Directory %s Invalid\n",cp); return(0); }
//in.setinput(sizeof(tbuf),tbuf,num,tlist); 
TM->exit();
TM->open_image(cp);
TM->taskmsg();
fp=fopen("/tmp/response.html","r");
if (!cp) { printf("Response Invalid\n"); return(0); }
while(fgets(mesg,sizeof(mesg),fp))
	printf("%s\n",mesg);
fclose(fp);
}

