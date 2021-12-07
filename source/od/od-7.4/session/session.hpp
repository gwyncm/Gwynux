class session
	{
	int	curimg;
	int	status;
public:
	void	init(void);
	int	taskmsg(void);
	int	create_image(char *fname);
	int	open_image(char *fname);
	void 	setimgname(char *name);
	char 	*getimgname(void);
	int	close_image(void);

	void	setpriority(int py);
	int	getpriority(void);
	void	setobject(oid on);
	oid	getobject(void);
	int 	getimage(void);
	void 	setstatus(int st);
	int 	getstatus(void);

	int 	create_symbol(char *sname,oid cid,int size);
	void 	delete_symbol(int on);
	oid 	create_object(oid cid,int asize);
	oid 	create_object(char *cname,int asize);
	void 	delete_object(oid id);

	char 	*get(oid id,long line,int fld);
	void 	set(oid id,long line,int fld,char *value);

	void 	setparent(oid id,oid pid);
	oid 	getparent(oid id);
	oid 	getclass(oid id);
	oid 	getclass(int on);
	oid 	getid(int id);
	int 	lookup_symbol(char *oname);
	void 	setname(oid id,char *name);
	char 	*getname(oid id);
	char 	*getname(int id);
	void 	setid(int on,oid id);
	void 	*getaddress(oid id);

	void	setparent(oid cid,char *pn);

	char 	*sessget(oid id,int line,int fld);
	void 	sessset(oid id,int line,int fld,char *value);
	long 	sendmsg(oid id,int msg,int num,oid parm[]);

	void	showmap(void);
	};
