#include "image.hpp"

class imgmgr
	{
	image	*img[20];
	static	imgmgr* _instance;
protected:
		imgmgr(void);
public:
	void	init(void);
// image 
	int	create_image(char *fname);
	int	open_image(char *fname);
	int	close_image(int curimg);
	void 	setimgname(int curimg,char *name);
	char 	*getimgname(int curimg);
	void 	showmap(int curimg);
// object
	oid 	create_object(int curimg,oid cid,int asize);
	void 	delete_object(int curimg,oid id);
	void 	setclass(int curimg,oid id,oid cid);
	oid 	getclass(int curimg,oid id);
	void 	setparent(int curimg,oid id,oid pid);
	oid 	getparent(int curimg,oid id);
	void 	*getaddress(int curimg,oid id);

	static 	imgmgr* getinstance();
	};

#define IM imgmgr::getinstance()
