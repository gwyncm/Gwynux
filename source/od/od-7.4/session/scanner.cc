/*************************/
/* command scanner class */
/*************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "oid.hpp"
#include "scanner.hpp"

const char is_table[128] =	/* iinitial character class table */
	{
/*	 0 ^a ^b ^c ^d ^e ^f ^g ^h ^i ^j ^k ^l ^m ^n ^o 	*/
	 9, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 0, 0,

/*	^p ^q ^r ^s ^t ^u ^v ^w ^x ^y ^z ^[ ^\ ^] ^^ ^-		*/
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*	sp  !  "  #  $  %  &  '  (  )  *  +  ,  -  .  / 	*/
	 3, 1, 6, 1, 1, 1, 2, 7, 1, 1, 2, 9, 1, 9, 1, 2,

/*	 0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?		*/
	 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 2, 2, 2, 1,

/*	 @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O		*/
	 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,

/*	 P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _		*/
	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 2, 5,

/*	 `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o		*/
	 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,

/*	 p  q  r  s  t  u  v  w  x  y  z  {  |  }  -  .		*/
	 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 2, 1, 2, 0
	 };

static int scan_dlm(char str[],char dlm)
/* place a null over the dlm character in the string */
/* if theres a double dlm squeeze it to a single */
/* return the number of the last character processed */
{
int i,j;
j=0;
for(i=0; str[i]; i++)
	if (str[i]==dlm)
		{
		if (str[i+1]==dlm)
			{ str[j++]=dlm; i++; }
		else
			{ str[j]=0; return(i); }
		}
	else
		str[j++]=str[i];
str[j]=0;
return(i);
}

/**************************/
/* public command methods */
/**************************/

/*<>*/
scanner::scanner(void)
{
init();
}

/*<>*/
void scanner::init(void)
{
int i;
line[0]=0;
token[0]=0;
length=0;
offset=0;
tokoff=0;
type=0;
upper=0;
for (i=0; i<128; i++) s_table[i]=is_table[i];
}

/*<>*/
int scanner::size(void)
{
return(sizeof(scanner));
}

/*<>*/
void scanner::set_line(char *aline)
{
strncpy(line,aline,sizeof(line));
token[0]=0;
length=0;
offset=0;
tokoff=0;
type=0;
upper=0;
}

/*<>*/
void scanner::set_code(int code,int val)
{
if (code>=128 || code<0) return;
s_table[code]=val;
}

/*<>*/
int scanner::get_type(void)
{
return(type);
}

/*<>*/
char *scanner::get_offset(void)
/* return the current command offset */
{
return(&line[offset]);
}

/*<>*/
void scanner::set_upper(int up)
{
upper=up;
}

/*<>*/
char *scanner::get_next(void)
/* scan for next token in line and return offset */
{
char *tp;
int len,done;
done=len=0;
tp = &token[tokoff];
type = T_SYMBOL;
for(; line[offset] && !done; offset++)
	{
//	printf("sc: char=%c case=%d  len=%d off=%d <%s>\n",line[offset],
//		s_table[line[offset]],len,offset,line);
	switch(s_table[line[offset]])
		{
default:		/* invalid character */
break;

case 1:			/* delimiting token */
case 2:                 /* delimiting token */
if (len)	        /* token in progress */
	{
	done=1;
	offset--;
	}
else
	{
	tp[len++]=line[offset];
	if ((tokoff+len)>=sizeof(token)) return(0);
	type = T_DELIM;
        if (line[offset+1]=='>' ||
        	line[offset+1]=='=')
		{
		tp[len++]=line[++offset];
		if ((tokoff+len)>=sizeof(token)) return(0);
		}
	done=1;
        }
break;

case 3:			/* delimiter */
if (len) done=1;        /* token in progress */
break;

case 4:			/* lower */
case 5:			/* upper */
if (len && s_table[tp[len-1]]==9)
	{
	offset--;	
	done=1;
	break;
	}
if (upper && s_table[line[offset]]==4)
	tp[len++]=toupper(line[offset]);
else
	tp[len++]=line[offset];
if ((tokoff+len)>=sizeof(token)) return(0);
if (len==2 && tp[0]=='0' && tp[1]=='x') type=T_HEX;
if (type==T_INT) type=T_SYMBOL;
break;

case 6:			/* " quote */
case 7:			/* ' quote */
if (len)                /* token in progress */
	{
	if (line[offset]==line[offset+1])
		{
		tp[len++]=line[offset++];
		if ((tokoff+len)>=sizeof(token)) return(0);
		break;
		}
	}
else
	{
	type = T_CHAR; 	/* string */
	strncpy(tp,&line[offset+1],sizeof(token)-tokoff);
	len=scan_dlm(tp,line[offset]);
	offset+=len+1;
	len=strlen(tp);
	}
done=1;
break;


case 9:			/*  +-  */
if (len)
	{
	offset--;	
	done=1;
	break;
	}
case 8: 		/* number */
if (!len) type = T_INT;
tp[len++]=line[offset];
if ((tokoff+len)>=sizeof(token)) return(0);
if (line[offset]=='.' && type==T_INT) type=T_FLOAT;
break;
		}
	}
tp[len]=0;
if (done || len)
	{
	length=len;
        tokoff+=len+1;
	return(tp);
        }
return(0);
}

/*<>*/
int scanner::lookup(char *tok,char *table[],int tabsize,int offset,int mode)
/* lookup string in table return position */
/* allow partial matches */
/* return -1 not found -2 not unique */
/* tabsize	number of entries in table */
/* offset	offset of table entries */
/* mode		0=all table, 1=first, 2=exact */
{
int i,match;
char	*strptr,*tabptr;
match = -1;
if (0==tabsize) return(match);
if (!tok[0]) return(match);
for (i=0; i<tabsize; i++)
	{
	strptr=tok;
	tabptr=table[i*offset];
	if ((strptr==0) || (tabptr==0)) return(-1);
	while (*strptr==*tabptr && *strptr!='\0')
		{
		strptr++; tabptr++;
		}
	if (*tabptr==0) return(i);
	if (*strptr==0)
		{
		if (match != -1)
			return (-2);
		else
			{
			if (mode==1)
				return(i);
			else
				match=i;
			}
    		}
	}
	if (mode==2)
		{
		if (strlen(tok)==strlen(table[i]))
			return(match);
		else
			return(-2);
		}
	return match;
}

char *scanner::get(int line,int fld)
{
static char rt[2];
switch(fld)
	{
case 4:	
return(get_next());

case 5:	
rt[1]=0;
rt[0]=get_type();
return(rt);
	}
return("");
}

void scanner::set(int line,int fld,char *value)
{
char *cp;
int code,val;
switch(fld)
	{
case 3:	
set_line(value);
return;

case 7:
cp=strchr(value,':');
if (!cp) return;
code=atoi(value);
val=atoi(cp+1);
set_code(code,val);
return;
	}
}

long scanner::message(int msg,int num,oid parm[])
{
switch(msg)
	{
case 0:
init();
break;
	}
return(0);
}
