class objmgr
	{
	oid	symlist;
	static	objmgr* _instance;
protected:
		objmgr(void);
public:
	void	init(void);
// image 
	int	create_image(char *fname);
	int	open_image(char *fname);
	int	close_image(int curimg);
	void 	setimgname(int curimg,char *name);
	char 	*getimgname(int curimg);
// symbol
	int 	create_symbol(int curimg,char *sname,oid cid,int size);
	void 	delete_symbol(int curimg,int on);
	int 	lookup_symbol(int curimg,char *oname);
	void 	setid(int curimg,int on,oid id);
	oid 	getid(int curimg,int on);
	char 	*getname(int curimg,int on);
	oid 	getclass(int curimg,int on);
// object
	oid 	create_object(int tn,char *cname,int asize);

	oid 	create_object(int curimg,oid cid,int asize);
	void 	delete_object(int curimg,oid id);
	oid 	getclass(int curimg,oid id);
	void 	setparent(int curimg,oid id,oid pid);
	oid 	getparent(int curimg,oid id);
	void 	setname(int curimg,oid on,char *name);
	char 	*getname(int curimg,oid id);
	void 	*getaddress(int curimg,oid id);

	char 	*taskget(int curimg,oid id,int line,int fld);
	void 	taskset(int curimg,oid id,int line,int fld,char *buf);
	long	sendmsg(int curimg,oid id,int msg,int num,oid parm[]);
	void 	showmap(int curimg);

	static 	objmgr* getinstance();
	};

#define OM objmgr::getinstance()
