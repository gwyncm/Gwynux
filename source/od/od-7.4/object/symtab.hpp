#include "symbol.hpp"
class	symtab
	{
	symbol name[250];
public:
	void 	init(void);
	int 	create_symbol(char *oname,oid cid,int size);
	void 	delete_symbol(int on);
	int 	lookup_symbol(char *oname);
	oid	getid(int on);
	void	setid(int on,oid id);
	oid	getcid(int on);
	int	getsize(int on);
	void 	show(char *iname);
	char 	*getname(oid on);
	char 	*getname(int on);
	void 	setname(oid on,char *name);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
	long	message(int msg,char *parm);
		symtab(void);
	};

