class trgener
	{
	char	metname[20];	/* current method name */
	char 	name[20];	/* current saved name */
	char 	cname[20];	/* current saved classname */
	char 	type[20];	/* current saved type */
	char 	msg[100];	/* error message buffer */
	int	level;		/* nesting level */
	int	notype;		/* no type supplied */
        int	inswitch;	/* currently in switch */
        char	swval[20];	/* current switch value */
public:
	void 	init(void);
	void	generate(int code,char *token,int line);
		trgener(void);
        };
