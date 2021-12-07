class hpcomp 
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

	int 	token(char *tok,int type);
	long 	compline(char *line);
	void 	help(void);
public:
	void 	init(void);
	int 	topcompile(char *fbase);
	int 	compile(char *fname);
	void 	setecho(int ec);
	void 	setcomptok(int compt);
		hpcomp(void);
	};
        

