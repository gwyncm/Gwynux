class stgener
	{
	char	metname[100];	/* current method name */
	char 	objname[100];	/* current object name */
	char 	parm1[100];	/* current parm1 */
	char 	parm2[100];	/* current parm2 */
	char 	target[100];	/* current target */
	char 	blkname[100];	/* current blkname */
	char 	msg[100];	/* error message buffer */
	int	type1;		/* parm 1 type */
	int	type2;		/* parm 2 type */
	int	level;		/* nesting level */
	int	msgid;		/* current message id */
	oid	objid;		/* current object id */
	oid	classid;	/* current class id */
public:
	void 	init(void);
	void	generate(int tn,int code,char *token,int line,int type);
	void	genparm(int tn,char *parm,char type);
	oid	genblock(int tn);
	void 	reset(void);
		stgener(void);
        };
