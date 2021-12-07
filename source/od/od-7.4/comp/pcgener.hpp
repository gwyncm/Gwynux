class pcgener
	{
        char	metname[40];	/* current method name */
	char 	name[40];	/* current saved name */
	char 	cname[40];	/* current saved classname */
	char 	type[40];	/* current saved type */
	char 	msg[100];	/* error message buffer */
	char 	opfunc[40];	/* operator function */
	char 	compname[40];	/* compiler file name */
	int	attrnum;	// attribute number */
	int 	osize;		/* object size */
	int	level;		/* nesting level */
	int	notype;		/* no type supplied */
	int	msgid;		/* current message id */
	oid	objid;		/* current object id */
	oid	classid;	/* current class id */
	int	priv;		/* attributes are private */
public:
	void 	init(void);
	void	generate(int tn,int code,char *token,int line,int type);
	void 	setname(char *nm);
	void	genparm(int tn,char *parm,char type);
	oid	genblock(int tn);
		pcgener(void);
        };
