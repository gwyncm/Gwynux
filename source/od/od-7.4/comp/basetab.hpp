class basetab
	{
	int offset;
public:
	void	init(void);
	int 	load(char *fname);
	int 	save(char *fname);
	void 	build(char *bn);
        void	first(void);
        void	next(void);
        void	prev(void);
        void	write(char *fn);
        void	read(char *fn);

	char	*getmet(int mn);
	char	*getattr(int mn);
	int 	size(void);
        char	*get(long line,int fld);
	void	set(long line,int fld,char *value);
	long	message(int sn,int msg,int num,oid parm[]);
		basetab(void);
	};
