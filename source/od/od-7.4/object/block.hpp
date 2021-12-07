class block 
	{
	short 	number;			// number of frames
	short 	curline;		// current line
	oid 	blist[1000];		// instruction list
public:
	void	init(void);		// 0
	void	additem(void);		// 1
	void	delitem(void);		// 2
	int	getnumber(void); 	// 3
	int 	getins(int in);		// 4
	oid 	getid(int in);		// 5
	int 	getnparm(int in); 	// 6
	int 	getmsg(int in);		// 7
	oid 	*getparm(int in); 	// 8

	char 	*getmet(int mn);
	char 	*getattr(int mn);
	void	genmsg(int tn,int objnum,int msg);
	void 	gencode(int type);
	void 	setcode(int in,int type);
	void	genparm(oid sid);
	char 	*getinstype(int in);
	int 	getinstype(char *in);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
		block(void);
	};

#define	BLOCK_ADDITEM	1
#define	BLOCK_DELITEM	2
#define BLOCK_GETNUMBER	3
#define BLOCK_GETINST	4
#define BLOCK_GETID	5
#define BLOCK_GETNPARM	6
#define BLOCK_GETMSG	7
#define BLOCK_GETPARM	8


#define	PARAMETER	0
#define	SENDMSG		1
#define IFTRUE		2
#define IFFALSE		3
#define REPEAT		4
#define RETURN		5
#define WHILETRUE	6	
#define WHILEFALSE	7
#define STARTBLK	8
#define ENDBLOCK	9
