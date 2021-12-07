class object
	{
	short	ref;
	short	numvar;
	short	size;
	short	start;
	oid	myclass;
        oid	parent;	
	oid	ivar[];
public:
	void	init(void);
        void	set_ref(int re);
        int	get_ref(void);
        void	set_class(oid mycla);
	oid 	get_class(void);
	void 	set_parent(oid pp);
	oid 	get_parent(void);
	void 	set_size(int sz);
	int 	get_size(void);
        void	set_start(int st);
        int	get_start(void);
	void 	show(FILE *fp,int seg,int item);
		object(void);
        };

