#ifndef BASECLASS
#define BASECLASS
class	baseclass 
	{
public:        
	void	init(void);
	char	*getmet(int mn);			
	char	*getattr(int mn);
	int	size(void);			
	char	*get(int line,int fld);			
	void 	set(int type,long item,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
        };

#endif 
