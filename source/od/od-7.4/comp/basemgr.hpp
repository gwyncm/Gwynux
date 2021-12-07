class basemgr
	{
public:
	void	init(void);
	int 	gettype(char *atype);
	void 	setrmt(int type,int rid);
	int 	getrmt(int ty);
	int 	getsize(int type);
	char	*getname(int type);

	long	sendmsg(int type,int sn,char *obj,int msg,int num,oid parm[]);
	char 	*getobj(int type,char *obj,long item,int fld);
	void 	setobj(int type,char *obj,long item,int fld,char *value);
	char 	*get(int type,long item,int fld);
	void 	set(int type,long item,int fld,char *value);
	
		basemgr(void);
	};

#define CA_ATTRNAME	0
#define CA_DATATYPE	1
#define CA_LENGTH	2
#define CA_OFFSET	3
#define CA_METNAME	4
