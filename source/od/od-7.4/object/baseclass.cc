#include "oid.hpp"
#include "baseclass.hpp"

/*<>*/
void baseclass::init(void)
/* initialize a class */
{
}

/*<>*/
int baseclass::size(void)
{
return(sizeof(baseclass));
}


/*<>*/
char	*getmet(int mn)
/* get a method name */
{
return("");
}

/*<>*/
char	*getattr(int mn)
/* get an attribute name */
{
return("");
}

/*<>*/
char	*get(int line,int fld)
/* get an attribute value */
{
return("");
}

/*<>*/
void 	set(int type,long item,int fld,char *value)
/* set an attribute value */
{
return;
}

/*<>*/
long baseclass::message(int msg,int num,oid parm[])
/* send a message to a class */
{
return(0);
}
