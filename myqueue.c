#include <linux/kernel.h>
#include <linux/slab.h>			//	for kmalloc
#include "myqueue.h"

//#define DEBUG



/** @brief myqueue_init This function allocates a new queue and returns it to the caller
 *  @param size The initial size of the queue
 *  @returns the pointer to the queue object or NULL if not successful
 **/
static struct myqueue_t* myqueue_init(int size) {
	struct myqueue_t*	pQueue;

	pQueue = (struct myqueue_t*)kmalloc(sizeof(struct myqueue_t), GFP_KERNEL);
	if (pQueue == NULL)
		printk (KERN_ALERT "KBDSCAN: Unable to allocate memory for the Queue !\n");
	else {
		pQueue->data = (char *)kmalloc(size, GFP_KERNEL);
		if (pQueue->data == NULL) {
			printk (KERN_ALERT "KBDSCAN: Unable to allocate memory for the queue data object !\n");
			kfree(pQueue);
			pQueue = NULL;
		} else {
			pQueue->size = size;
			pQueue->readptr = 0;
			pQueue->writeptr = 0;
		}
	}

	return pQueue;
}

/** @brief myqueue_exit This function frees a queue obejct and the related data
  * @param pQueue Contains the queue object, which needs to be created by a previous call to mqueue_init
 **/
static void myqueue_exit(struct myqueue_t *pQueue) {
	if (pQueue != NULL) {
		if (pQueue->data != NULL) {
			kfree(pQueue->data);
			kfree(pQueue);
			pQueue = NULL;
			printk (KERN_INFO "KBDSCAN: Successfully freed queue data and queue object.\n");
		}
	} else
		printk (KERN_INFO "KBDSCAN: Called exit without valid queue object !\n");
}


/** @brief myqueue_purge The purge function will discard any read items and reset the read and write pointers to the same position.
  * @param pQueue The queue object for which the purge function shall be applied.
  **/
static void myqueue_purge(struct myqueue_t *pQueue) {
	if (pQueue == NULL)
		printk (KERN_ALERT "KBDSCAN: Called purge on invalid queue data object !\n");
	else 
		pQueue->readptr = pQueue->writeptr = 0;
}

/** @brief The myqueue_put function will add an item into the queue. If the queue is already full the oldest item in the queue will be overwritten.
  * @param pQueue The queue object for which a new item shall be added.
  * @param c The item which shall be added into the queue.
  * @returns -1 if the queue object is invalid, 1 if the item has been added successfully.
  **/
static int myqueue_put(struct myqueue_t *pQueue, char c) {
	int retval = 0;
	if (pQueue == NULL) {
		printk (KERN_ALERT "KBDSCAN: Called put on invalid queue data object !\n");
		retval = -1;
	}
	else {
		pQueue->data[pQueue->writeptr++] = c;
		pQueue->writeptr %= pQueue->size;
		if (pQueue->writeptr == pQueue->readptr) {
			printk (KERN_INFO "KBDSCAN: queue had a buffer overflow.\n");
			pQueue->readptr ++;
			pQueue->readptr %= pQueue->size;
		} else {
#ifdef DEBUG
			printk (KERN_INFO "KBDSCAN: successfully put one char in queue\n");
#endif
		}
		retval = 1;
	}

	return retval;
}

/** @brief myqueue_get This function will read one item from the queue.
  * @param pQueue The queue data object from which to read an item.
  * @param c The pointer to a valid buffer object to which to copy the item.
  * @returns 0 if no item was available to read, 1 if item was successfully read from the queue or a negative value on error.
  **/
static int myqueue_get(struct myqueue_t *pQueue, char *c) {
	int retval = 0;
	if (pQueue == NULL) {
		printk (KERN_ALERT "KBDSCAN: Called get on invalid queue data object !\n");
		retval = -1;
	} else if (c == NULL) {
		printk (KERN_ALERT "KBDSCAN: Called get with invalid output element !\n");
		retval = -2;
	} else {
		if (pQueue->readptr == pQueue->writeptr) {
			//	No items available for read
			retval = 0;
		} else {
			*c = pQueue->data[pQueue->readptr++];
			pQueue->readptr %= pQueue->size;
			retval = 1;
#ifdef DEBUG
			printk (KERN_INFO "KBDSCAN: successfull received one char from queue\n");
#endif
		}
	}

	return retval;
}

/** @brief myqueue_readable This function will return the number of available/readable items in the queue.
  * @param pQueue The queue data object for which to query.
  * @returns the number of readable items or -1 on error.
  **/
static int myqueue_readable(struct myqueue_t *pQueue) {
	int retval = 0;
	if (pQueue == NULL) {
		printk (KERN_ALERT "KBDSCAN: Called readable on invalid queue data object !\n");
		retval = -1;
	} else {
		retval = pQueue->writeptr - pQueue->readptr;
		if (retval < 0)
			retval += pQueue->size;
	}

	return retval;
}


static struct myqueue_operations_t queue_ops = {
	.init 	  = myqueue_init,
	.exit 	  = myqueue_exit,
	.purge	  = myqueue_purge,
	.put	  = myqueue_put,
	.get  	  = myqueue_get,
	.readable = myqueue_readable
};

struct myqueue_operations_t *get_queue_ops(void) {
	return &queue_ops;
}
