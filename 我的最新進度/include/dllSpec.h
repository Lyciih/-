#ifndef DLLSPEC_H_INCLUDED
#define DLLSPEC_H_INCLUDED

#define NULL ((void *)0)

extern int printf(const char* s, ...);
extern void * malloc(int size);
extern void free(void *ptr);




typedef struct dllNode_t {
    struct dllNode_t *prev;
    struct dllNode_t *next;
} dllNode_t;





dllNode_t * DLL_init(); //建構一個空的 list

int DLL_isEmpty(const dllNode_t *head);// head 是否為空的list

dllNode_t * DLL_next_node(const dllNode_t * node);//(下一個節點)

dllNode_t * DLL_prev_node(const dllNode_t * node);//(上一個節點)

unsigned int DLL_num_nodes(const dllNode_t *head);//計算List中有幾個node

void DLL_add_first(dllNode_t * new_node, dllNode_t * head);//(將新new_node加入到head List 的第一個)

void DLL_add_tail(dllNode_t * new_node, dllNode_t *head);//(將新new_node加入到head List 的最後一個)

void DLL_addto_prev(dllNode_t *new_node, dllNode_t *node);// (將新new_node加入到node的前一個)

void DLL_addto_next(dllNode_t *new_node, dllNode_t *node);// (將新new_node加入到node的後一個)

void DLL_delete(dllNode_t * node);//(從node所在的 Linked List 中刪除此點)

dllNode_t * DLL_concate(dllNode_t *srcList, dllNode_t * dstList);//(將srcList 串在dstList之後)



//-------------以下是自己加的function----------------------------------------------------------------------------------------------------------------------


dllNode_t *DLL_get_tail(dllNode_t *head);//取得list的尾巴

void DLL_free_head(dllNode_t *head);//釋放空的head

#endif