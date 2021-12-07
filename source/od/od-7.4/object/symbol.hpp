class symbol
	{
	short	size;
	oid	id;	
	oid	cid;	
	char	name[14];		
public:
	void	init(void);
        void	setname(char *aname);
	char 	*getname(void);
	void 	setid(oid nid);
	oid 	getid(void);
	void 	setcid(oid nid);
	void 	setsize(int sz);
	int 	getsize(void);
	oid 	getcid(void);
	int 	test(char *aname);
		symbol(void);
        };

