#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#define MAXMESG 1600

static char mesg[MAXMESG];
static char line[2000];
static char buf[2000];

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
//printf("<%s>\n",hmsg);
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

int main(int argc,char *argv[])
{
short suid;
FILE *fp;
char *cp,*tp;
int size,num,rc,i;
printf("Content-type: text/html\n\n");
getcwd(buf,2000);
suid=geteuid();
//printf("running from %s with uid %d\n",buf,suid);
cp=getenv("CONTENT_LENGTH");
if (!cp) { printf("Content Length Undefined\n"); return(0); }
size=atol(cp);
fread(line,size,1,stdin);
//printf("%d characters read\n",size);
num=parse(line);
//printf("%d tokens\n<%s>\n",num,line);
if (argc<2) { printf("Missing Filename\n"); return(0); } 
//printf("%d args %s\n",argc,argv[1]);
fp=fopen(argv[1],"a");
for(i=0; i<=num; i++) 
	{
	tp=strchr(&tbuf[tlist[i]],'=');
	if (tp)
		{
		tp++;
//		fprintf(fp,"%s:",tp);
		fwrite(tp,strlen(tp),1,fp);
		fprintf(fp,":");
//		printf("Token length %d\n",strlen(tp));
//		printf("Token address %d\n",tp);
		}
	}
fprintf(fp,"\n");
fclose(fp);
}

