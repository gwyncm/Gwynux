#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oid.hpp"
#include "imgmgr.hpp"

#define MAX_IMG	20

imgmgr* imgmgr::_instance=0;

imgmgr* imgmgr::getinstance()
{
if (_instance == 0) _instance = new imgmgr;
return (_instance);
}

void imgmgr::init(void)
{
}

imgmgr::imgmgr(void)
{
int i;
for(i=0; i<MAX_IMG; i++) img[i]=0;
} 

/******************** image functions ****************/

/*<>*/
int imgmgr::create_image(char *fname)
{
int i;
for(i=0; i<MAX_IMG; i++)
	if (img[i] &&
		!strcmp(img[i]->getimgname(),fname))
			return(0);
for(i=0; i<MAX_IMG; i++)
	if (!img[i])
        	{
		img[i] = new image;
//printf("IM: create image address=%p image=%d\n",img[i],i);
		if (!img[i]) return(-1);
		img[i]->setimgname(fname);
		img[i]->create_image(fname);
                return(i);
                }
return(-1);
}

int imgmgr::open_image(char *fname)
{
int i,rc;
for(i=0; i<MAX_IMG; i++)
	if (img[i] &&
		!strcmp(img[i]->getimgname(),fname))
			return(0);
for(i=0; i<MAX_IMG; i++)
	if (!img[i])
        	{
		img[i] = new image;
		if (!img[i]) return(-1);
		rc=img[i]->open_image(fname);
                if (rc<0) return(rc); 
                return(i);
                }
return(-1);
}

int imgmgr::close_image(int curimg)
{
int rc;
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return(0);
rc=img[curimg]->close_image();
delete img[curimg];
img[curimg]=0;
}

void imgmgr::showmap(int curimg)
{
int i;
for (i=0; i<MAX_IMG; i++)
	if (img[i]) img[i]->showmap();
}

char *imgmgr::getimgname(int curimg)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return((char *)"");
return(img[curimg]->getimgname());
}

void imgmgr::setimgname(int curimg,char *name)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return;
img[curimg]->setimgname(name);
}

void *imgmgr::getaddress(int curimg,oid id)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return(0);
return(img[curimg]->getaddress(id));
}

void imgmgr::setparent(int curimg,oid id,oid pid)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return;
return(img[curimg]->setparent(id,pid));
}

void imgmgr::setclass(int curimg,oid id,oid cid)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return;
return(img[curimg]->setclass(id,cid));
}

/**************  object functions *******************/

/*<>*/
oid imgmgr::create_object(int curimg,oid cid,int asize)
{
oid empty;
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return(empty);
//printf("IM: creating object\n");
return(img[curimg]->create_object(cid,asize));
}

/*<>*/
void imgmgr::delete_object(int curimg,oid id)
{
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return;
img[curimg]->delete_object(id);
}

/*<>*/
oid imgmgr::getparent(int curimg,oid id)
{
oid empty;
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return(empty);
return(img[curimg]->getparent(id));
}

/*<>*/
oid imgmgr::getclass(int curimg,oid id)
{
oid empty;
if (curimg<0 || curimg>=MAX_IMG || !img[curimg]) return(empty);
return(img[curimg]->getclass(id));
}

