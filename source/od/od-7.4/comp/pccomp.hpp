class pccomp 
	{
	int	number;		// number of parse cases 
        int	level;		// current parse level 
	int	curline;	// current compile line 
	int	comptok;	// show compiled tokens 
	int	compsw;		// compile on/off 
	int	numdefs;	// number of defines 
        int	paren;		// inside parens 
	int	echo;		// echo compiled lines 
	char 	msg[100];	// error message buffer 

	int 	token(int tn,char *tok,int type);
	long 	compline(int tn,char *line);
	void 	help(void);
public:
	void 	init(void);
	int 	compile(int tn,char *name);
	int 	topcompile(int tn,char *fbase);
	void 	setecho(int ec);
	void 	setcomptok(int compt);
		pccomp(void);
	};
        

