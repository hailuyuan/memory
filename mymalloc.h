#ifndef MYMALLOC_H
#define MYMALLOC_H

extern int mymalloc_init();
extern void *mymalloc(unsigned long _size);
extern void myfree(void *p);
extern int get_free_block_num(void);
#endif