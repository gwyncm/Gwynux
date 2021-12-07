class attrib
	{
	char 	name[16];
	oid	reference;
        short 	length;	
        short 	offset;	
	char	datatype;
	char	precision;
	char	fieldtype;
	char	access;
	char	null;
	char	key;
public:
	void 	init(void);

	void	setdatatype(char *atype);
	char 	*getdatatype(void);
	void	setlength(int num);
	int 	getlength(void);
	void	setoffset(int off);
	int 	getoffset(void);
	char 	*getname(void);
	void 	setname(char *nm);
	void	setprecision(int num);
	int 	getprecision(void);
	void	setfieldtype(char *atype);
	char 	getfieldtype(void);
	void	setnull(char *atype);
	char 	getnull(void);
	void	setaccess(char *atype);
	char 	getaccess(void);
	void	setkey(int num);
	int 	getkey(void);
	oid 	getref(void);
	void 	setref(oid aid);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
		attrib(void);
	};

