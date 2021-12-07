#include "segment.hpp"

class	image
	{
	short 	numseg;		// number of segments in image
	int	fileid;		// image file id
	segment	*seg[250];	// segment table
        char 	*name;		// image file name
public:
	void 	init(void);
// image
	int 	create_image(char *fname);
	int 	open_image(char *fname);
	int 	close_image(void);
	void 	setimgname(char *iname);
	char 	*getimgname(void);
	int	getnumseg(void);
	void 	showmap(void);
// object
	oid 	create_object(oid cid,int size);
	void 	delete_object(oid id);
	void 	setclass(oid id,oid cid);
	oid	getclass(oid id);
	void	setparent(oid id,oid pid);
	oid	getparent(oid id);
	void	*getaddress(oid id);

		image(void);
		~image(void);
	};

