class string 
	{
        short	len;
        short	max;
        char	data[1];
public:
	void	init(void);
	void	setmax(int mx);
        void	setlen(int ln);
        void	system(void);
        int	getlen(void);

	char	*getmet(int mn);
	char	*getattr(int mn);
        int	size(void);
	void 	set(int line,int fld,char *value);
	char 	*get(int line,int off);
	long	message(int msg,int num,oid parm[]);
		string(void);
        };

#define ST_SETMAX  	1
#define ST_GETLEN	5
