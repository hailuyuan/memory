#include <stdio.h>
#include "mymalloc.h"
#include "list.h"


struct test_node_t {
    char * data;   
    struct list_head node_list;       
};


int main(void)
{
    /*jusr for test*/
    struct test_t{
        //char *data;
        //int  number; 
        int  arry[2];           
       };    
    struct test_t *test_t_p1[100000];
   
    for(int i=0;i<10;i++)
    {
        test_t_p1[i] = mymalloc(sizeof(struct test_t)); 
    }
     for(int i=0;i<10;i++)
    {
       printf("test_t_p1[%d] = %p\n",i,test_t_p1[i]);
    }
    
    printf("number of free block is %d\n",get_free_block_num());
    myfree(test_t_p1[0]);
    myfree(test_t_p1[1]);
   
    
    // for(int i=0;i<10;i++)
    // {
       // myfree(test_t_p1[i]); 
    // }
    printf("number of free block is %d\n",get_free_block_num());   
    // for(int i=0;i<10;i++)
    // {
        // test_t_p1[i] = mymalloc(sizeof(struct test_t)); 
    // }
     // for(int i=0;i<10;i++)
    // {
       // printf("test_t_p1[%d] = %p\n",i,test_t_p1[i]);
    // }
    return 0;
}