#ifndef LIST_H
#define LIST_H

/*form linux kernel*/

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER) 

#define container_of(ptr, type, member) ({			        \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


/*  list_entry 根据成员指针找到结构体指针
 *  @ptr    成员指针
 *  @head   结构体类型
 *  @member 在结构体中链表对应的成员名称
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
	
/*  list_for_each_entry 遍历整个链表
 *  @pos    循环变量
 *  @head   链表头节点
 *  @member 在结构体中链表对应的成员名称
 */
#define list_for_each_entry(pos, head, member)				    \
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))
		 
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)
	
struct list_head {
	struct list_head *next, *prev;
};

/* INIT_LIST_HEAD 初始化链表头
 * @list 链表头名称
 */
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/* __list_add 添加节点到指定位置
 * @ new  新添加的节点
 * @ prev 插入点的前一个结点
 * @ next 插入点的下一个节点
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next  = next;
	new->prev  = prev;
	prev->next = new;
}

/* list_add_tail 添加节点到链表尾部
 * @ new  新添加的节点
 * @ head 链表头节点
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/* __list_del 删除指定位置节点
 * @ prev 要删除节点的前一个节点
 * @ next 要删除节点的后一个节点
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/*end*/

#endif