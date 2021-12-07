class sharemgr
	{
	int	number;
        short	imgnum[30];
        short	tasknum[30];
	oid 	objlist[30];
	char	objname[30][20];
public:
	void	init(void);
	int	addobj(char *on,oid id,int img,int tn);
	void 	clearimage(int in);
	int	lookup(char *oname);
	char	*getname(int in);
	int 	gettask(int on);
	oid 	getobj(int on);

	char	*getmet(int mn);
	char	*getattr(int an);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
		sharemgr(void);
	};

#define SM_CLEARIMG	2
