class	socket
	{
	unsigned hostport;
public:
	void	init(int tn);
	char	*receive(int tn);
	void 	sethostport(int pn);

	char	*getmet(int mn);
	char	*getattr(int mn);
	int 	gethostport(void);
	char 	*get(long line,int fld);
	void 	set(long line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
	long	message(int msg,char *parm);
		socket(void);
		~socket(void);
	};
