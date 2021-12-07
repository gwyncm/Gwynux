#ifndef SEGMENT
#define SEGMENT
#include "object.hpp"
class segment
	{
	object	 obj[100];
        short 	 offset;
        short 	 version;
        char 	 data[6788];
public:
	void	init(void);
	int	load_segment(int fileid,int segnum);
	int	save_segment(int fileid,int segnum);
	int 	create_object(int size);
	void 	delete_object(int item);
	int 	show(FILE *fp,int seg,int objn);
// object
	oid	get_class(int item);
	void	set_class(int item,oid cid);
	oid	get_parent(int item);
	void	set_parent(int item,oid pid);
	int	get_ref(int item);
	void	*get_address(int item);
		segment(void);
	};
#endif
