#ifndef __myqueue_h__
#define __myqueue_h__


struct myqueue_t {
	int	size;		//	Size of the queue
	char	*data;		//	the queue data --> will be of size <size>
	int	readptr;	//	pointer into the round robin queue for reading
	int	writeptr;	//	pointer into the round robin queue for writing
};

struct myqueue_operations_t {
	struct myqueue_t*	(*init)		(int);
	void			(*exit)		(struct myqueue_t*);

	void			(*purge)	(struct myqueue_t*);		//	empty the queue
	int			(*put)		(struct myqueue_t*, char);	//	add one item into the queue
	int			(*get)		(struct myqueue_t*, char*);	//	get one item from the queue
	int			(*readable)	(struct myqueue_t*);		//	get the number of readable items in the queue
};


extern struct myqueue_operations_t *get_queue_ops(void);


#endif
