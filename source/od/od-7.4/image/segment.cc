#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "oid.hpp"
#include "segment.hpp"

#define MAX_OBJS     100
short VERSION = 0x0402;

segment::segment(void)
{
init();
}

void segment::init(void)
{
int i;
offset=0;
version=VERSION;
for(i=0; i<MAX_OBJS; i++) obj[i].init();
}

void *segment::get_address(int item)
{
int start;
if (item<0 || item>=MAX_OBJS) return(0);
start=obj[item].get_start();
//printf("segment getaddress start=%d data=%p\n",start,&data[start]);
return((void *) &data[start]);
}

/*<>*/
int segment::save_segment(int fileid,int sn)
{
lseek(fileid,sn*sizeof(segment),SEEK_SET);
write(fileid,&obj,sizeof(obj));
write(fileid,&offset,sizeof(offset));
write(fileid,&version,sizeof(version));
write(fileid,data,sizeof(data));
// printf("segment saved %d offset %d\n",sn,offset);
return(0);
}

/*<>*/
int segment::load_segment(int fileid,int sn)
{
lseek(fileid,sn*sizeof(segment),SEEK_SET);
read(fileid,&obj,sizeof(obj));
read(fileid,&offset,sizeof(offset));
read(fileid,&version,sizeof(version));
if (version!=VERSION) 
	{
	printf("Incorrect image versions\n");
	//exit(0);
	}
read(fileid,data,sizeof(data));
// printf("segment loaded %d offset %d\n",sn,offset);
return(0);
}

/*<>*/
int segment::create_object(int size)
/* create space for an object of given size and return its id */  
{
int i,off;
if ((offset+size)>=sizeof(data)) return(-1);
for(i=0; i<MAX_OBJS; i++)
	{
        if (!obj[i].get_ref())
        	{
//printf("segment create object item %d offset %d size %d addr %p\n",i,offset,size,&obj[0]);
		obj[i].init();
                obj[i].set_start(offset);
		offset+=size;
		obj[i].set_size(size);
                obj[i].set_ref(1);
                return(i);
                }
	}                
return(-1);
}

/*<>*/
void segment::delete_object(int item)
/* delete object and free its space */
{
if (item<0 || item>=MAX_OBJS) return;
obj[item].set_ref(0);
return;
printf("seg: object deleted size %d start %d offset %d\n",
	obj[item].get_size(),
	obj[item].get_start(),
	offset);
}

oid segment::get_parent(int item)
{
oid empty;
if (item<0 || item>=MAX_OBJS) return(empty);
return(obj[item].get_parent());
}

void segment::set_parent(int item,oid pid)
{
if (item<0 || item>=MAX_OBJS) return;
obj[item].set_parent(pid);
}

oid segment::get_class(int item)
{
oid empty;
if (item<0 || item>=MAX_OBJS) return(empty);
return(obj[item].get_class());
}

void segment::set_class(int item,oid cid)
{
if (item<0 || item>=MAX_OBJS) return;
obj[item].set_class(cid);
}

int segment::get_ref(int item)
{
if (item<0 || item>=MAX_OBJS) return(0);
return(obj[item].get_ref());
}

/*<>*/
int segment::show(FILE *fp,int seg,int objn)
{
if (objn<MAX_OBJS)
	{
	obj[objn].show(fp,seg,objn);
	return(0);
	}
return(-1);
}
