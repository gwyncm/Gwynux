#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "oid.hpp"
#include "sessmgr.hpp"
#include "htmldoc.hpp"

static char line[100];
static char *chmonth[12] =
	{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
	};


void htmldoc::append(char *fname,FILE *fp)
{
char *rc;
FILE *ap;
if (fname[0])
	{
	ap=fopen(fname,"r");
	if (ap)
		{
		rc=fgets(line,100,ap);
		while(rc)
			{
			fprintf(fp,"%s",line);
			rc=fgets(line,100,ap);
			}
		fclose(ap);
		}
	}
}

char *htmldoc::getdate(char f)
{
int year,month,day;
struct tm *t;
time_t ltime;
static char dstring[20];
time(&ltime);
t = localtime(&ltime);
year=t->tm_year+1900;
month=t->tm_mon;
day=t->tm_mday;
if (f=='y')
	sprintf(dstring,"%d",year);
else
	sprintf(dstring,"%s %d, %d",chmonth[month],day,year);
return(dstring);
}

/*<>*/
htmldoc::htmldoc(void)
{
// do not reinitialize
}

/*<>*/
void htmldoc::init(void)
{
doctitle[0]=0;
subtitle[0]=0;
banner[0]=0;
background[0]=0;
body[0]=0;
header[0]=0;
link[0]=0;
image[0]=0;
strcpy(company,"Toadware Technologies Inc");
strcpy(bgcolor,"FFFFFFFF");
strcpy(txcolor,"00000000");
strcpy(lkcolor,"000000FF");
strcpy(vlcolor,"00000099");
strcpy(trcolor,"00000290");
ruler=1;
}

/*<>*/
int htmldoc::size(void)
{
return(sizeof(htmldoc));
}

/*<>*/
char *htmldoc::get(long line,int fld)
{
return("");
}

/*<>*/
int htmldoc::set(long line,int fld,char *value)
{
return(0);
}

/*<>*/
void htmldoc::setbgcolor(char *cp)
{
strncpy(bgcolor,cp,sizeof(bgcolor));
}

/*<>*/
void htmldoc::settxcolor(char *cp)
{
strncpy(txcolor,cp,sizeof(txcolor));
}

/*<>*/
void htmldoc::setvlcolor(char *cp)
{
strncpy(vlcolor,cp,sizeof(vlcolor));
}

/*<>*/
void htmldoc::setlkcolor(char *cp)
{
strncpy(lkcolor,cp,sizeof(lkcolor));
}

/*<>*/
void htmldoc::settrcolor(char *cp)
{
strncpy(trcolor,cp,sizeof(trcolor));
}

/*<>*/
void htmldoc::setcompany(char *cn)
{
strncpy(company,cn,sizeof(company));
}

/*<>*/
void htmldoc::setimage(char *iname)
{
strncpy(image,iname,sizeof(image));
}

/*<>*/
void htmldoc::setdoctitle(char *dtitle)
{
strncpy(doctitle,dtitle,sizeof(doctitle));
}

/*<>*/
void htmldoc::setsubtitle(char *stitle)
{
strncpy(subtitle,stitle,sizeof(subtitle));
}

/*<>*/
void htmldoc::setbanner(char *bname)
{
strncpy(banner,bname,sizeof(banner));
}

/*<>*/
void htmldoc::setbackground(char *bname)
{
strncpy(background,bname,sizeof(background));
}

/*<>*/
void htmldoc::setbody(char *fname)
{
strncpy(body,fname,sizeof(body));
}

/*<>*/
void htmldoc::setheader(char *fname)
{
strncpy(header,fname,sizeof(header));
}

/*<>*/
void htmldoc::setlink(char *fname)
{
strncpy(link,fname,sizeof(link));
}

/*<>*/
void htmldoc::setruler(int rs)
{
ruler=rs;
}

/*<>*/
void htmldoc::generate(char *fn)
{
FILE *fp;
if (fn[0])
	fp=fopen(fn,"w");
else
	fp=stdout;
if (!fp) return;
fprintf(fp,"<!DOCTYPE HSML PUBLIC \"-//W3C//DTD HSML 3.2//EN\">\n");
fprintf(fp,"<HSML>\n");
fprintf(fp,"<!-- This file generated using Object Director htmlgen. -->\n");
// generate the header
fprintf(fp,"<HEAD>\n");
fprintf(fp,"<META NAME=\"GENERATOR\" CONTENT=\"Object Director S4\">\n");
append(header,fp);
if (doctitle[0]) fprintf(fp,"<TITLE>%s</TITLE>\n",doctitle);
fprintf(fp,"</HEAD>\n");
// generate the body
fprintf(fp,"<BODY BGCOLOR=\"#%s\" ",bgcolor);
if (background[0]) fprintf(fp,"BACKGROUND=\"%s\" ",background);
fprintf(fp,"TEXT=\"#%s\" LINK=\"#%s\" VLINK=\"#%s\">\n",
	txcolor,lkcolor,vlcolor);
if (banner[0])
	fprintf(fp,"<IMG src=\"%s\" alt=\"banner\" border=\"0\"><BR>\n",banner);
fprintf(fp,"<span style=\"width: 60px\"></span>\n");
fprintf(fp,"<font face=\"Helvetica\">\n");
if (doctitle[0]) fprintf(fp,"<H3>%s</H3>\n",doctitle);
if (subtitle[0]) fprintf(fp,"<H2>%s</H2>\n",subtitle);
if (ruler) fprintf(fp,"<HR>\n");
// generate the link table
fprintf(fp,"<table border=\"0\" width=\"600\">\n");
fprintf(fp,"<tr> <td width=\"200\" colspan=\"5\"></td> </tr>\n");
fprintf(fp,"<tr> <td width=\"150\" valign=\"top\" align=\"left\">\n");
if (image[0]) fprintf(fp,"	<p>	<img src=\"%s\" border=\"0\">\n",image);
append(link,fp);
fprintf(fp,"</td>\n");
fprintf(fp,"<td width=\"550\" colspan=\"3\"><h2 align=\"left\">\n");
// include the body text
append(body,fp);
// generate the trailor
fprintf(fp,"<HR>\n");
fprintf(fp,"<STRONG><FONT COLOR=\"#%s\" SIZE=\"-1\">\n",trcolor);
fprintf(fp,"Last modified: %s <BR>\n",getdate('a'));
fprintf(fp,"Copyright &#169 %s ",getdate('y'));
fprintf(fp,"%s.</STRONG></FONT>\n",company);
fprintf(fp,"</BODY>\n</HSML>\n");
if (fp!=stdout) fclose(fp);
}

/*<>*/
long htmldoc::message(int sn,int msg,int num,oid parm[])
/* table message entry point */
{
long rc;
int ip=0;
char *cp = "";
if (num)
	{
        ip = parm[0].getseg();
	cp = SM->sessget(sn,parm[0],0,0);
        }
rc=0;
switch(msg)
	{
case 0:
init();
break;

case 1:
//taskmsg();
break;

case 2:
setdoctitle(cp);
break;

case 3:
setsubtitle(cp);
break;

case 4:
setbanner(cp);
break;

case 5:
setbody(cp);
break;

case 6:
generate(cp);
break;

case 7:
setbackground(cp);
break;

case 8:
setheader(cp);
break;

case 9:
setlink(cp);
break;

case 10:
setimage(cp);
break;

case 11:
setbgcolor(cp);
break;

case 12:
settxcolor(cp);
break;

case 13:
setlkcolor(cp);
break;

case 14:
setvlcolor(cp);
break;

case 15:
settrcolor(cp);
break;

case 16:
setcompany(cp);
break;

case 17:
setruler(ip);
break;

default:
rc = -1;
break;
	}
return(rc);
}
