class hpgener
	{
	char 	name[40];	// current saved name 
	char 	type[40];	// current saved type 
	char 	cname[40];	// current classname
	char 	compname[40];	// compiler file name 
	int	notype;		// no type supplied 
	int	pub;		// public definitions
	int	genlevl;	// generate level
	int	metgen;		// method gen switch
	int	nummet;		// number of methods
	int	numattr;	// number of attribs
public:
	void 	init(void);
	void	generate(int code,char *token,int line,int type);
	void 	setname(char *nm);
	void 	setgen(int gn);
		hpgener(void);
        };
