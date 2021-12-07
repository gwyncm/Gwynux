#include "baseclass.hpp"
#include "attrib.hpp"
class	defclass : baseclass
	{
	short	basetype;			// baseclass type
        short	objsize;			// size of object
        short	nummet;				// number of methods
        short	numattr;			// number of attributes
	char	classname[16];			// class name
	char	parent[16];			// parent name
        attrib	attrlist[60];			// attribute/method list
public:        
	void	init(void);			// 0
        int     addmet(void);			// 1
	int 	addattr(void); 			// 2
	int 	lookupattr(char *aname);	// 3
        void	reset(void);			// 4
        int     lookupmet(char *mname);		// 5
	oid 	getattrname(int na);		// 6
	char 	*getattrtype(int na);		// 7
	oid 	getmetname(int na);		// 8
	char 	*getname(void);			// 9
	void 	setname(char *cname);		// 10
	int	getsize(void);			// 11
	int	getattrlen(int an);		// 12
	void 	setattrlen(int an,int len);	// 13

	int 	getoffset(int id);		// 14
	char 	*getdatatype(int id);		// 15
	int	setblockid(int mn,oid mid);	// 16
	int	getblockid(int msg);		// 17
	void 	setparent(char *pname);		// 18
	char 	*getparent(void);		// 19
	oid 	getattrref(int an);		// 20
	void 	setattrref(int an,oid rid);	// 21
	int 	delattr(int an);		// 22
	void	genclass(void);			// 23
	int	getnummet(void);		// 24
	void 	setbasetype(int ty);		// 25
	int 	getbasetype(void);		// 26
	void 	setsize(int size);		// 27

	void 	setmetname(int mn,char *mname);	// 28
	void 	setattrname(int an,char *aname);// 29

	char	*getmet(int mn);
	char	*getattr(int mn);
	int	size(void);
	char 	*get(int line,int fld);
	void 	set(int line,int fld,char *value);
	long	message(int sn,int msg,int num,oid parm[]);
        };

#define CM_ADDMET	1
#define CM_ADDATTR	2
#define CM_RESET	4
#define CM_GETATTRNAME	6
#define CM_GETMETNAME	8
#define CM_GETSIZE	11
#define CM_SETBLOCK	16
#define CM_GETBLOCK	17
#define CM_DELATTR	22
#define CM_GENCLASS	23
#define CM_GETNUMMET	24
#define CM_SETSIZE	27

#define CA_ATTRNAME	0
#define CA_DATATYPE	1
#define CA_LENGTH	2
#define CA_OFFSET	3
#define CA_METNAME	4
#define CA_PRECISION	5
#define CA_KEY		6
#define CA_FIELDTYPE	7
#define CA_NULL		8
#define CA_ACCESS	9
#define CA_NUMATTR	10
#define CA_ATTRREF	11
#define CA_PARENT	12
#define CA_OBJSIZE	13
#define CA_CLASSNAME	14
#define CA_BLOCKID	15
#define CA_BASETYPE	16
