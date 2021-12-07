class sqlcomp 
	{
        int	level;		/* current parse level */
	int	curline;	/* current compile line */
	int	comptok;	/* show compiled tokens */
	int	compsw;		/* compile on/off */
	int	numdefs;	/* number of defines */
	int	echo;		/* echo compilerd lines */
	char 	msg[100];	/* error message buffer */
public:
	void 	init(void);
	int 	topcompile(char *fbase);
	int 	token(char *tok,int type);
	long 	compline(char *line);
	int 	compile(char *name);
	void 	setcomptok(int compt);
	void 	setecho(int ec);
	void 	help(void);
	void 	genprint(void);
		sqlcomp(void);
	};
        

