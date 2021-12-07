class trcomp 
	{
	int	number;		/* number of parse cases */
        int	level;		/* current parse level */
	int	curline;	/* current compile line */
        int	nest;		/* current nest level */
	int	echo;		/* echo compilerd lines */
	char 	msg[100];	/* error message buffer */
public:
	void 	init(void);
	int 	token(char *tok,int type);
	long 	compline(char *line);
	int 	compile(char *name);
	int 	topcompile(char *fbase);
	void 	setecho(int ec);
	void 	help(void);
		trcomp(void);
	};
        

