class syslog
	{
	char	filename[40];
	int	echo;
	int	errors;
	FILE	*fp;
        static  syslog* _instance;
protected:
		syslog(void);
public:	
	void	init(void);
	void 	set_echo(int ec);
	void	log_error(char *err);
	void	log_message(char *msg);
	int	get_errors(void);
	void	set_file(char *name);

	char	*getmet(int mn);
	char	*getattr(int an);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
	static  syslog* getinstance();
	};
#define SL syslog::getinstance()
#define SL_GETERRS	3
