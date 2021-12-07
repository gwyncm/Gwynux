class stcomp 
	{
	int	number;		/* number of parse cases */
        int	level;		/* current parse level */
	int	curline;	/* current compile line */
	int	echo;		/* echo compilerd lines */
	char 	msg[100];	/* error message buffer */
public:
	void 	init(void);
	int 	topcompile(int tn,char *fbase);
	int 	token(int tn,char *tok,int type);
	long 	compline(int tn,char *line);
	int 	compile(int tn,char *name);
	void 	setecho(int ec);
	void 	help(void);
		stcomp(void);
	};
        

