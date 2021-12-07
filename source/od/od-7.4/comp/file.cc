/**********************************/
/* UNIX File System - File object */
/**********************************/

#define DEFPERM 0644
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "file.hpp"

file::file(void)
{
init();
}

void file::init(void)
{
int i;
for(i=0; i<sizeof(buffer); i++) buffer[i]=0;
filename[0]=0;
isopen=0;
rsize=0;			
fileid=0;			
empty=0;			
}

void file::setsize(int size)
{
rsize=size;
}

/*<>*/
int file::open(void)
/* This method opens a file */
{
int i;
fileid=::open(filename,O_RDWR,DEFPERM);
//fileid=::open(filename,O_RDONLY,DEFPERM);
if (fileid<0) 
	{ 
	fileid=creat(filename,DEFPERM); 
	::close(fileid); 
	fileid=::open(filename,O_RDWR,DEFPERM);
//	fileid=::open(filename,O_RDONLY,DEFPERM);
	}
if (fileid<0) return(fileid);
isopen=1;
for (i=0; i<sizeof(buffer); i++) buffer[i]=0;
//printf("file: %s opened as %d\n",filename,fileid);
return(0);
}

/*<>*/
int file::create(void)
/* This method creates a collection and returns its id */
{
long rc;
if (isopen) ::close(fileid);
isopen=0;
fileid=creat(filename,DEFPERM);
if (fileid<0) return(fileid);
::close(fileid);
rc=open();
return(rc);
}

/*<>*/
void file::close(void)
/* This method closes a collection */
{
if (!isopen) return;
::close(fileid);
//printf("file: %s closed as %d\n",filename,fileid);
isopen=0;
}

/*<>*/
void file::setname(char *fname)
{
//printf("file: %s setname to %s isopen %d \n",filename,fname,isopen);
#ifdef SETNAME
if (isopen)
	{
	::close(fileid);
	strncpy(filename,fname,sizeof(filename));
	fileid=::open(filename,O_RDWR,DEFPERM);
	}
else
#endif
	strncpy(filename,fname,sizeof(filename));
}

/*<>*/
void file::rename(char *fname)
{
if (isopen)
	{
	::close(fileid);
	::rename(filename,fname);
	strncpy(filename,fname,sizeof(filename));
	fileid=::open(filename,O_RDWR,DEFPERM);
	}
}

/*<>*/
void file::remove(void)
/* This method resets a collection */
{
if (isopen)
	{
	::close(fileid);
	unlink(filename);
	fileid=creat(filename,DEFPERM);
	::close(fileid);
	fileid=::open(filename,O_RDWR,DEFPERM);
	}
else
	unlink(filename);
}

/*<>*/
long file::first(void)
/* This method gets the id of the first object */
{
long rc;
empty=1;
if (!isopen) open();
if (!isopen) return(-1);
rc=lseek(fileid,0L,SEEK_SET);
if (rc<0) return(-1);
rc=next();
return(rc);
}

/*<>*/
long file::last(void)
/* This method gets the id of the last object */
{
long rc;
empty=1;
if (!isopen) open();
if (!isopen) return(-1);
rc=lseek(fileid,(long) -1*rsize,SEEK_END);
if (rc<0) return(-1);
rc=next();
return(rc);
}

/*<>*/
long file::next(void)
/* This method gets the id of the next object */
{
long rc;
empty=0;
if (!isopen) open();
if (!isopen) return(-1);
rc=::read(fileid,buffer,rsize);
if (rc==rsize) return(getid()); 
empty=1;
return(-1);
}

/*<>*/
long file::previous(void)
/* This method gets the id of the previous object */
{
long rc;
if (!isopen) open();
if (!isopen) return(-1);
rc=lseek(fileid,(long) -2*rsize,SEEK_CUR);
if (rc<0) 
	{	/* bug in zortech compiler */
	rc=lseek(fileid,(long) rsize,SEEK_SET);
        return(-1);
        }
rc=next();
return(rc);
}

/*<>*/
long file::current(void)
/* This method gets the id of the current object */
{
long rc;
if (!isopen) open();
if (!isopen) return(-1);
rc=lseek(fileid,(long) -1*rsize,SEEK_CUR);
if (rc<0) return(-1);
rc=next();
return(rc);
}

/*<>*/
long file::insert(void)
/* This method inserts a new object */
{
int rc,i;
//printf("file: insert=%d",rsize);
if (!isopen) open();
if (!isopen) return(-1);
lseek(fileid,0L,SEEK_END);
rc=::write(fileid,buffer,rsize);
if (rc<0) return(-1);
empty=0; 
return(getid()); 
}

/*<>*/
int file::write(char *buf,int num)
/* This method writes data to a file */
{
int rc,i;
if (!isopen) open();
if (!isopen) return(-1);
rc=::write(fileid,buf,num);
return(rc); 
}

/*<>*/
int file::read(char *buf,int num)
/* This method reads data from a file */
{
int rc,i;
if (!isopen) open();
if (!isopen) return(-1);
rc=::read(fileid,buf,num);
//printf("file: read %d bytes from %s\n",rc,filename);
return(rc); 
}

/*<>*/
long file::update(void)
/* This method updates the current object */
{
int rc;
if (!isopen) open();
if (!isopen) return(-1);
if (empty) return(0);
lseek(fileid,(long) -1*rsize,SEEK_CUR);
rc=::write(fileid,buffer,rsize);
if (rc<0) return(-1);
return(getid()); 
}

/*<>*/
void file::clear(void)
/* This method clears the current object */
{
int i;
for(i=0; i<sizeof(buffer); i++) buffer[i]=0;
}

/*<>*/
void file::get(char *to)
/* This method gets the current object */
{
int i;
for(i=0; i<rsize; i++) to[i]=buffer[i];
}

/*<>*/
void file::set(char *from)
/* This method sets the current object */
{
int i;
for(i=0; i<rsize; i++) buffer[i]=from[i];
}

/*<>*/
char *file::getaddress(void)
/* This method returns the buffer address */
{
return(buffer);
}

/*<>*/
char *file::getname(void)
{
return(filename);
}

/*<>*/
long file::getid(void)
/* This method gets the current object id */
{
if (!rsize) return(-1);
if (!isopen) open();
if (!isopen) return(-1);
return((lseek(fileid,0L,SEEK_CUR)/(long) rsize)-1);
}

/*<>*/
int file::setid(long id)
/* This method sets the current object id */
{
long rc;
if (!isopen) open();
if (!isopen) return(-1);
if (id<0)
	{
        clear();
        empty=1;
        return(-1);
        }
lseek(fileid,id*rsize,SEEK_SET);
return(next());
}
