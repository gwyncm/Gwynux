class oid		// object id structure	
	{
public:
	 short	seg;
	 char	item;
unsigned char 	type;
public:
	 void	init(void);
	 int	asint(void);
	 char	*aschar(void);
	 char	*astype(void);
	 int	gettype(void);
	 int	getobj(void);
	 int	getseg(void);
	 int	getitem(void);
	 int	getmsg(void);
	 int	getcode(void);
	 void	setint(int in);
	 void	setobj(int in);
	 void	setseg(int in);
	 void	setmsg(int in);
	 void	setcode(int in);
	 void	settype(int in);
		oid(void);
	};


#define OT_BINARY	0	// undefined object
#define OT_SHINT	1	// short int
#define OT_BASEOBJ	2	// base object reference
#define OT_SYMBOL	3	// symbol only
#define OT_STRING	4	// string object
#define OT_CLASS	5	// class reference
#define OT_SHARE	6	// shared object
#define OT_BLOCK	7	// block reference
#define OT_INST		8	// instruction
