#include "baseclass.hpp"
class htmldoc : public baseclass
	{
	char	doctitle[200];			// document title
	char	subtitle[200];			// docsub title
	char	banner[200];			// banner
	char	background[200];		// background
	char	body[200];			// body
	char	header[200];			// header
	char	link[200];			// link
	char	image[200];			// image
	char	bgcolor[10];			// background colour
	char	txcolor[10];			// text colour
	char	lkcolor[10];			// link colour
	char	vlcolor[10];			// vlink colour
	char	trcolor[10];			// trailor colour
	char	company[40];			// company name
	int	ruler;				// ruler
public:
	void	init(void);
	void	taskmsg(void);			// 1  task message
	void	setdoctitle(char *dtitle);	// 2  setdoctitle
	void	setsubtitle(char *stitle);	// 3  setsubtitle
	void	setbanner(char *bname);		// 4  setbanner
	void	setbody(char *fname);		// 5  setbody
	void	generate(char *fname);		// 6  generate
	void	setbackground(char *fname);	// 7  setbackground
	void	setheader(char *fname);		// 8  setheader
	void	setlink(char *fname);		// 9  link
	void	setimage(char *iname);		// 10 image
	void	setbgcolor(char *cp);		// 11 background colour
	void	settxcolor(char *cp);		// 12 text colour
	void	setlkcolor(char *cp);		// 13 link colour
	void	setvlcolor(char *cp);		// 14 vlink colour
	void	settrcolor(char *cp);		// 15 trailor colour
	void	setcompany(char *cn);		// 16 company name
	void	setruler(int rs);		// 17 ruler

	char	*getmet(int mn);		// get method name
	char	*getattr(int mn);		// get attr name
	char	*getdate(char f);		// get current date
	void	append(char *fname,FILE *fp);	// append document
	char 	*get(long line,int fld);
	int 	set(long line,int fld,char *value);
	int	size(void);
	long	message(int sn,int msg,int num,oid parm[]);
		htmldoc(void);
        };
