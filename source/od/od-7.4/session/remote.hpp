class	remote 
	{
public:
	void	init(void);
	char	*receive(void);
	void	send(char *m);
	int	bind(char *sym);
	char	*check(void);

	void	predraw(oid obj,int line,int fld,int num);
	int 	size(void);
	char 	*get(int myid,int line,int fld);
	void 	set(int myid,int line,int fld,char *value);
	long	message(int myid,int msg,int num,oid parm[]);
		remote(void);
		~remote(void);
	};
