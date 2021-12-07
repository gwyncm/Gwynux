#ifndef SM
#include "session.hpp"
class sessmgr 
	{
        session	sesslist[30];
        int	number;
        static  sessmgr* _instance;
protected:
                sessmgr(void);
public:
	void	init(void);			// 0

	char	*getmet(int mn);
	char	*getattr(int mn);
// image
	int	create_image(char *fname);	
	int	open_image(char *fname);	
	int	close_image(int sn);	
// symbol
	int	create_symbol(int sn,char *sname,oid cid,int size);
	int 	lookup_symbol(int sn,char *oname);
	void 	delete_symbol(int sn,int on);
	oid 	getclass(int sn,int on);
	char 	*getname(int sn,int id);
	void 	setid(int sn,int on,oid id);
	oid 	getid(int sn,int on);
// object
	oid 	create_object(int sn,oid cid,int asize);	
	oid 	create_object(int sn,char *cname,int asize);
	oid 	lookup_object(int sn,char *oname);
	void 	delete_object(int sn,oid id);
	oid 	create_class(int sn,char *cname);
	int 	lookupmet(int sn,oid cid,char *mname);
	oid 	getclass(int sn,oid id);
	void	setparent(int sn,oid id,oid pid);
	void	setparent(int sn,oid id,char *pname);
	oid	getparent(int sn,oid id);
	void 	setname(int sn,oid id,char *name);
	char 	*getname(int sn,oid id);
	void	share(int sn,char *on);
// session
	int	start(int sn);	
	void 	setstatus(int sn,int st);	

	char 	*sessget(int sn,oid id,int line,int fld);
	void 	sessset(int sn,oid id,int line,int fld,char *value);
	long 	sendmsg(int sn,oid id,int msg,int num,oid parm[]);
	void	showmap(int sn);			
	void 	*getaddress(int sn,oid id);

	int	size(void);
	char 	*get(long line,int fld);
	void 	set(long line,int fld,char *value);
	long	message(int sn,int msg,int num,oid parm[]);
	static  sessmgr* getinstance();
	};
#define SM sessmgr::getinstance()
#endif
