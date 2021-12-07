#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#include "oid.hpp"
#include "sessmgr.hpp"
#include "remote.hpp"

#define MAX_FIELDS 100
static char buffer[3000];
static int  flist[MAX_FIELDS];
static int  llist[MAX_FIELDS];
static char *bplist[MAX_FIELDS*MAX_FIELDS];
static int  fcount=0;
static int  object = -1;

#define MAXMESG 3000

static char qmesg[MAXMESG];
static char mesg[MAXMESG];
static char repl[MAXMESG];
static int sd, len;
static struct sockaddr_in sa;
static struct sockaddr sa2;
static int sock_open = 0;
static char buf[2000];
static int ipaddr = 0x0100007f;
static char cmsg[20];
static char cbuf[200];
static char gbuf[200];

#define DEFPERM 0644

extern "C" void rtcpinit(int port); 
extern "C" void rtcpclose(void);
extern "C" char *rtcpreceive(void);
extern "C" void rtcpsend(char *m);
extern "C" void rprmsg(char *m);

void remote::init(void) 
{ 
if (!sock_open) rtcpinit(2000);
}

remote::remote(void) 
{ 
}

char *remote::check(void) 
// returns image to be loaded
{ 
int i;
char *cp;
static int count;
if (!sock_open) return(0);
if (++count<100000) return(0);
count=0;
sprintf(cmsg,"p\n");
rtcpsend(cmsg); 
cp=rtcpreceive();
if (!cp) return(0); 
for(i=0; cp[i] && cp[i]>=' ' && i<sizeof(cbuf); i++) cbuf[i]=cp[i];
cbuf[i]=0;
return(cbuf);
}

remote::~remote(void)
{
rtcpclose();
}

int remote::size(void) 
{ 
return(sizeof(remote));
}

/*<>*/
char *remote::get(int myid,int line,int fld)
{
union { long ip; unsigned char cip[4]; } ipa;
int i;
char *cp;
if (fld == -1000)
	{
	ipa.ip=ipaddr;
	sprintf(buf,"%d.%d.%d.%d",ipa.cip[0],ipa.cip[1],ipa.cip[2],ipa.cip[3]);
	if (ipa.ip==0) return(""); else return(buf);
	}
if (object==myid)
	{
	for(i=0; i<fcount; i++)
		if (flist[i]==fld)
	 		return(bplist[(line*fcount)+i]);
	}
sprintf(cmsg,"g %d %d %d\n",myid,line,fld);
if (!sock_open) return("");
rtcpsend(cmsg); 
cp=rtcpreceive();
if (!cp) { gbuf[0]=0; return(gbuf); }
for(i=0; cp[i] && cp[i]>=' ' && i<sizeof(gbuf); i++) gbuf[i]=cp[i];
gbuf[i]=0;
//printf("rmt: get (%d) %d %d l=%d <%s>\n",myid,line,fld,i,gbuf);
return(gbuf);
}

/*<>*/
void remote::set(int myid,int line,int fld,char *value)
{
union { long ip; unsigned char cip[4]; } ipa;
char *lp,*cp;
int i,ipn;
if (fld == -1000)
	{
	lp=value;
	for(i=0; i<3; i++)
		{
		cp=strchr(lp,'.');
		if (!cp) break;
		*cp=0;
		ipn=atoi(lp);
		ipa.cip[i]=ipn;
		*cp='.';
		cp++;
		lp=cp;
		}
	ipn=atoi(lp);
	ipa.cip[3]=ipn;
	ipaddr=ipa.ip;
	rtcpclose();
	rtcpinit(2000);
	return;
	}
sprintf(cmsg,"s %d %d %d '%s'\n",myid,line,fld,value);
if (sock_open) rtcpsend(cmsg); 
//printf("rmt: set (%d) %d %d  '%s'\n",myid,line,fld,value);
}

/*<>*/
long remote::message(int myid,int msg,int num,oid parm[])
{
long rc;
int i,ip=0;
char *cp;
//printf("rmt: msg (%d) %d\n",myid,msg);
if (num)
	{
        ip = parm[0].getseg();
	if (parm[0].gettype()==OT_BINARY) 
		{
		cp = SM->sessget(-1,parm[0],0,0);
		sprintf(cmsg,"m %d %d '%s' \n",myid,msg,cp);
                }
	else
		sprintf(cmsg,"m %d %d %d\n",myid,msg,ip);
//printf("rmt: parm -> %s",cmsg);
        }
else
	sprintf(cmsg,"m %d %d\n",myid,msg);
if (!sock_open) return(0);
rtcpsend(cmsg); 
cp=rtcpreceive();
if (cp) rc=atoi(cp); else rc=0;
return(rc);
}

/*<>*/
int remote::bind(char *sym)
{
int rc,i;
char *cp;
if (!sock_open) return(-1); 
sprintf(cmsg,"b %s\n",sym);
rtcpsend(cmsg); 
cp=rtcpreceive();
for(i=0; cp[i]; i++) if (cp[i]<' ') cp[i]=0;
rc=atoi(cp);
//printf("rmt: bind <%s> %d\n",sym,rc);
return(rc);
}

/*<>*/
void remote::predraw(oid obj,int lines,int fld,int num)
{
static char fn[20];
int i,j;
char *cp;
if (!sock_open) return; 
if (fld == -2) { object = -1; return; }
if (fld == -1)
	{
//	object=tm.getrmtid(-1,obj);
	object = -1;
	if (object<0) return;
	sprintf(qmesg,"q %d %d %d ",object,lines,fcount);
	for(i=0; i<fcount; i++)
		{
		sprintf(fn,"%d ",flist[i]);
		strcat(qmesg,fn);
		}
//printf("rmt: sending <%s>\n",qmesg);
	strcat(qmesg,"\n");
	rtcpsend(qmesg); 
	cp=rtcpreceive();
	if (!cp) return; 
	for(i=0; i<sizeof(buffer); i++) buffer[i]=cp[i];
	j=0;
	for(i=0; i<(lines*fcount); i++)
		{
//		printf("<%s>",&buffer[j+1]);
		bplist[i] = &buffer[j+1];
		j+=2+buffer[j];
		}
	return;
	}
if (fld<MAX_FIELDS) 
	{
	flist[fld]=num;
	fcount=fld;
	}
else
	rprmsg("field count exceeded");
}

void rprmsg(char *m)
{
time_t t;
struct tm *tp;
time(&t);
tp=localtime(&t);
printf("rmt: %02d.%02d.%02d %02d:%02d:%02d %s\n",
	tp->tm_year,
	tp->tm_mon+1,
	tp->tm_mday,
	tp->tm_hour,
	tp->tm_min,
	tp->tm_sec,
	m);
}

void rtcpinit(int port) 
{ 
int rc;
struct hostent *hp;
sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
sa.sin_family = AF_INET; 
sa.sin_addr.s_addr = ipaddr;
sa.sin_port = htons(port);
sock_open=0;
rc=connect(sd, (sockaddr *) &sa, sizeof sa);
if (rc < 0 ) 
	perror("Remote");
else
	{
	sprintf(buf,"Connected to port %d",port);
	rprmsg(buf);
	sock_open=1;
	}
}

void rtcpclose(void)
{
close(sd);
}

char *rtcpreceive(void) 
{
int len;
len=read(sd, mesg, sizeof(mesg));
mesg[len]=0;
if (len==0) 
	{ 
	rprmsg("Connection dropped");
	return(0); 
	} 
return(mesg); 
}

void rtcpsend(char *m) 
{
write(sd, m, strlen(m));
}

