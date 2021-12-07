class scanner
	{
        int	length;			// 0 token length 
        int	offset;			// 1 current line offset 
        int	tokoff;			// 2 current token offset 
        char	line[400];		// 3 line buffer 
        char	token[400];		// 4 token buffer 
        char	type;			// 5 token type 
        char	upper;			// 6 translate to upper 
	char	s_table[128];		// 7 character class table 
public:
	void	init(void);
	void 	set_line(char *aline);	
	int 	get_type(void);
	char	*get_offset(void);
	void 	set_upper(int up);
	char 	*get_next(void);
	int 	lookup(char *tok,char *table[],int tabsize,int offset,int mode);
	void	set_code(int code,int val);
	char	*getmet(int mn);
	char	*getattr(int an);

	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int msg,int num,oid parm[]);
		scanner(void);
        };

#define T_SYMBOL	'S'
#define T_DELIM		'D'
#define T_CHAR		'C'
#define T_FLOAT		'F'
#define T_INT		'I'
#define T_HEX		'H'

#define SA_LINE		3
#define SA_TOKEN	4
#define SA_TYPE		5
#define SA_CODE		7

#define MAX_LINESIZE	400
#define MAX_TOKENSIZE	400


