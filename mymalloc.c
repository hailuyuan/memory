#include "list.h"
#include <stdio.h>
#define _DEBUD_
#ifdef  _DEBUD_
#include <stdio.h>
    #define LOG(fmt,args...) printf(fmt,##args)
#else
    #define LOG(fmt,args...)
#endif

#define SIZE_OF_MALLOC    1024*1000                          /*1M Byet*/

static  unsigned char MALLOC_POLL[SIZE_OF_MALLOC];           /*预先设置堆空间*/
static  unsigned char *allign_addr = NULL;                   /*对齐后的首地址*/
static  unsigned long allign_size  = SIZE_OF_MALLOC;         /*对齐后的总大小*/


struct free_block_t{
    unsigned long size;
    unsigned char *start;
    unsigned char *end;
    struct list_head node_list;
    unsigned char is_empty;
	
}__attribute__((aligned(4)));

static struct free_block_t *free_block_p = NULL;

static struct list_head *p_head = NULL;

/* mymalloc_init  内存分配初始化
 * @ NULL
 */
 
LIST_HEAD(head);

int mymalloc_init()
{
    unsigned long poll_addr;
    INIT_LIST_HEAD(&head);
    p_head = &head;
    
    LOG("head = %p\n",p_head);
    
    /*1、对齐地址,32位处理器为4字节*/
    
    poll_addr = (unsigned long)MALLOC_POLL;
    
    LOG("first : poll_addr = 0x%08x\n",poll_addr);

    if(poll_addr & 0x7)
    {
        allign_size = SIZE_OF_MALLOC - (poll_addr & 0x7);  
    }
    allign_addr = (unsigned char *)poll_addr;
    
    LOG("second : allign_size = %ld allign_addr = 0x%p\n",allign_size,allign_addr);
    
    /*2、初始化空闲链表*/
    
    free_block_p            = (void *)allign_addr;   /*防止编译器警告*/  
    free_block_p->size      = allign_size - sizeof(struct free_block_t);
    free_block_p->start     = (void *)allign_addr;
    free_block_p->end       = (void *)allign_addr + allign_size;
    free_block_p->is_empty  = 1;
    free_block_p->node_list.next = &(free_block_p->node_list);
    free_block_p->node_list.prev = &(free_block_p->node_list);
    
    list_add_tail(&(free_block_p->node_list),p_head);
    
    LOG("p_head->next = %p p_head->prev = %p\n",p_head->next,p_head->prev);
    LOG("third size = %d start = 0x%p end = 0x%p free_block_p = 0x%p\n",free_block_p->size, \
        free_block_p->start,free_block_p->end,free_block_p);
    
	return 0;
}

void *mymalloc(unsigned long _size)
{
    void *ret  = NULL;
    long  size = _size;
    long  bk_remain_size = 0;
    void *addr = NULL;
    struct free_block_t *k;
    /*1、判断输入参数，并且对齐*/
    if(p_head == NULL)
        mymalloc_init();
    if(_size > allign_size && _size <1)
        return ret;
    if(_size & 0x7)
    {
        size = _size + (0x8 - _size & 0x7);  
    }
    LOG("first size = %ld\n",size);
    LOG("p_head->prev->next = %p\n",p_head->prev);
    /*2、寻找空闲块*/
    list_for_each_entry(k,p_head,node_list)                 /*找到合适的空闲块*/
    {
        if(k->is_empty && k->size >= size)
        {
            LOG("k->start = %p\n", k->start);
            addr = k->start + sizeof(struct free_block_t); 
            
            LOG("start addr = %p\n",addr);
            
            bk_remain_size = k->end - k->start - sizeof(struct free_block_t) - size;
            
            LOG("bk_remain_size = %ld",bk_remain_size);
            
            k->end = addr + size;      /*截取空闲块*/
            
            LOG(" end addr = %p\n",k->end );
            k->is_empty = 0;   
            
            if(bk_remain_size > sizeof(struct free_block_t))
            {
                free_block_p                 = (void *)k->end;       /*建立新的内存控制块*/
                free_block_p->start          = free_block_p;
                free_block_p->end            = (void *)free_block_p + bk_remain_size;
                free_block_p->size           = bk_remain_size - sizeof(struct free_block_t);
                free_block_p->is_empty       = 1;
                free_block_p->node_list.next = &(free_block_p->node_list);
                free_block_p->node_list.prev = &(free_block_p->node_list);
                
                LOG("node_list = %p\n", &free_block_p->node_list);
                
                LOG("next = %p prev = %p\n",free_block_p->node_list.next,free_block_p->node_list.prev);
                
                LOG("head = %p\n",p_head);
                
                LOG("p_head->prev->next = %p\n",p_head->prev);
                
                LOG("pos = 0x%p\n",&k);
                
                list_add_tail(&(free_block_p->node_list),p_head);
            }
            LOG("found block node \n");
            break;
        }
        else
        {
            LOG("find a used block\n");
            continue;
            //ret = NULL;//list_add_tail(p_head,p_head);
        }
    }
        
    ret = addr;
    return ret; 
} 

void myfree(void *p)
{
    struct free_block_t *k = p - sizeof(struct free_block_t);                     /*找到空闲块结构体的首地址*/
    k->is_empty = 1;
    if(k->node_list.prev != p_head)                                               /*空闲块的前一个不是head*/
    {
        if(list_entry(k->node_list.prev,struct free_block_t,node_list)->is_empty == 1)
        {   
            LOG("delete a free block \n");
            LOG("k->node_list.prev->next = %p \n",k->node_list.prev->next);
            LOG("k->node_list.next->prev = %p \n",k->node_list.next->prev);
            
            
            LOG("k->node_list.prev = %p \n",k->node_list.prev);
            LOG("k->node_list.next = %p \n",k->node_list.next);
            
            __list_del((k->node_list.prev),(k->node_list.next));
           // k->node_list.prev->next = k->node_list.next;
           // k->node_list.next->prev = k->node_list.prev;
            LOG("k->node_list.prev->next = %p \n",k->node_list.prev->next);
            LOG("k->node_list.next->prev = %p \n",k->node_list.next->prev);
        }
    }
    if(k->node_list.next != p_head)                                               /*空闲块的后一个不是head*/
    {
        if(list_entry(k->node_list.next,struct free_block_t,node_list)->is_empty == 1)
        {
            LOG("delete b free block \n");
            __list_del(&(k->node_list),(k->node_list.next->next));  
        }
    }
    LOG("myfree k = %p\n",k);
    LOG("myfree k = %ld\n",k->size);
}

int get_free_block_num(void)
{   
    struct free_block_t *k = NULL;
    int num = 0;
    list_for_each_entry(k,p_head,node_list) 
    {
        LOG("free_block_ = %p\n",k);
        num++;
        LOG("node_list = %p\n",k->node_list);
    }
       
    return num;  
}


























