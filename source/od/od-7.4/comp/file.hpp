#ifndef EFILE
#define EFILE
class file
	{
	short	isopen;			
	short  	rsize;			
	short  	fileid;			
	short 	empty;			
	char 	filename[50];	
	char   	buffer[2000];		
public:        
	void	init(void);
	int	create(void);
	int 	open(void);
	void 	close(void);
	void 	setname(char *name);
        void	setsize(int size);
	long 	first(void);
	long 	last(void);
	long 	next(void);
	long 	previous(void);
	long 	insert(void);
	long 	update(void);
	long 	current(void);
	void 	remove(void);
	void 	clear(void);
	void 	get(char *to);
	void 	set(char *from);
	char 	*getaddress(void);
	long 	getid(void);
	int 	setid(long id);
	char 	*getname(void);
	int 	write(char *buf,int num);
	int 	read(char *buf,int num);
	void	rename(char *fname);
		file(void);
	};
#endif
