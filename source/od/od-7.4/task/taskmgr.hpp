#ifndef TM
#include "task.hpp"
class taskmgr 
	{
        task	tasklist[30];
	int	active;
        int	number;
        int	waittime;
	int	curtask;
        static  taskmgr* _instance;
protected:
                taskmgr(void);
public:
	void	init(void);			// 0
	int 	taskmsg(void);			// 1
	void	exit(void);			// 2
	int	start(int tn,char *oname);	// 3
	void	exec(int tn,char *oname);	// 4
	int	open_image(char *fname);	// 5

	char	*getmet(int mn);
	char	*getattr(int mn);
// image
	int	create_image(char *fname);	
	int	close_image(void);	
// symbol
	int	create_symbol(int tn,char *sname,oid cid,int size);
	int 	lookup_symbol(int tn,char *oname);
	void 	delete_symbol(int tn,int on);
	oid 	getclass(int tn,int on);
	char 	*getname(int tn,int id);
	void 	setid(int tn,int on,oid id);
	oid 	getid(int tn,int on);
// object
	oid 	create_object(int tn,oid cid,int asize);	
	oid 	create_object(int tn,char *cname,int asize);
	oid 	lookup_object(int tn,char *oname);
	void 	delete_object(int tn,oid id);
	oid 	create_class(int tn,char *cname);
	int 	lookupmet(int tn,oid cid,char *mname);
	oid 	getclass(int tn,oid id);
	void	setparent(int tn,oid id,char *pname);
	oid	getparent(int tn,oid id);
	void 	setname(int tn,oid id,char *name);
	char 	*getname(int tn,oid id);
	void	share(char *on);
// task
	void 	setstatus(int tn,int st);	
	void 	setpriority(int tn,int py);	
	void	setobject(int tn,oid id);
	oid	getobject(int tn);
    	int	getsession(int tn);		
	void	setactive(int tn);
    	int	getactive(void);		
	void	stop(void);			

	char 	*taskget(int tn,oid id,int line,int fld);
	void 	taskset(int tn,oid id,int line,int fld,char *value);
	long 	sendmsg(int tn,oid id,int msg,int num,oid parm[]);
	void	showmap(void);			
	void 	*getaddress(int tn,oid id);
	long 	exec_block(int tn,oid bid,int num,oid parm[]);

	int	size(void);
	char 	*get(long line,int fld);
	void 	set(long line,int fld,char *value);
	long	message(int sn,int msg,int num,oid parm[]);
	static  taskmgr* getinstance();
	};
#define TM taskmgr::getinstance()
#endif
