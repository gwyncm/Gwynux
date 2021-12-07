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
#include "scanner.hpp"		// for message codes
#include "string.hpp"		// for message codes
#include "sessmgr.hpp"
#include "socket.hpp"

#define MAXMESG 3000
#define MONITOR 

#define MAX_FIELDS 100
static int  flist[MAX_FIELDS];
static int block=0;
static int cd = 0;
static char mesg[MAXMESG];
static char cmesg[MAXMESG];
static char repl[MAXMESG];
static int sd,clen,len;
static struct sockaddr_in sa;
static struct sockaddr sa2;
static char buf[100];
static oid strid,intid,scanid;
#define DEFPERM 0644

extern "C" void tcpinit(int port); 
extern "C" void tcpclose(void);

void socket::init(int tn) 
{ 
oid num;
//tcpinit(htons(3000));
scanid=SM->create_object(tn,"scanner",0);
strid=SM->create_object(tn,"string",100);
num=SM->create_object(tn,"shint",100);
//SM->sendmsg(tn,strid,ST_SESMAX,1,&num);
}

socket::socket(void) 
{ 
// do not reinitialize
}

socket::~socket(void)
{
tcpclose();
}

int socket::gethostport(void)
{
return(hostport);
}

void socket::sethostport(int pn)
{
hostport=pn;
}

void prmsg(char *m)
{
time_t t;
struct tm *tp;
time(&t);
tp=localtime(&t);
printf("sock: %02d.%02d.%02d %02d:%02d:%02d %s\n",
	tp->tm_year,
	tp->tm_mon+1,
	tp->tm_mday,
	tp->tm_hour,
	tp->tm_min,
	tp->tm_sec,
	m);
}

void tcpinit(int port) 
{ 
int rc,slen;
sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
bzero((char *) &sa, sizeof(sa));
sa.sin_family = AF_INET; 
sa.sin_addr.s_addr = INADDR_ANY;
sa.sin_port = port;
slen = sizeof(sa);
rc=bind(sd, (sockaddr *) &sa, slen);
if (rc < 0 ) perror("Bind");
sprintf(buf,"Listening on port %d\n",ntohs(port));
prmsg(buf);
if (!block)
	{
	rc=fcntl(sd,F_SETFL,FNDELAY);
	if (rc < 0 ) printf("Unblock socket failed");
	}
listen(sd,5);
//SM->setfd(sd);
}

void tcpclose(void)
{
if (cd) close(cd);
cd=0;
}

char *socket::receive(int tn) 
{
char *tp,*vp,*cp;
int cr,i,j,k,line,fld,obj,msg,rc,num;
socklen_t slen;
slen = sizeof(sa2);
if (cd<1) 
	{
	cd = accept(sd, &sa2, &slen);
	if (cd>0)
		{
		sprintf(buf,"Connection accepted");
		prmsg(buf);
		if (!block)
			{
			rc=fcntl(cd,F_SETFL,FNDELAY);
			if (rc < 0 ) printf("Unblock socket failed");
			}
//		SM->setfd(cd);
		len=0;
		}
	}
else
	{ 
	clen = -1;
	clen=read(cd, cmesg, sizeof(cmesg));
	if (clen<0) return(0);
	cr=0;
	for (i=0; i<clen; i++) 
		{ 
		mesg[len+i]=cmesg[i]; 
		if (cmesg[i]=='\n') cr++;
		}
	len+=clen;
	mesg[len]=0;
//if (mesg[0]!='p') printf("mesg=<%s>(%d)\n",mesg,len);
	if (len==0) 
		{ 
		prmsg("Connection dropped");
//		SM->clrfd(cd);
		close(cd); 
	//	cnet.save("network.dat");
		cd=0; 
		} 
	else
		{
		if (!cr) return(0);
		len=0;
		SM->sessset(tn,scanid,0,SA_LINE,mesg);
		tp=SM->sessget(tn,scanid,0,SA_TOKEN);
		if (tp) switch(tp[0])
			{
	case 'b':	// bind
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="UNKNOWN";	
	sprintf(repl,"%d\n",SM->lookup_symbol(tn,tp));
	write(cd, repl, strlen(repl));
	break;

	case 'e':	// exit
	prmsg("Connection closed by request");
//	SM->clrfd(cd);
	close(cd);
	cd=0;
	break;

	case 's':	// set
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	obj=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	line=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	fld=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="";
	SM->sessset(tn,SM->getid(tn,obj),line,fld,tp);
	break;

	case 'q':	// quick get
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	obj=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	line=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	fld=atoi(tp);
	for(i=0; i<fld; i++)
		{
		tp=SM->sessget(tn,scanid,0,SA_TOKEN);
		if (!tp) tp="0";
		flist[i]=atoi(tp);
		}
	k=0;
//printf("q request obj=%d lines=%d flds=%d\n",obj,line,fld);
	for(i=0; i<line; i++)
		for(j=0; j<fld; j++)
			{
			vp=SM->sessget(tn,SM->getid(tn,obj),i,flist[j]);
			repl[k]=strlen(vp);	
			if ((k+repl[k])<MAXMESG)
				{
				strcpy(&repl[k+1],vp);
				k+=1+repl[k];	
				repl[k++]=0;
				}
			}
	repl[k++]=0;
//printf("sending %d byte reply\n",k);
	write(cd, repl, k);
	break;

	case 'g':	// get
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	obj=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	line=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	fld=atoi(tp);
	vp=SM->sessget(tn,SM->getid(tn,obj),line,fld);
//	printf("get: %d %d %d <%s>\n",obj,line,fld,vp);
	strncpy(repl,vp,sizeof(repl));
	strcat(repl,"\n");
	write(cd, repl, strlen(repl));
	break;

	case 'm':	// msg
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	obj=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) tp="0";
	msg=atoi(tp);
	tp=SM->sessget(tn,scanid,0,SA_TOKEN);
	if (!tp) 
		rc=SM->sendmsg(tn,SM->getid(tn,obj),msg,0,0);
	else
		{
		if (strcmp(SM->sessget(tn,scanid,0,SA_TYPE),"I"))
			{
			intid.setseg(atoi(tp));
			intid.settype(OT_SHINT);
			rc=SM->sendmsg(tn,SM->getid(tn,obj),msg,1,&intid);
			}
		else
			{
			SM->sessset(tn,strid,0,0,tp);
			rc=SM->sendmsg(tn,SM->getid(tn,obj),msg,1,&strid);
			}
		}
//	printf("msg: %d %d <%s>\n",obj,msg,tp);
	sprintf(repl,"%d\n",rc);
	write(cd, repl, strlen(repl));
	break;

	case 'r':	// cgi read
	sprintf(repl,"/tmp/response.html",cp);
	write(cd, repl, strlen(repl));
	break;

//	case 'p':	// poll load
//	vp=SM->getload();
//	sprintf(repl,"%s\n",vp);
//	write(cd, repl, strlen(repl));
//	break;

	default:	
	sprintf(repl,"Invalid request %s",tp);
	prmsg(repl);
	sprintf(repl,"Invalid request %s\n",tp);
	write(cd, repl, strlen(repl));
	break;
			}
//if (mesg[0]!='p') printf("repl=<%s>(%d)\n",repl,strlen(repl));
		}
	}
return(0); 
}

