#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFPERM 0644

#include "oid.hpp"
#include "image.hpp"

#define MAX_SEGS	250

// image initialization

/*<>*/
image::image(void)
/* initialize image (initial) */
{
init();
}

/*<>*/
image::~image(void)
/* image delete */
{
free(name);
name=0;
}

/*<>*/
void image::init(void)
/* initialize image (reuseable) */
{
int i;
numseg=0;		// segs in image
name=0;			// image name
fileid = -1;		// current fileid
for(i=0; i<250; i++) seg[i]=0;
}

/**********************   image mechanics  **************/

/*<>*/
int image::create_image(char *fname)
{
int i;
oid id;
char fn[100];
init();		// reinitalize image
for(i=strlen(fname)-1; i>0 && fname[i]!='/'; i--);
if (fname[i]=='/') i++;
setimgname(&fname[i]);
if (!name) return(-1);
sprintf(fn,"%s.img",fname);
fileid=creat(fn,DEFPERM);
close(fileid);
fileid=open(fn,O_RDWR);
if (fileid<0) return(fileid);
return(0);
}

/*<>*/
int image::open_image(char *fname)
{
int rc,i;
char fn[100];
init();		// reinitalize image
for(i=strlen(fname)-1; i>0 && fname[i]!='/'; i--);
if (fname[i]=='/') i++;
setimgname(&fname[i]);
if (!name) return(-1);
strcpy(name,&fname[i]);
sprintf(fn,"%s.img",fname);
fileid=open(fn,O_RDWR);
if (fileid<0) return(fileid);
numseg=lseek(fileid,0,SEEK_END)/8192;
//printf("image open %s fd=%d load %d segments\n",fn,fileid,numseg);
lseek(fileid,0,SEEK_SET);
for(i=0; i<numseg; i++)
	{
	seg[i] = new segment;
	if (!seg[i]) return(-1);
	rc=seg[i]->load_segment(fileid,i);
	if (rc) return(rc); 
	}
return(0);
}

/*<>*/
int image::close_image(void)
{
int i;
char fn[100];
sprintf(fn,"%s.img",name);
if (fileid<0) return(fileid);
//printf("image close %s fd=%d save %d segments\n",fn,fileid,numseg);
for(i=0; i<numseg; i++)
	{
	seg[i]->save_segment(fileid,i);
	delete seg[i];
	}
close(fileid); 
free(name);
name=0;
fileid = -1;
}

/***************************** image attributes **************/

/*<>*/
int image::getnumseg(void)
/* used for display */
{
return(numseg);
}

/*<>*/
char *image::getimgname(void)
/* return image name */
{
if (name) return(name);
return((char *)"");
}

/*<>*/
void image::setimgname(char *iname)
/* set image name */
{
if (name) free(name);
name = (char *) malloc(1+strlen(iname));
if (!name) return;
strcpy(name,iname);
}

/************************   segment references  *******************/

/*<>*/
oid image::create_object(oid cid,int size)
/* create an object of a given size */
{
//printf("image create object %s %d\n",cid.aschar(),size);
int i;
oid id;
id.item = -1;
id.type  = 0;
if (numseg) id.item=seg[numseg-1]->create_object(size);
if (id.item<0)
	{
	seg[numseg] = new segment;
	if (!seg[numseg]) return(id);
	id.item=seg[numseg++]->create_object(size);
	}
id.seg=numseg-1;
if (id.item<0) return(id);
seg[id.seg]->set_class(id.item,cid);
return(id);
}

/*<>*/
void image::delete_object(oid id)
/* delete an object */
{
if (!seg[id.seg]) return;
if (id.seg>=MAX_SEGS || id.item<0 || id.type) return;
seg[id.seg]->delete_object(id.item);	
}        

/*<>*/
void image::setparent(oid id,oid pid)
/* set the parent of an object */
{
if (id.seg>=MAX_SEGS || id.item<0 || id.type) return;
seg[id.seg]->set_parent(id.item,pid);
}

/*<>*/
void image::setclass(oid id,oid cid)
/* set the class of an object */
{
if (id.seg>=MAX_SEGS || id.item<0 || id.type) return;
seg[id.seg]->set_class(id.item,cid);
}

/*<>*/
void *image::getaddress(oid id)
/* return address of an object */
{
if (id.type==OT_CLASS) id.type=0;
if (id.seg>=MAX_SEGS || id.item<0 || id.type) 
	{
	printf("image::getaddress invalid object id %d.%d.%d\n",
		id.type,id.seg,id.item);
	abort();
	}
//printf("image getaddress id=%s\n",id.aschar());
return(seg[id.seg]->get_address(id.item));
}

/*<>*/
oid image::getparent(oid id)
/* return the parent id of an object */
{
oid idr;
if (id.seg>=MAX_SEGS || id.type || id.item<0) return(idr);
return(seg[id.seg]->get_parent(id.item));
}

/*<>*/
oid image::getclass(oid id)
/* return the class id of an object */
{
oid idr;
if (id.seg>=MAX_SEGS || id.type || id.item<0) return(idr);
return(seg[id.seg]->get_class(id.item));
}

/*<>*/
void image::showmap(void)
/* show object map */
{
int i,j,rc;
oid id;
FILE *fp;
char fn[20];
sprintf(fn,"%s.omp",name);
fp=fopen(fn,"w");
if (!fp) return;
for(i=0; i<numseg; i++) 
	{
	rc=0;
	for(j=0; rc==0; j++) 
		{
		id.seg=i; id.item=j;
		rc=seg[i]->show(fp,i,j);
		}
	}
fclose(fp);
}
